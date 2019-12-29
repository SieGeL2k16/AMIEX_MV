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

void zooinfo(void)
	{
	long	error;

	*glob=*pathbuf='\0';
	if(!*zoopath||CheckProg(zoopath)==FALSE)
		{
		inoutput("\r[K\n\r[36mZOO-Handling not allowed !\n\n\r",3);
		return;
		};
	switch(Show_Archiv_Header(fname))
		{
		case -1:	return;
		case 1:		inoutput(TEST,3);
							SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld -test %lc%s%lc",34,zoopath,34,slot,34,fullname,34);
							pipecheck(pathbuf);
							return;
							break;
		}
	SPrintf(glob,"Showing contents from [35m%s[34m...",fname);
	inoutput(glob,3);
	SPrintf(pathbuf,"%lc%s%lc >%s V %lc%s%lc",34,zoopath,34,TEMP,34,fullname,34);
	if(error=SystemTagList(pathbuf,NULL))
		{
		inoutput(fehler,3);
		Loesche_File(TEMP);
		errors("ZOO",error);
		};
	inoutput(ALLRIGHT,3);
	JH_SF(TEMP);
	Loesche_File(TEMP);
	inoutput(ALLDONE,3);
	}
