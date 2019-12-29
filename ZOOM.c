#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include "struct_ex.h"
#include "global_defines.h"
#include "proto.h"

void zoominfo(void);
void warpinfo(void);

void zoominfo(void)
	{
	*glob=*pathbuf='\0';
	if(!*zoompath||CheckProg(zoompath)==FALSE)
		{
		inoutput("\r[K\r[36mZOOM-Handling not allowed !\n\n\r",3);
		return;
		};
	SPrintf(glob,"\r[K\r[36mFile [35m%s [36midentified as ZOOM-archive, test it",FilePart(fullname));
	if(AskBool(glob,TRUE)==FALSE) return;
	inoutput("\n\r[36mTesting archive integrity[34m...[m\n\r",3);
	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld %lc%s%lc",34,zoompath,34,slot,34,fullname,34);
	pipecheck(pathbuf);
	inoutput(ALLDONE,3);
	}

void warpinfo(void)
	{
	*glob=*pathbuf='\0';
	if(!*warppath||CheckProg(warppath)==FALSE)
		{
		inoutput("\r[K\r[36mWARP-Handling not allowed !\n\n\r",3);
		return;
		};
	SPrintf(glob,"\r[K\r[36mFile [35m%s [36midentified as WARP-archive, test it",FilePart(fullname));
	if(AskBool(glob,TRUE)==FALSE) return;
	inoutput("\n\r[36mTesting archive integrity[34m...[m\n\r",3);
	SPrintf(pathbuf,"run %lc%s%lc >PIPE:OUTPUT.%ld %lc%s%lc",34,warppath,34,slot,34,fullname,34);
	pipecheck(pathbuf);
	inoutput(ALLDONE,3);
	}
