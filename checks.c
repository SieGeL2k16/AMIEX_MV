// Headerfile: CHECK-ROUTINEN fuer mEGA-vIEW V2.++

#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include <libraries/fileid.h>					// Die benoetigten Headerfiles und
#include <libraries/fileid_IDDefs.h>	// Definitionen von FileID.library
#include <proto/fileid.h>
#include "global_defines.h"
#include "struct_ex.h"
#include <clib/utility_protos.h>
#include <proto/utility.h>

BOOL 	checkram(long);
BOOL 	pathmemcheck(char *,long);
void 	pipecheck(char *);
BOOL 	checkformount(char *);
void 	checkext(void);
BOOL 	checkforfile(char *);
void 	checkfortemp(char *);
BOOL 	checkspecial(void);
BOOL 	checkfile(void);
void 	GetComment(char *file);
BOOL 	CTRL(void);		 // CTRL-C (Break) Abfrage

struct 	DiskObject *checkicon=NULL;					// Globaler Ptr.

void GetComment(char *file)
	{
	BPTR	lo;										// Lock fuer File

	*glob='\0';
	if(!(lo=Lock(file,ACCESS_READ)))
		{
		SPrintf(glob,"\n\r[31mERROR: [36mCouldn't 'lock' [33m%s !\n\n\r",FilePart(file));
		wb(glob);
		};
	if(Examine(lo,myfib)==FALSE)
		{
		UnLock(lo);
		SPrintf(glob,"[31mERROR: [36mFile [33m%s [36mdoesn't exist[34m...[31mABORTING !\n\n\r",FilePart(file));
		wb(glob);
		}
	UnLock(lo);
	if(!(Stricmp(myfib->fib_Comment,"RESTRICTED"))||(!(Stricmp(FilePart(fullname),"USER.DATA")))) writeclog(fullname);
	}

BOOL CTRL(void)
	{
	char	str[5],c[3];

	change(str,1,GETKEY);
	if(Atol(str)) { *str='\0'; if(JH_HK(str)<0) wb(MY_LOST_CARRIER); }
	SPrintf(c,"%lx",str[0]);
	if(Atol(c)==3) return(FALSE);
	else return(TRUE);
	}

BOOL checkfile(void)
	{
	BOOL	patt=FALSE;

	if(checkspecial()==FALSE) return(FALSE);
	*glob='\0';
	if(!(Stricmp(fname,"$")))
		{
		SPrintf(glob,"[35mmEGA-vIEW [mV%s [36m([m%s %s[36m) - ",VIEW_VER,__DATE__,__TIME__);
		inoutput(glob,3);
		SPrintf(glob,"%s [mV%ld.%ld\n\n\r",FILEIDNAME,FileIDBase->lib_Version,FileIDBase->lib_Revision);
		wb(glob);
		};
	switch(ParsePattern(fname,glob,1000))
		{
		case 1:   patt=TRUE;
							break;
		case 0:		patt=FALSE;
							break;
		case -1:	wb("\n\n\r[36mBUFFER OVERFLOW!\n\n\r");
		};
	if(patt==TRUE)
		{
		if(readdir()==FALSE) return(FALSE);
    *glob='\0';
		}
	else
		{
		if(checkforfile(fname)==FALSE)
			{
			SPrintf(glob,"\n\r[36mFile [33m%s [36mnot found !\n\r",fname);
			inoutput(glob,3);
			return(FALSE);
			}
		else
			{
			if(special==TRUE) strcpy(fullname,fname);
			makeupper(fullname);
			}
		}
	if(getfilelength(fullname)==FALSE)
		{
		inoutput("\n\r[31mERROR: [36mCouldn't read filelength !\n\n\r",3);
		return(FALSE);
		}
	}

BOOL checkspecial(void)
	{
	char	errbuf[81],sep[1],*t;
	int		errnum=0;
	BPTR	fp;
  BOOL	sp=FALSE;

	*glob='\0';
	t=fname;
	while(*t)
		{
		if(*t==':'||*t=='/') { sp=TRUE;break;}
		else *t++;
		};
  if(sp==TRUE)
		{
		if(sysflag==FALSE) { inoutput("\n\r[mYou may not include any special symbols ! \n\n\r",3); return(FALSE);};
		sep[0]=':';
		if(SplitName(fname,(ULONG) sep[0],pathbuf,0,998)!=-1)
			{
			strcat(pathbuf,":");
 			special=TRUE;
			if(checkformount(pathbuf)==FALSE) { errnum=218; goto errormsg;};
			switch(ParsePattern((FilePart(fname)),glob,198))
				{
				case  1:	return(TRUE);
        case -1:	wb("\n\rBUFFER OVERFLOW!!!\n\r");
				default:	break;
				}
			if(!(fp=Lock(fname,ACCESS_READ))) errnum=IoErr();
			else UnLock(fp);
			}
		errormsg:
		if(errnum)
			{
			Fault(errnum,NULL,errbuf,80);
			*glob='\0';
			SPrintf(glob,"[31mERROR: [36m%s\n\r",errbuf);
			inoutput(glob,3);
			return(FALSE);
			}
		}
	else special=FALSE;
	return(TRUE);
	}

