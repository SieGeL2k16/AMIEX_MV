/*
 *  --- ShowText.c - Displays finally the Textfile ---
 */
#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "proto.h"
#include "global_defines.h"
#include "struct_ex.h"
#include <clib/utility_protos.h>
#include <proto/utility.h>

BOOL showtext(char *);
int 	checklines(int);
void	showhelp(void);								// Keyboard-Help
BOOL zippysearch(BPTR,LONG);				// Sucht im text nach strings

BOOL showtext(char *text)
	{
  BPTR	fp;
	long	fsize=0L,
				zeichen,
				READBUFFER=NULL;
	char  *s;									// Buffer zum lesen, 'READBUFFER' Bytes gross
	long	mypos;

	inoutput(WEISS,3);
	*glob='\0';
  SPrintf(glob,"V %s",FilePart(text));
	Write_Server(glob,TRUE);
  switch(CheckAnsiFlag())
		{
		case	0:  READBUFFER=1010L;
            	break;
		case 	1:  READBUFFER=5010L;
							break;
    }
	actuallines=0;
  *glob='\0';
  if(view==FALSE)
		{
		inoutput(NO_VIEW,3);
		return(FALSE);
		};
	if(!(s=AllocVec(READBUFFER,MEMF_PUBLIC|MEMF_CLEAR))) wb(NO_MEM);
  if(!(fp=Open(text,MODE_OLDFILE)))
		{
		SPrintf(glob,"\n\r[36mFile [35m%s [36mnot found !\n\n\r",FilePart(text));
		inoutput(glob,3);
		FreeVec(s);
		return(FALSE);
		};
	change(glob,1,BB_NONSTOPTEXT);
	mypos=Seek(fp,0L,OFFSET_CURRENT);
	while(1)
		{
		*glob='\0';
		inoutput("[m",3);
		if(CTRL()==FALSE)
			{
			SPrintf(glob,"%lc\n\r%s**Break\n\r",(char) 15,WEISS);
			inoutput(glob,3);
			break;
			};
		if(!FGets(fp,s,READBUFFER-10L)) break;
		zeichen=strlen(s);
		fsize+=zeichen;
		strcat(s,"\r\0");
		smp(s,0);
		CutAnsi(s);
		zeichen=strlen(s)-2;
		if(zeichen>=81) actuallines++;
		actuallines++;
    if (ns==FALSE)
			{
			switch(checklines(fsize))
				{
				case -2:	Close(fp);
                  FreeVec(s);
									SPrintf(glob,"%lc[31mQuit !%s\n\n\r",(char) 15,WEISS);
									checkfortemp(t);
									wb(glob);
				case -1:	Close(fp);
									inoutput("\bNext File !\n\r",3);
									goto endofshow;
									break;
				case -3:  showhelp();
									Flush(fp);
									Seek(fp,mypos,OFFSET_BEGINNING);
									inoutput("\f\n\r[0m[38m",3);
                  actuallines=0;
									fsize=mypos;
									break;
				case 4:   inoutput(KILL_LINE,3);
                  if(zippysearch(fp,mypos)==FALSE) { Close(fp);	goto endofshow;}
                  actuallines=0;
									mypos=Seek(fp,0L,OFFSET_CURRENT);
									fsize=mypos;
									break;
				case 5:		mypos=Seek(fp,0L,OFFSET_CURRENT);
									break;
       	}
			}
    }
	Close(fp);
	endofshow:
	FreeVec(s);
	checkfortemp(t);
	*glob='\0';
	SPrintf(glob,"%lc%s\n\r",(char) 15,WEISS);
	inoutput(glob,3);
	actuallines=0;
	return(TRUE);
	}

