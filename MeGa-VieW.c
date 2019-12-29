/****************************************************************************
 *                                                                          *
 *        MeGa-VieW by S¡eGeL/TRSi  -  Textviewer for Amiex V3.xx & FAME    *
 *                                                                          *
 *  (c) 1993/1994 by (MYSTiC/X-DeSiGN) - (c) 1994-96 by (tRSi/X-iNNOVATiON) *
 *                                                                          *
 ****************************************************************************/
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
#include <time.h>
#include <libraries/fileid.h>					// Die benoetigten Headerfiles und
#include <libraries/fileid_IDDefs.h>	// Definitionen von FileID.library
#include <proto/fileid.h>
#include <clib/utility_protos.h>
#include <proto/utility.h>
#include "global_defines.h"
#include "struct.h"
#include "proto.h"
#define MEM_ATTRIBUTES MEMF_PUBLIC|MEMF_CLEAR

extern char *_ProgramName;							// PRGName von mEGA-vIEW

struct 	Library *FileIDBase=NULL;
struct 	Library *IconBase=NULL;
struct 	Library	*UtilityBase=NULL;

//--- VersionsString des Programms ---

char *Ver="$VER:mEGA-vIEW "VIEW_VER" "__AMIGADATE__"\0";
long	ArgArray[1]={ 0L };

char  XYZ[80];

struct  Process *process;
APTR	temp;

//--- Alle ProtoTypen von MeGa-VieW-MAIN  ---

void 	wb(char *);												// Verlaesst das Programm
void 	getfilename(void);								// Abfrage nach Filenamen
BOOL 	getfilelength(char *);						// Ermittelt Filelaenge
void	allother(void);										// Unbekannter FileTyp
void	jumpext(void);										// Verzweigt anhand des Filetypes
void	executeit(void);									// Checkt Extension und verzweigt
void	showguide(char *);								// AmigaGuide-Files anzeigen
void 	writeclog(char *);								// Meldung ins Callerslog schreiben
void	Write_Server(char *,BOOL);				// Wenn FALSE, dann ->mEGA-vIEW Vx.x
void 	builtmain(void);									// Titel von MeGa-VieW
void 	CloseMyPtr(void);

void main(void)
	{
	char    mainline[100],
					**parms;
	struct  RDArgs *rda=NULL;

	if(DOSBase->dl_Root->rn_Flags&RNF_WILDSTAR) {} // Dann isses gesetzt!
	else DOSBase->dl_Root->rn_Flags=DOSBase->dl_Root->rn_Flags^RNF_WILDSTAR;
	Decrypt(XYZ);
	if(!(rda=ReadArgs("NODE/N/A",ArgArray,rda)))
		{
		Printf("\nmEGA-vIEW V%s written by SieGeL (tRSi/X-iNNOVATiON)\n\n",VIEW_VER);
		FPuts(Output(),"A powerful Textviewer with archive-support for /X 3.18++ and FAME\n");
		Printf("\n%s\n\n",XYZ);
		PrintFault(IoErr(),NULL);
		return;
		};
	node=*(long *)ArgArray[0];
	FreeArgs(rda);
	Register(node);
	change(mainline,1,BB_MAINLINE);
	Decrypt(XYZ);
	glob=AllocVec(1024L,MEM_ATTRIBUTES);
  fullname=AllocVec(512L,MEM_ATTRIBUTES);
	fname=AllocVec(256L,MEM_ATTRIBUTES);
	pathbuf=AllocVec(1024L,MEM_ATTRIBUTES);
	t=AllocVec(200L,MEM_ATTRIBUTES);
  temppath=AllocVec(200L,MEM_ATTRIBUTES);
	checkbuf=AllocVec(200L,MEM_ATTRIBUTES);
  tempname=AllocVec(200L,MEM_ATTRIBUTES);
  myfib=AllocDosObject(DOS_FIB,NULL);
	if(!glob||!fullname||!fname||!pathbuf||!t||!temppath||!checkbuf||!tempname||!myfib) wb("\n\n\r[37mOUT OF MEMORY ERROR !\n\n\r");
	if(!(IconBase=OpenLibrary("icon.library",36L))) wb("\n\n\r[37mCan't open icon.library V36++ !\n\n\r");
	if(!(UtilityBase=OpenLibrary("utility.library",37L))) wb("\n\n\r[37mUnable to open utility.library V37++ !\n\n\r");
	process = (struct Process *) FindTask(NULL);
	temp = process->pr_WindowPtr;				// alten Pointer retten
	process->pr_WindowPtr = (APTR) -1;	// Requester Abschalten
	Write_Server("",FALSE);
	if(!(FileIDBase=OpenLibrary(FILEIDNAME,6L))) { SPrintf(glob,"[31mERROR: [36mCouldn't open %s V6++ !\n\n\r",FILEIDNAME);wb(glob);};
	if(checkformount("PIPE:")==FALSE) wb("\n\r[36mDevice [33mPIPE: [36mnot mounted !\n\n\r");
	initvars(_ProgramName);
	builtmain();
	parms=MakeArray(mainline);
	if(*parms)
  	{
		parms++;
		if(*parms)
			{
			strcpy(fname,*parms);
			parms++;
	    if(*parms)
  	    {
				if(!Stricmp(*parms,"NS")) ns=TRUE;
				else ns=FALSE;
				}
			}
		else getfilename();
    FreeArray(parms);
		}
	if(checkfile()==FALSE) goto enternew;
	executeit();
	enternew:
	ns=FALSE;
	getfilename();
	if(checkfile()==FALSE) goto enternew;
	executeit();
	goto enternew;
	ende:
	wb("\n\n\r[m");
	}