void checkfortemp(char *pfad)
	{
	BPTR	lock;

	if(!(lock=Lock(pfad,ACCESS_READ))) return;
	else {UnLock(lock);Loesche_File(pfad);}
	}

BOOL checkforfile(char *name)
	{
	char	*entry;
	int		z=1;
	BOOL	check=FALSE;
	BPTR	lock;

	if(special==TRUE) return(TRUE);
	change(pathbuf,1,BB_CONFLOCAL);
	pathbuf[strlen(pathbuf)-1]='\0';
	if(!(checkicon=GetDiskObject(pathbuf))) wb("\n\r[31mERROR: [36mCONF.ICON not found !\n\n\r");
	*pathbuf='\0';
	SPrintf(pathbuf,"DLPATH.%ld",z);
	while(entry=FindToolType(checkicon->do_ToolTypes,pathbuf))
		{
		*glob='\0';
		SPrintf(glob,"%s%s",entry,fname);
		if(lock=Lock(glob,ACCESS_READ))
			{
			UnLock(lock);
			strncpy(fullname,glob,198);
			check=TRUE;
			break;
			}
		else
			{
			z++;
			SPrintf(pathbuf,"DLPATH.%ld",z);
			}
		};
	FreeDiskObject(checkicon);
	checkicon=NULL;
	return(check);
	}

void checkext(void)
	{
	struct FI_FileInfo *finfo;

	if(!(finfo=FIAllocFileInfo())) wb(NO_MEM);
	FIIdentifyFromName(finfo,fullname);
	strcpy(fid_desc,finfo->FI_Description);
	fid_num=finfo->FI_ID;
	gfc=finfo->FI_GlobalFileClass;
	if(gfc&1) fid_num=FID_UNKNOWNEXE;
	FIFreeFileInfo(finfo);
	makeupper(fid_desc);
	}

BOOL checkformount(char *drive)		// assign/drive + ':'
	{
	struct 	DosList *dl;
	ULONG 	flags = LDF_DEVICES|LDF_VOLUMES|LDF_ASSIGNS|LDF_READ;

	drive[strlen(drive)-1]='\0';
	dl=LockDosList(flags);
	dl=FindDosEntry(dl,drive,flags);
	UnLockDosList(flags);
	if(!dl) return(FALSE);
	else return(TRUE);
	}

void pipecheck(char *cmd)
	{
	char	pipename[200],s1[200],s2[200];
	long	pipesize;
	long	e=NULL,
				i=NULL;
	BPTR	pipefile;
	long	error;

	inoutput("[m\n\r",3);
	if(error=SystemTagList(cmd,NULL)) errors("Pipecheck",error);
	SPrintf(pipename,"PIPE:OUTPUT.%ld",slot);
	if(!(pipefile=Open(pipename,MODE_OLDFILE))) wb("\n\r[31mERROR WITH PIPE!\n\r");
	do
		{
		if(CTRL()==FALSE)
			{
			SPrintf(s1,"%lc\n\r[m***Break, wait until process is finished !\n\r",(char) 15);
			inoutput(s1,3);
			break;
   		};
		pipesize=Read(pipefile,pipename,100);
		pipename[pipesize]='\0';
		if (pipesize>0) i=e=NULL;
		while(pipename[e]!='\0')
			{
			s2[i]=pipename[e];
			if(pipename[e]=='\n')
				{
				i++;
				s2[i]='\r';
				}
			i++;e++;
			}
		s2[i]='\0';
		inoutput(s2,3);
		*s2='\0';
		}
	while(pipesize);
	while(pipesize) pipesize=Read(pipefile,pipename,198);
	Close(pipefile);
	}
BOOL pathmemcheck(char *pfad,long needed)
	{
	char		sep[1],str[200];
	struct  InfoData __aligned di;
	BPTR		testlock;

	*str='\0';
	sep[0]=':';
	if(SplitName(pfad,(ULONG) sep[0],str,0,198)!=-1)
		{
		strcat(str,":");
		if(!(Stricmp(str,"RAM:")))
			{
			if(checkram(needed)==FALSE) return(FALSE);
			else return(TRUE);
			};
		if(!(testlock=Lock(str,ACCESS_READ))) return(FALSE);
		if(Info(testlock,&di)==FALSE) {UnLock(testlock);return(FALSE);};
		UnLock(testlock);
		if(needed>((di.id_NumBlocks-di.id_NumBlocksUsed)*di.id_BytesPerBlock)) return(FALSE);
  	else return(TRUE);
		}
  else return(FALSE);
	}

BOOL checkram(long needed)
	{
	if(needed>=((AvailMem(MEMF_PUBLIC))-10000L)) return(FALSE); // 10kb Sicherheitshalber!
	else return(TRUE);
	}
