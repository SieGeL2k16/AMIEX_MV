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

void shrinkinfo(void);
void shrinktest(void);

void shrinkinfo(void)
	{
	*glob=*pathbuf='\0';
	if(!*shrinkpath||CheckProg(shrinkpath)==FALSE)
		{
		inoutput("\n\n\r[36mShrink-Handling not allowed !\n\n\r",3);
		return;
		};
	switch(Show_Archiv_Header(fname))
		{
		case -1:	return;
							break;
		case 1:		shrinktest();
							return;
							break;
		}
	if(view==FALSE) { shrinktest(); return;}
	SPrintf(glob,"[36mList Contents from [35m%s[34m...\n\r",FilePart(fullname));
	inoutput(glob,3);
	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld l %lc%s%lc",34,shrinkpath,34,slot,34,fullname,34);
	pipecheck(pathbuf);
	inoutput(ALLDONE,3);
	}

void shrinktest(void)
	{
	*pathbuf='\0';
	inoutput("\n\n\r[36mTesting archive contents[34m...[m",3);
	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld t %lc%s%lc",34,shrinkpath,34,slot,34,fullname,34);
	pipecheck(pathbuf);
	}