void executeit(void)
	{
	checkext();
	jumpext();
	}

void jumpext(void)
	{
	GetComment(fullname);
	switch(fid_num)
		{
		case FID_LHA:					archivlha(FID_LHA);
													break;
		case FID_DMS:					dmsinfo();
													break;
		case FID_ZIP:					zipinfo();
													break;
		case FID_ARJ:					arjinfo();
													break;
		case TXT:							showtext(fullname);
													break;
		case FID_AMIGAGUIDE:	showguide(fullname);
													break;
		case FID_UNKNOWNEXE:	exeinfo();
													break;
		case FID_SHRINK:			shrinkinfo();
													break;
		case FID_DRAWERICON:
		case FID_TOOLICON:
		case FID_PROJECTICON:	iconinfo();
													break;
		case FID_ZOO:					zooinfo();
													break;
		case FID_ZOOM:				zoominfo();
													break;
		case FID_WARP:				warpinfo();
													break;
		case FID_LZX:					archivlha(FID_LZX);
													break;
		default:							allother();
													break;
		}
	}

void showguide(char *text)
	{
	LONG	error;

	*glob='\0';
	if(!*guidepath||CheckProg(guidepath)==FALSE) { showtext(text); return;};
	SPrintf(glob,"[36mConverting [35m%s[36m to an ANSI-File[34m...",fname);
	inoutput(glob,3);
	SPrintf(t,"%s%s.%ld",temp_path,FilePart(fullname),slot); // Filename+TEMP_PATH
  *glob=*pathbuf='\0';
	SPrintf(pathbuf,"%lc%s%lc %lc%s%lc %lc%s%lc",34,guidepath,34,34,fullname,34,34,t,34);
	if(error=SystemTagList(pathbuf,NULL))
		{
		inoutput(fehler,3);
		if(extract==TRUE) { Loesche_File(fullname); extract=FALSE;};
		errors("Guide2DocProf",error);
		};
	inoutput(ALLRIGHT,3);
	if(extract==TRUE) { Loesche_File(fullname); extract=FALSE;};
	showtext(t);
	Loesche_File(t);
	}

