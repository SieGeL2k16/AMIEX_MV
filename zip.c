#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "global_defines.h"
#include <clib/utility_protos.h>
#include <proto/utility.h>
#include "struct_ex.h"

#include "proto.h"

void zipinfo(void);
void ziptest(void);

STATIC char *baseline="------  ------   ----  -----   ----    ----   ------     ----";

void zipinfo(void)
	{
	char	**parms;
	long 	error,myret,mycount=NULL;
	BPTR	fh;
	struct	Liste *myptr;
	BOOL	br=FALSE;

	*glob=*pathbuf='\0';
	if(!*zippath||CheckProg(zippath)==FALSE)
		{
		inoutput("\n\n\r[36mZIP-Handling not allowed !\n\n\r",3);
		return;
		};
	switch(Show_Archiv_Header(fname))
		{
		case -1:	return;
							break;
		case 1:		ziptest();
							return;
							break;
		}
	SPrintf(glob,"[36mReading contents from [35m%s[34m...",FilePart(fname));
	inoutput(glob,3);
	SPrintf(pathbuf,"%lc%s%lc >%s -q -v %lc%s%lc",34,zippath,34,TEMP,34,fullname,34);
	if(error=SystemTagList(pathbuf,NULL))
		{
		inoutput(fehler,3);
		Loesche_File(TEMP);
		errors("UnZip",error);
		};
  if(!(fh=Open(TEMP,MODE_OLDFILE))) { inoutput(NO_TEMP,3);return;};
  Fib=NULL;
	Fib2=NULL;
	if(!(fib=AllocVec(sizeof(struct PackerList),MEMF_CLEAR|MEMF_PUBLIC))) {Close(fh);wb(NO_MEM);};
	while(FGets(fh,glob,199))	{	if(StrStr(glob,baseline)) break; };
  while(FGets(fh,glob,199))
		{
		if(StrStr(glob,baseline)) break;
		CutSpaces(glob);
		glob[strlen(glob)-1]='\0';
		parms=MakeArray(glob);
		if(*parms) { fib->Bytes=Atol(*parms);parms++;};
		if(*parms) parms++;
		if(*parms) parms++;
		if(*parms) parms++;
		if(*parms) { strcpy(fib->Datum,*parms);parms++;};
		if(*parms) { strcpy(fib->Uhrzeit,*parms);parms++;};
		if(*parms) parms++;
		if(*parms) { strncpy(fib->FullName,*parms,256); parms++;}
		else { FreeArray(parms);continue;};
		fib->Selected=' ';				// Nicht angewaehlt!
		if(fib->FullName[0]=='^') { fib->FullName[0]=' '; CutSpaces(fib->FullName);};
		strcpy(fib->FileName,FilePart(fib->FullName));
		AddToList();
		fib->Bytes=NULL;
    strcpy(fib->Datum,"");
		strcpy(fib->Uhrzeit,"");
		strcpy(fib->FileName,"");
		strcpy(fib->FullName,"");
		FreeArray(parms);
		}
  Close(fh);
	FreeVec(fib);
	inoutput(ALLRIGHT,3);
  Loesche_File(TEMP);
	DisplayThem();
	myptr=Fib;
	strcpy(temppath,fullname);			// Original-Fullname retten
	strcpy(tempname,fname);					// Original-Fname retten
	if(choosen)
		{
		while(myptr)
			{
			if(myptr->fib.Selected=='*')
				{
				mycount++;
				strcpy(fullname,temppath);
        if(pathmemcheck(temp_path,myptr->fib.Bytes)==FALSE)
					{
					SPrintf(glob,"\f\n\r[31mFile [32m%s [31mis too big to extract !\n\n\r",myptr->fib.FileName);
					inoutput(glob,3);
					Get_Key();
					myptr=myptr->next;
					continue;
					};
				SPrintf(glob,"\f\n\r[36mNow inflating file [33m: [32m%s[34m...",myptr->fib.FileName);
  			inoutput(glob,3);
				SPrintf(pathbuf,"%lc%s%lc -n -j %lc%s%lc %lc%s%lc -d %s",34,zippath,34,34,fullname,34,34,myptr->fib.FullName,34,temp_path);
				if(error=SystemTagList(pathbuf,NULL))
        	{
					inoutput(fehler,3);
					myptr=myptr->next;
					continue;
					};
				strcpy(fullname,temp_path);
				AddPart(fullname,myptr->fib.FileName,255);
				strcpy(fname,(FilePart(fullname)));
				if(getfilelength(fullname)==TRUE)
					{
					inoutput(ALLRIGHT,3);
					myret=PromptForName();
					if(myret<0) br=TRUE;
					else
						{
						switch(myret)
							{
							case 9:   free_elements(Fib);
												wb(MY_LOST_CARRIER);
							case 2: 	down();
												break;
							case 1:		ns=TRUE;
							default:	extract=TRUE;
  	      							executeit();

            	};
						Loesche_File(fullname);
						extract=FALSE;
						if(mycount!=choosen) Get_Key();
						}
					}
				else inoutput(fehler,3);
				}
			myptr=myptr->next;
      if(br==TRUE) { Loesche_File(fullname);extract=FALSE;break;};
			}
    }
	else inoutput("\f\n\r[36mNo files selected[34m...\n\r",3);
	free_elements(Fib);
	}

void ziptest(void)
	{
	inoutput("\n\n\r[36mTesting archive contents[34m...[m\n\r",3);
	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld -t %s",34,zippath,34,slot,fullname);
	pipecheck(pathbuf);
	inoutput(ALLDONE,3);
	return;
	}