BOOL zippysearch(BPTR myfile,long origpos)
	{
  char	zipstr[200],mysearch[60],searching[60];
	long	foundpos,seekcounter=0L;
	int		i=0;
  BOOL	zip=FALSE;

  inoutput("[36mEnter Searchstring[33m: ",3);
	*mysearch='\0';
	if(JH_PM(mysearch,55)<0) return(FALSE);
	if(strlen(mysearch)<1) { inoutput("[1A\r",3);return(TRUE);};
	inoutput("\r[1A[K\r[36mSearch from [32m([33mB[32m)[36meginning, [32m([33mC[32m)[36murrent position or [32m([33mA[32m)[36mbort searching ?",3);
	*zipstr='\0';
	strcpy(searching,mysearch);
	makeupper(searching);
	if(JH_HK(zipstr)<0) return(FALSE);
	switch(ToUpper(*zipstr))
		{
		case	'B':		Seek(myfile,0L,OFFSET_BEGINNING);
                  break;
		case  'C':    Seek(myfile,origpos,OFFSET_BEGINNING);
                  break;
		case	'A':		return(TRUE);
    }
	inoutput("\f\n\r[m",3);
	*zipstr='\0';
	foundpos=FSearch(searching,myfile);
	if(foundpos==-1)
		{
   	SPrintf(zipstr,"[32m'[33m%s[32m' [36mnot found !",mysearch);
    center(zipstr,36);
    Get_Key();
		Seek(myfile,origpos,OFFSET_BEGINNING);
		inoutput("\f\n\r[m",3);
    }
	else
		{
		Seek(myfile,-1L,OFFSET_CURRENT);
		seekcounter++;
		while(zip==FALSE)
			{
	    if(!IoErr())
  			{
				while(1)
	      	{
					i=FGetC(myfile);
					if(i==10||i==13) { seekcounter--;zip=TRUE;break;}
					Seek(myfile,-2L,OFFSET_CURRENT);
					if(IoErr()) { zip=TRUE;break;};
					seekcounter++;
					}
      	}
			else break;
			}
		Flush(myfile);
		*zipstr='\0';
		if(seekcounter)
			{
			Read(myfile,zipstr,seekcounter);
			zipstr[seekcounter]='\0';
			inoutput(zipstr,3);
			}
		inoutput("[32m",3);
		*zipstr='\0';
		Read(myfile,zipstr,strlen(mysearch));
		zipstr[strlen(mysearch)]='\0';
		inoutput(zipstr,3);
		inoutput("[m",3);
		}
	return(TRUE);
	}

int checklines(int size)
	{
	char	r[3],mystr[200];
	long	prozent;

 	if (actuallines>=mlines-2)
		{
		actuallines=0;
		prozent=(size*100)/totalsize;
		SPrintf(mystr,"%s%lc[36mFile:[33m%s[36m, [33m%ld[32m/[33m%ld [36mBytes [32m([33m%2.2ld [36m%%[32m) [32m",KILL_LINE,(char) 15,FilePart(fullname),size,totalsize,prozent);
		inoutput(mystr,3);
		inoutput("- [33mC[36m,[33mN[36m,[33mNS[36m,[33mQ[36m,[33mZ[36m,[33m?[36m,[33mCR [32m>:[m ",3);
		*r='\0';
		if(JH_HK(r)<0) wb(MY_LOST_CARRIER);
		switch(ToUpper(*r))
			{
			case 'C':	inoutput("\f\n\r[m",3);
								return(5);
			case 'Q':	return(-2);
			case 'N':	inoutput(r,3);
								r[0]='\0';
								if(JH_HK(r)<0) wb(MY_LOST_CARRIER);
								switch(ToUpper(*r))
									{
									case 'S':		ns=TRUE;
															inoutput(KILL_LINE,3);
															break;
									case '\n':
									case '\r':  actuallines=0;
															ns=FALSE;
															return(-1);
	                };
								break;
			case 'Z':	return(4);
			case '?': return(-3);
			default:	inoutput(KILL_LINE,3);
								return(5);
      }
    }
	return(0);
	}

void showhelp(void)
	{
  inoutput("\f\n\r[23C[35mmEGA-vIEW [34m- [36mKeyboard-Help\n\r[23C[m~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\r",3);
	inoutput("[4C[35m[[33mC[35m] [34m= [36mClear Screen    [35m[[33mQ[35m] [34m= [36mQuit mEGA-vIEW     [35m[[33mNS[35m] [34m= [36mView NonStop\n\r",3);
	inoutput("[4C[35m[[33mN[35m] [34m= [36mNext File       [35m[[33mZ[35m] [34m= [36mZippy Search       [35m[[33mCR[35m] [34m= [36mContinue Viewing\n\n\r",3);
	Get_Key();
	}
