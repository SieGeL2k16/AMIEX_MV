// Directory(ies) durchsuchen nach filename

#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include <clib/utility_protos.h>
#include <proto/utility.h>
#include "global_defines.h"
#include "struct_ex.h"

int  	 Show_Found(char *,long);
BOOL 	 readdir(void);
struct DiskObject *conficon=NULL;

BOOL readdir(void)
	{
	char		*fullpath,*entry,*mybuf,suchname[200],confpath[200];
	long 		z=1;
	long		temprc;
	struct 	AnchorPath __aligned ua;
	BOOL		found=FALSE;

	*glob='\0';
	memset(&ua,0,sizeof(struct AnchorPath));
  if(special==TRUE)
		{
		strcpy(suchname,fname);
		if(sysflag==FALSE) return(FALSE);
		if(!(fullpath=(char *)AllocVec(300,MEMF_CLEAR|MEMF_PUBLIC))) wb(NO_MEM);
		strcpy(ua.ap_Buf,fullpath);
		ua.ap_Strlen=299;
		SPrintf(pathbuf,"\r[K\r[36mSearching for [35m%s[34m...",fname);
		smp(pathbuf,0);
		MatchFirst(suchname,&ua);
		if(temprc=IoErr()||CTRL()==FALSE)
			{
			MatchEnd(&ua);
			FreeVec((char *)fullpath);
			SPrintf(pathbuf,"\n\n\r[36mFile [33m%s [36mnot found !\n\r",suchname);
      smp(pathbuf,0);
			return(FALSE);
			}
		if(ua.ap_Info.fib_DirEntryType<0)
			{
			switch(Show_Found(ua.ap_Info.fib_FileName,ua.ap_Info.fib_Size))
				{
				case 0:		strcpy(fullname,ua.ap_Buf);
									strcpy(fname,ua.ap_Info.fib_FileName);
									found=TRUE;
									break;
				case 1: 	found=FALSE;
									break;
				case -1:	MatchEnd(&ua);
									FreeVec((char *)fullpath);
									inoutput("\n\r",3);
									return(FALSE);
				case -2:	MatchEnd(&ua);
                  FreeVec(fullpath);
									FreeDiskObject(conficon);
									wb(MY_LOST_CARRIER);
				}
			}
		while(1)
			{
			if(found==TRUE) break;
			SPrintf(pathbuf,"\r[K\r[36mSearching for [35m%s[34m...",fname);
			smp(pathbuf,0);
			MatchNext(&ua);
			if(temprc=IoErr()||CTRL()==FALSE) break;
			if(ua.ap_Info.fib_DirEntryType>0) continue;
			switch(Show_Found(ua.ap_Info.fib_FileName,ua.ap_Info.fib_Size))
				{
				case 0:		strcpy(fullname,ua.ap_Buf);
									strcpy(fname,ua.ap_Info.fib_FileName);
									found=TRUE;
									break;
				case 1: 	found=FALSE;
									break;
				case -1:	MatchEnd(&ua);
									FreeVec((char *)fullpath);
									inoutput("\n\r",3);
									return(FALSE);
				case -2:	MatchEnd(&ua);
                  FreeVec(fullpath);
									FreeDiskObject(conficon);
									wb(MY_LOST_CARRIER);

				}
			}
		MatchEnd(&ua);
		FreeVec((char *)fullpath);
		if(found==FALSE) { inoutput("\n\n\r[36mNo more files found !\n\r",3);return(FALSE);}
		else return(TRUE);
		}

//------------ AB HIER ISS DIE NORMALE BBSDIR-SCANROUTINE -----------------

	strcpy(suchname,fname);
	*glob='\0';
	change(confpath,1,BB_CONFLOCAL);
	confpath[strlen(confpath)-1]='\0';
	if(!(conficon=GetDiskObject(confpath))) wb("\n\rERROR: CONF.ICON NOT FOUND !\n\n\r");
	if(!(mybuf=AllocVec(1024L,MEMF_CLEAR|MEMF_PUBLIC))) { FreeDiskObject(conficon);wb(NO_MEM);};
	if(!(fullpath=(char *)AllocVec(300,MEMF_CLEAR|MEMF_PUBLIC))) { FreeVec(mybuf);FreeDiskObject(conficon);wb(NO_MEM);}
  strcpy(ua.ap_Buf,fullpath);
	ua.ap_Strlen=299;
	while(found==FALSE)
		{
		SPrintf(pathbuf,"\r[K\r[36mSearching for [35m%s [36min path [32m%ld",fname,z);
		smp(pathbuf,0);
		SPrintf(glob,"DLPATH.%ld",z);
		if(!(entry=FindToolType(conficon->do_ToolTypes,glob))) break;
		*mybuf='\0';
		SPrintf(mybuf,"%s%s",entry,suchname);
		if(CTRL()==FALSE) break;
		MatchFirst(mybuf,&ua);
		if(temprc=IoErr())
			{
			MatchEnd(&ua);
			z++;
			continue;
			}
		if(ua.ap_Info.fib_DirEntryType<0)
			{
			switch(Show_Found(ua.ap_Info.fib_FileName,ua.ap_Info.fib_Size))
				{
				case 0:		strcpy(fullname,ua.ap_Buf);
									strcpy(fname,ua.ap_Info.fib_FileName);
									found=TRUE;
									break;
				case 1: 	found=FALSE;
									break;
				case -1:	MatchEnd(&ua);
									FreeVec(fullpath);
									FreeVec(mybuf);
									FreeDiskObject(conficon);
									inoutput("\n\r",3);
									return(FALSE);
				case -2:	MatchEnd(&ua);
                  FreeVec(fullpath);
									FreeVec(mybuf);
									FreeDiskObject(conficon);
									wb(MY_LOST_CARRIER);
				}
			}
		while(1)
			{
			if(found==TRUE) break;
			SPrintf(pathbuf,"\r[K\r[36mSearching for [35m%s [36min path [32m%ld",fname,z);
			smp(pathbuf,0);
			MatchNext(&ua);
			if(temprc=IoErr()) break;
			if(ua.ap_Info.fib_DirEntryType>0) continue;
			switch(Show_Found(ua.ap_Info.fib_FileName,ua.ap_Info.fib_Size))
				{
				case 0:		strcpy(fullname,ua.ap_Buf);
									strcpy(fname,ua.ap_Info.fib_FileName);
									found=TRUE;
									break;
				case 1: 	found=FALSE;
									break;
				case -1:	MatchEnd(&ua);
									FreeVec((char *)fullpath);
                  FreeVec(mybuf);
									FreeDiskObject(conficon);
									inoutput("\n\r",3);
									return(FALSE);
									break;
				case -2:	MatchEnd(&ua);
                  FreeVec(fullpath);
									FreeVec(mybuf);
									FreeDiskObject(conficon);
									wb(MY_LOST_CARRIER);
				}
			}
		if(found==FALSE) { MatchEnd(&ua);z++;}
		}
	FreeDiskObject(conficon);
	MatchEnd(&ua);
	FreeVec((char *)fullpath);
	FreeVec(mybuf);
	if(found==FALSE) { *pathbuf='\0';SPrintf(pathbuf,"\n\n\r[36mFile [33m%s [36mnot found !\n\n\r",suchname);smp(pathbuf,0);return(FALSE);}
	else return(TRUE);
	}


// FUNKTION: Anzeigen der gefundenen Filenamen
// INPUTS  : *name = Filename, size = Groesse des Files in Bytes
// RETURN  : 1 = NO, -1 = QUIT, 0 = YES

int Show_Found(char *name,long size)
	{
	char  showstr[200];

	SPrintf(showstr,"\r[K\r[36mFound File [35m%s [m([33m%ld [36mBytes[m) ",name,size);
	inoutput(showstr,3);
	inoutput("[36m- Okay [32m([33mY[32m/[33mn[32m/[33mq[32m)[36m ? ",3);
	*showstr='\0';
	if(JH_HK(showstr)<0) return(-2);
	switch(ToUpper(*showstr))
		{
		case 'N':	inoutput("[34mNo !",3);
							return(1);
							break;
		case 'Q':	inoutput("[31mQuit !\n\n\r",3);
							return(-1);
							break;
		default:	inoutput("[32mYes !\n\n\r",3);
							return(0);
							break;
		}
	}

