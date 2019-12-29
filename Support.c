#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "global_defines.h"
#include "struct_ex.h"
#include "proto.h"
#include <clib/utility_protos.h>
#include <proto/utility.h>

int 	PromptForName(void);
void 	errors(char *,long);					// Prg-Fail anzeigen
int 	Show_Archiv_Header(char *f);
BOOL 	AskBool(char *frage,BOOL wie);
void 	makeupper(char *s);
void 	Loesche_File(char *);
void 	Show_DIZ(void);

// RETURNCODES:
// ~~~~~~~~~~~~
// 0 	= NORMAL VIEW
// 1 	= NONSTOP VIEW
// 2 	= DOWNLOADING
// -1 = ABORT
// 9  = Lost Carrier !!

int PromptForName(void)
	{
	char	myptr[3];

	ns=FALSE;
	if(sec>=dllevel) dl=TRUE;
  else dl=FALSE;
	switch(dl)
		{
		case TRUE:	smp("[35mSelect[33m: [36mView Non[32m([33mS[32m)[36mtop[34m, [32m([33mA[32m)[36mbort[34m, [32m([33mD[32m)[36mownload[34m, [32m([33mEnter[32m)[34m=[36mView normal[33m:",0);
    	          break;
		case FALSE:	smp("[35mSelect[33m: [36mView Non[32m([33mS[32m)[36mtop[34m, [32m([33mA[32m)[36mbort[34m, [32m([33mEnter[32m)[34m=[36mView normal[33m:",0);
    	          break;
  	}
	*myptr='\0';
	if(JH_HK(myptr)<0) return(9);
	inoutput("\n\n\r",3);
	switch(ToUpper(*myptr))
		{
		case 'D':	if(dl==TRUE) return(2);
              break;
		case 'A': return(-1);
		case 'S': return(1);
    default:	return(0);
		}
	}

//FUNKTION: Erkanntes Archivformat anzeigen mit Abrage zum ansehen
//  INPUTS: *f = FileName
//  RETURN: -1 = QUIT, 0 = OK , 1 = TESTEN

int Show_Archiv_Header(char *f)
	{
	char	my[200];

	if(archives==FALSE) {inoutput("\r[K\r[36mThe Sysop decides to disable the Archive-Support !\n\r",3); return(-1);};
	SPrintf(my,"\r[K\r[36mFile [35m%s[36m identified as %s[34m...[36m",FilePart(f),fid_desc);
	inoutput(my,3);
	SPrintf(my,"%s it [32m([33mY[32m/[33mn[32m/[33mt[32m)[36m ? ",vt[view]);
	inoutput(my,3);
	*my='\0';
	if(JH_HK(my)<0) wb(MY_LOST_CARRIER);
	if(!(Stricmp(my,N))) { inoutput(NO,3); return(-1);};
	if(view==FALSE||(!(Stricmp(my,T)))) return(1);
	inoutput(YES,3);
	return(0);
	}

void errors(char *prg,long error)
	{
	*glob='\0';
	if(error==-1) SPrintf(glob,"\n\r[36mUnable to find [33m%s ![m\n\n\r",prg);
	else SPrintf(glob,"\n\r[33m%s [36mfailed with Returncode [m%ld\n\n\r",prg,error);
	wb(glob);
	}

BOOL AskBool(char *frage,BOOL wie)
	{
	char	s[200];
	BOOL	wat;

	inoutput(frage,3);
	if(wie==TRUE) inoutput(" [32m([33mY[32m/[33mn[32m) [36m? ",3);
	else inoutput(" [32m([33my[32m/[33mN[32m) [36m? ",3);
	*s='\0';
	if(JH_HK(s)<0) wb(MY_LOST_CARRIER);
	if(wie==TRUE)
		{
		if(!(Stricmp(s,"N"))) {inoutput(NO,3);wat=FALSE;}
		else {inoutput(YES,3);wat=TRUE;}
		}
	else
		{
		if(!(Stricmp(s,"Y"))) {inoutput(YES,3);wat=TRUE;}
		else {inoutput(NO,3);wat=FALSE;}
		}
	return(wat);
	}

void makeupper(char *s)
	{
	int	i=0;
	while(s[i]) { s[i]=ToUpper(s[i]); i++;}
	}

void Loesche_File(char *was)
	{
	SetProtection(was,FIBB_DELETE);
	DeleteFile(was);
	}

void Show_DIZ(void)
	{
	BPTR	fp=NULL;

	*glob='\0';
	if(!(fp=Open(TEMP,MODE_OLDFILE))) inoutput("[31mNot Found !\n\n\r",3);
	else
		{
		inoutput("[32mFound !\n\n\r",3);
		if(AskBool("[36m[24CView Description",TRUE)==TRUE)
			{
			inoutput("[m\n\n\r",3);
			while(FGets(fp,glob,800))
				{
				strcat(glob,"\r");
				inoutput(glob,3);
				};
			};
		Close(fp);
		inoutput("[m\n\r",3);
		Loesche_File(TEMP);
		}
	}
