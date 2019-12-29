#include <stdio.h>
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

void dmsinfo(void);

void dmsinfo(void)
	{
	long	error;

	*glob='\0';
	if((!*dmspath)&&(!*dmsid))
		{
		inoutput("\r[K\n\r[36mDMS-Handling not allowed !\n\n\r",3);
		return;
		};
	switch(Show_Archiv_Header(fname))
		{
		case -1:	return;
							break;
		case  1:	inoutput(TEST,3);
							SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld TEST %lc%s%lc",34,dmspath,34,slot,34,fullname,34);
							pipecheck(pathbuf);
							return;
							break;
		}
	if(*dmspath!='\0'&&view==TRUE&&CheckProg(dmspath)==TRUE)
		{
		SPrintf(glob,"\n\r[36mGettin' information for [35m%s[34m...\n\n\r[0m[38m",(FilePart(fullname)));
		inoutput(glob,3);
		SPrintf(pathbuf,"%lc%s%lc >PIPE:OUTPUT.%ld VIEW %lc%s%lc",34,dmspath,34,slot,34,fullname,34);
		pipecheck(pathbuf);
		inoutput(ALLDONE,3);
		}
	if(*dmsid!='\0'&&CheckProg(dmsid)==TRUE)
		{
		SPrintf(glob,"[36mNow checking for %lc[32mFileID_DIZ[36m%lc in [35m%s[34m...",34,34,(FilePart(fullname)));
		inoutput(glob,3);
		SPrintf(pathbuf,"%lc%s%lc x %s %lc%s%lc",34,dmsid,34,TEMP,34,fullname,34);
		if(error=SystemTagList(pathbuf,NULL))
			{
			Loesche_File(TEMP);
			inoutput(fehler,3);
			errors("DMSDescript",error);
			}
		Show_DIZ();
		}
	}
