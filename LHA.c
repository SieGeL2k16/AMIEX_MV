#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include <time.h>
#include "global_defines.h"
#include "struct_ex.h"
#include "proto.h"

void lhatest(long);
void CutSpaces(char *);
void archivlha(long);

void CutSpaces(char *s)
	{
	char *d=s;

	while(*d==' ') *d++;
  while(*d) *s++=*d++;
	*s='\0';
	}

/*
 *  --- New for V2.13, supports now LHA & LZX Archives ---
 */

void archivlha(long ID)
	{
	char		**parms;
	struct	Liste *myptr;
	long		error,myret,mycount=NULL;
	BPTR		fh=NULL;
	BOOL		br=FALSE;

 	*glob=*pathbuf='\0';
	switch(ID)
		{
		case	FID_LHA:	if(*lhapath=='\0'||CheckProg(lhapath)==FALSE)
											{
											inoutput("\r[K\n\r[36mLHA-Handling not allowed !\n\n\r",3);
											return;
											};
										break;
		case	FID_LZX:	if(*lzxpath=='\0'||CheckProg(lzxpath)==FALSE)
											{
											inoutput("\r[K\n\r[36mLZX-Handling not allowed !\n\n\r",3);
											return;
											};
										break;
		}
	switch(Show_Archiv_Header(fname))
		{
		case -1:	return;
							break;
		case  1:	switch(ID)
								{
								case	FID_LHA:	lhatest(FID_LHA);
																return;
								case	FID_LZX:	lhatest(FID_LZX);
                                return;
								}
		}
	SPrintf(glob,"[36mReading contents from [35m%s[34m...",FilePart(fullname));
	inoutput(glob,3);
	switch(ID)
		{
		case	FID_LHA:	SPrintf(pathbuf,"%lc%s%lc >%s -N -r -Qw v %lc%s%lc",34,lhapath,34,TEMP,34,fullname,34);
                    break;
		case	FID_LZX:	SPrintf(pathbuf,"%lc%s%lc >%s l %lc%s%lc",34,lzxpath,34,TEMP,34,fullname,34);
                    break;
		}
	if(error=SystemTagList(pathbuf,NULL))
		{
		inoutput(fehler,3);
		Loesche_File(TEMP);
		errors("LHA/LZX",error);
		};
	if(!(fh=Open(TEMP,MODE_OLDFILE))) { inoutput(NO_TEMP,3);return;};
  Fib=NULL;
	Fib2=NULL;
	if(!(fib=AllocVec(sizeof(struct PackerList),MEMF_CLEAR|MEMF_PUBLIC))) wb(NO_MEM);
	while(FGets(fh,glob,199))
		{
		if(*glob!='-') continue;
		else break;
		};
  while(FGets(fh,glob,199))
		{
		if(*glob=='-') break;
		CutSpaces(glob);
		if(glob[0]==':') continue;
		glob[strlen(glob)-1]='\0';
    parms=NULL;
		parms=MakeArray(glob);
		if(*parms)
			{
			if(!(fib->Bytes=Atol(*parms)))
				{
				FreeArray(parms);
				continue;
				}
			else parms++;
			};
		if(*parms) parms++;
		if(*parms) parms++;
		if(*parms) 
			{ 
			if(*parms[0]=='%') parms++;
			if(!*parms) { FreeArray(parms);continue;}
			if(strlen(*parms)<9) { FreeArray(parms);continue;}
			strcpy(fib->Datum,*parms);
			parms++;
			}
		else { FreeArray(parms);continue;};
		if(*parms) { strcpy(fib->Uhrzeit,*parms);parms++;}
		if(*parms)
			{
			strncpy(fib->FullName,*parms,256);parms++;
			while(*parms)
				{
				strcat(fib->FullName," ");
				strcat(fib->FullName,*parms);
				parms++;
				};
			}
		else { FreeArray(parms);continue;};
		fib->Selected=' ';				// Nicht angewaehlt!
		if(StrStr(fib->FullName,"#")||StrStr(fib->FullName,"?")||StrStr(fib->FullName,"*")||StrStr(fib->FullName,"~")||StrStr(fib->FullName,"+")) {FreeArray(parms);continue;};
		strcpy(fib->FileName,FilePart(fib->FullName));
		AddToList();
		fib->Bytes=NULL;
    strcpy(fib->Datum,"");
		strcpy(fib->Uhrzeit,"");
		strcpy(fib->FileName,"");
		strcpy(fib->FullName,"");
		FreeArray(parms);
		};
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
				SPrintf(glob,"\f\n\r[36mNow processing file [33m: [32m%s[34m...",myptr->fib.FileName);
  			inoutput(glob,3);
				switch(ID)
					{
					case FID_LHA:	SPrintf(pathbuf,"%s -x2 -N -Qw -Qo e %lc%s%lc %lc%s%lc %s",lhapath,34,fullname,34,34,myptr->fib.FullName,34,temp_path);
												break;
					case FID_LZX:	SPrintf(pathbuf,"%s e %lc%s%lc %lc%s%lc %s",lzxpath,34,fullname,34,34,myptr->fib.FullName,34,temp_path);
												break;
					}
				if(error=SystemTagList(pathbuf,NULL))
        	{
					inoutput(fehler,3);
					myptr=myptr->next;
					continue;
					};
				SPrintf(fullname,"%s%s",temp_path,myptr->fib.FileName);
				strcpy(fname,myptr->fib.FileName);
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

void lhatest(long ID)
	{
	*pathbuf='\0';

	inoutput("\n\n\r[36mTesting archive-contents[34m...[m\n\r",3);
	switch(ID)
		{
		case  FID_LHA:	srand( (unsigned) time(NULL));
										SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld -D%ld t %lc%s%lc",34,lhapath,34,slot,zufall(0,3),34,fullname,34);
										break;
		case	FID_LZX:	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld t %lc%s%lc",34,lzxpath,34,slot,34,fullname,34);
										break;
    }
	pipecheck(pathbuf);
	}
