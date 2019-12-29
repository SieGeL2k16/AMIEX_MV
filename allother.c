#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "struct_ex.h"
#include "global_defines.h"
#include "proto.h"
#include <libraries/fileid_IDDefs.h>	// Definitionen von FileID.library

void allother(void);
void iconinfo(void);
void CheckForVersion(char *);			// Prueft auf vorhandene Versionsnummer
struct DiskObject *alot=NULL;			// Mein Globaler Ptr.

extern char *Ver;

char	mystr[200];

void allother(void)
	{
	if(fid_num==121)
		{
		inoutput("[31mWARNING: [36mSelected File is a Directory[34m...[36mviewing not possible !\n\r",3);
		return;
		};
	SPrintf(pathbuf,"\r[K\r[36mFile [35m%s %s([33m%ld [36mBytes%s) [36midentified as [34m: [32m%s\n\r",FilePart(fullname),WEISS,totalsize,WEISS,fid_desc);
  smp(pathbuf,0);
	CheckForVersion(fullname);
	if(AskBool("\n\n\r[36m[28CView anyway",FALSE)==TRUE)
		{
		if(view==FALSE) {inoutput(NO_VIEW,3);return;};
		showhex(fullname);
		};
	if(extract==TRUE) Loesche_File(fullname);
	extract=FALSE;
	}

void CheckForVersion(char *file)
	{
	BPTR	myopen;
	long	pos;

	switch(fid_num)
		{
		case	FID_GIFPICTURE:
	  case	FID_MSDOSEXE:
		case	FID_JPEG:
		case	FID_ILBM:
		case	FID_8SVX:
		case	FID_UNKNOWNIFF:
		case 	FID_DISKICON:
		case  FID_DRAWERICON:
		case	FID_TRASHCANICON:
		case 	FID_DEVICEICON:
		case	FID_KICKICON:
		case	FID_APPICON:
		case	FID_UNKNOWNICON:
		case	FID_UNKNOWNXPK:	return;
		}
	*pathbuf='\0';
	if(!(myopen=Open(file,MODE_OLDFILE))) { inoutput(NO_FILE,3);return;};
	strcpy(pathbuf,Ver);
	pathbuf[5]='\0';
	pos=FSearch(pathbuf,myopen);
	if(pos!=-1)
		{
		Seek(myopen,pos+5,OFFSET_BEGINNING);
		FGets(myopen,mystr,199);
		}
	else
		{
		Seek(myopen,0L,OFFSET_BEGINNING);
		strcpy(mystr,FilePart(file));
		makeupper(mystr);
		pos=FSearch(mystr,myopen);
		if(pos==-1) strcpy(mystr," [31mNot found !");
    else
			{
      Seek(myopen,strlen(mystr)+1,OFFSET_CURRENT);
      *glob='\0';
	    FGets(myopen,glob,100);
			SPrintf(mystr," %s",glob);
			}
		}
	Close(myopen);
  SPrintf(pathbuf,"\n\r[36mVersion[33m:[32m%s\r",mystr);
	smp(pathbuf,0);
	}

void iconinfo(void)
	{
	char		**tt;
	long	  counta=0L,zeilencount=0L;

	*mystr='\0';
	SPrintf(mystr,"\r[K\r[36mFile [35m%s[36m identified as %s[34m...[36m",FilePart(fullname),fid_desc);
	inoutput(mystr,3);
	if(AskBool("view it",TRUE)==FALSE) return;
	if(view==FALSE) {inoutput(NO_VIEW,3);return;};
	SPrintf(mystr,"\r[36mList of Tooltypes from [35m%s[34m...\n\n\r",FilePart(fullname));
	inoutput(mystr,3);
	strncpy(mystr,fullname,199);
	mystr[(strlen(mystr)-5)]='\0';	//.info cutten
	if(!(alot=GetDiskObject(mystr))) { inoutput(fehler,3); return;}
	tt=alot->do_ToolTypes;
	if(tt)
		{
		while(*tt)
			{
			SPrintf(mystr,"[m%s\n\r",*tt);
			inoutput(mystr,3);
			*tt++;counta++;zeilencount++;
			if(zeilencount>mlines-5) { Get_Key();inoutput("\f\n\r",3);zeilencount=0L;};
			};
   	};
	FreeDiskObject(alot);alot=NULL;
	if(!counta) inoutput("[32mNo ToolTypes found !\n\r",3);
	else inoutput("\n\r",3);
	if(extract==TRUE) {extract=FALSE;Loesche_File(fullname);};
	}
