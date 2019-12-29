//Executable-Support for mEGA-vIEW V2.++

#include <stdlib.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <amiex/amiex_proto.h>
#include "global_defines.h"
#include "struct_ex.h"
#include "proto.h"

void exeinfo(void)
	{
	long 	error;

	if((!*exepath)&&(*exeid=='\0'))
		{
		inoutput("\n\n\r[36mExecutable-Handling not allowed !\n\n\r",3);
		return;
		};
	*glob=*pathbuf='\0';
	SPrintf(glob,"\r[K\r[36mFile [35m%s [36midentified as Executable[34m...[36mtest it",(FilePart(fname)));
	if(AskBool(glob,TRUE)==FALSE) return;
	inoutput("[m\n\r",3);
	if(*exepath!='\0'&&CheckProg(exepath)==TRUE)
		{
		SPrintf(pathbuf,"%lc%s%lc >PIPE:OUTPUT.%ld %lc%s%lc",34,exepath,34,slot,34,fullname,34);
		pipecheck(pathbuf);
		}
	if(gfc>1)
		{
		SPrintf(glob,"\n\r[36mPackertype [33m: [32m%s\n\r",fid_desc);
		inoutput(glob,3);
		}
	CheckForVersion(fullname);
	if(*exeid!='\0'&&CheckProg(exeid)==TRUE)
		{
		SPrintf(glob,"\n\n\r[36mNow checking for %lc[32mFileID_DIZ[36m%lc in [35m%s[34m...",34,34,(FilePart(fname)));
		inoutput(glob,3);
		SPrintf(pathbuf,"%lc%s%lc x %s %lc%s%lc",34,exeid,34,TEMP,34,fullname,34);
		if(error=SystemTagList(pathbuf,NULL))
			{
			Loesche_File(TEMP);
			inoutput(fehler,3);
			errors("ExeDescript",error);
			}
		Show_DIZ();
		}
	}