void writeclog(char *file)
	{
	*glob='\0';
	SPrintf(glob,"\n\r[36mThe File [33m%s[36m is marked as [31mRESTRICTED [34m! \n\n\r",FilePart(file));
	inoutput(glob,3);
	if(comment==TRUE)
		{
		*glob='\0';
		inoutput("[mAdding comment to Callerslog[34m...",3);
		SPrintf(glob,"\tmEGA-vIEW:User tried to view %s\n",fullname);
		change(glob,0,BB_CALLERSLOG);
		inoutput(ALLRIGHT,3);
		};
	if(extract==TRUE) { Loesche_File(fullname); extract=FALSE;};
	wb(WEISS);
	}

void wb(char STR[])
	{
	inoutput(STR,3);
	inoutput(WEISS,3);
  Decrypt(XYZ);
	*glob='\0';
	SPrintf(glob,"\n\r%s\n\r",XYZ);
	inoutput(glob,3);
	process->pr_WindowPtr = temp;		// Alten Pointer setzen
	CloseMyPtr();
	ShutDown();
	exit(0);
	}

void CloseMyPtr(void)
	{
	if(glob) 				FreeVec(glob);
	if(fullname) 		FreeVec(fullname);
	if(fname) 			FreeVec(fname);
	if(pathbuf) 		FreeVec(pathbuf);
	if(t) 					FreeVec(t);
	if(tempname) 		FreeVec(tempname);
	if(checkbuf) 		FreeVec(checkbuf);
	if(temppath) 		FreeVec(temppath);
	if(myfib) 			FreeDosObject(DOS_FIB,myfib);
	if(Fib) 				free_elements(Fib);
	if(myicon) 			FreeDiskObject(myicon);
	if(IconBase) 		CloseLibrary(IconBase);
	if(UtilityBase) CloseLibrary(UtilityBase);
	if(FileIDBase) 	CloseLibrary(FileIDBase);
  }

void getfilename(void)
	{
	USHORT	i;
	char		**parms;

	Write_Server("",FALSE);
	*fullname=*fname=*glob='\0';
	extract=FALSE; *t='\0'; ns=FALSE;
	SPrintf(glob,"\n\r[40m[m[36mEnter Filename to %s [32m([33mEnter[32m) [36m= Quit [33m: ",vt[view]);
	inoutput(glob,3);
	inoutput("[0m[1 p",3);
	if(!sysflag) i=16;
	else i=119;
	*glob='\0';
	if(JH_PM(glob,i)<0) wb(MY_LOST_CARRIER);
	if(strlen(glob)<1) wb("\n\n\r");
	parms=MakeArray(glob);
	if(*parms)
		{
		strcpy(fname,*parms);
		parms++;
    if(*parms)
      {
			if(!Stricmp(*parms,"NS")) ns=TRUE;
			else ns=FALSE;
			}
		else ns=FALSE;
		FreeArray(parms);
		};
	inoutput("\n\r",3);
	}

void Write_Server(char *w,BOOL wat)
	{
	char	serv[50];

	if(wat==TRUE) change(w,0,WRITESERVER);
	else
		{
		SPrintf(serv,"mEGA-vIEW V%s",VIEW_VER);
		change(serv,0,WRITESERVER);
		}
	}

BOOL getfilelength(char *file)
	{
	BPTR	fh=NULL;
	long	success;

	if(!(fh=Open(file,MODE_OLDFILE))) return(FALSE);
	success=ExamineFH(fh,myfib);
	totalsize=myfib->fib_Size;
	Close(fh);
	if(success==DOSFALSE||totalsize<=0) return(FALSE);
	else return(TRUE);
	}

void builtmain(void)
	{
	*glob='\0';
	inoutput("\n\r",3);
	SPrintf(glob,"[35mmEGA-vIEW [mV%s [36mby SieGeL [m([36mtRSi[m/[36mX[m-[36miNNOVATiON[m)",VIEW_VER);
  center(glob,36);
	inoutput("\n\r",3);
	center("[m([32mUse [32m'[33m*[32m' as wildcard[m)",32);
	inoutput("\n\n\r",3);
	}

