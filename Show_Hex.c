//                      HEX-DUMP FOR mEGA-vIEW V2.++

#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "global_defines.h"
#include "struct_ex.h"
#include "proto.h"

void showhex(char *text);

void showhex(char *text)
	{
	UBYTE	ascbuf[18],buf[18],hex[4],zeilenpuffer[80];
	long	zaehler=NULL;
	long	readed,z,mypos;
	BPTR	fh=NULL;

	*glob='\0';
	if(view==FALSE) { inoutput(NO_VIEW,3);return;};
	if(!(fh=Open(text,MODE_OLDFILE))) {inoutput("[36mCouldn't found/open File !\n\n\r",3);return;};
	inoutput("\n\r[m",3);
	SPrintf(glob,"V %s",FilePart(text));
	Write_Server(glob,TRUE);
	actuallines=0;
	mypos=Seek(fh,0L,OFFSET_CURRENT);
	while(readed=Read(fh,buf,16))
		{
		*glob='\0';
		sprintf(glob,"%8.8X: ",zaehler);
    z=0;
    strcpy(zeilenpuffer,glob);
		while(z<readed)
			{
			if((int)buf[z]<32||(int)buf[z]>127) ascbuf[z]='.';
			else ascbuf[z]=buf[z];
 			sprintf(hex,"%2.2X",buf[z]);
			strcat(zeilenpuffer,hex);
			switch(z)
				{
				case 3:
				case 7:
				case 11:
				case 15:	strcat(zeilenpuffer," ");
									break;
				}
			z++;
			};
		ascbuf[z]='\0';
		if(readed<15) SPrintf(glob,"\r%s\r[46C%s\n\r",zeilenpuffer,ascbuf);
		else SPrintf(glob,"\r%s%s\n\r",zeilenpuffer,ascbuf);
		inoutput(glob,3);
		zaehler+=readed;*zeilenpuffer='\0';
		if(CTRL()==FALSE)
			{
			SPrintf(glob,"%lc\n\r[m***Break\n\r",(char) 15);
			inoutput(glob,3);
			break;
			};
		if(ns==FALSE)
			{
			actuallines++;
			switch(checklines(zaehler))
				{
				case -2:	Close(fh);
									SPrintf(glob,"%lcQuit ![m\n\n\r",(char) 15);
									if(extract==TRUE) { Loesche_File(text);extract=FALSE;};
									wb(glob);
				case -1:	Close(fh);
									SPrintf(glob,"%lc[36m\bNext File[34m...\n\r",(char) 15);
									inoutput(glob,3);
 									if(extract==TRUE) { Loesche_File(text);extract=FALSE;};
									return;
				case -3:  showhelp();
									Flush(fh);
									Seek(fh,mypos,OFFSET_BEGINNING);
									inoutput("\f[m",3);
									actuallines=0;
									zaehler=mypos;
									break;
				case 5:		mypos=Seek(fh,0L,OFFSET_CURRENT);
									break;
				};
			};
		};
	Close(fh);
	if(extract==TRUE) { Loesche_File(text);extract=FALSE;};
	SPrintf(glob,"%lc\n\r",(char) 15);
	inoutput(glob,3);
	}
