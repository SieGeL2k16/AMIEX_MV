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
#include <clib/utility_protos.h>
#include <proto/utility.h>
#include "proto.h"

void down(void);

void down(void)
	{
	long	ratio,
				dlfiles,
				dlbytes,
				ulfiles,
				ulbytes,
				dailydld,
				dailylmt;

	*glob='\0';
	GetComment(fullname);
	change(glob,1,DT_RATIO);
	ratio=Atol(glob);
	change(glob,1,DT_BYTEDOWNLOAD);
	dlbytes=Atol(glob);
	change(glob,1,DT_DOWNLOADS);
	dlfiles=Atol(glob);
	change(glob,1,DT_BYTESUPLOAD);
	ulbytes=Atol(glob);
	change(glob,1,DT_UPLOADS);
	ulfiles=Atol(glob);
	change(glob,1,DT_DAILYBYTEDLD);
	dailydld=Atol(glob);
	change(glob,1,DT_DAILYBYTELIMIT);
	dailylmt=Atol(glob);
	SPrintf(glob,"[36mNumber of Downloads    [33m: [36m%ld [m([36m%ldk total[m)\n\r",dlfiles,(dlbytes/1024));
	inoutput(glob,3);
	SPrintf(glob,"[36mNumber of Uploads      [33m: [36m%ld [m([36m%ldk total[m)\n\r",ulfiles,(ulbytes/1024));
	inoutput(glob,3);
	SPrintf(glob,"[36mTodays Bytes available [33m: [36m%ld [36mBytes",(dailylmt-dailydld));
	inoutput(glob,3);
	if(!ratio) inoutput("  [33mDisabled Ratio[34m...",3);
	SPrintf(glob,"\n\n\r[36mZModem Download[33m: [35m%s [m([36m%ld Bytes[m)\n\n\r",FilePart(fullname),totalsize);
	inoutput(glob,3);
	inoutput("[36mOkay[33m: [32m([33mEnter[32m)[36m to Start Transfer, [32m([33mA[32m)[36mbort ? [m",3);
	*glob='\0';
	if(JH_HK(glob)<0) wb(MY_LOST_CARRIER);
	if(!(Stricmp(glob,"A"))) { inoutput("Abort !\n\r",3);return;}
	inoutput("[36mStart Transfer[34m...\n\r",3);
	if(ratio)
		{
		if((!ulbytes)||(dlbytes+totalsize)>ulbytes||(dailylmt-dailydld)<totalsize)
			{
			inoutput("\n\r[mExceeded Bytes Limit[34m...[31mABORTING !\n\n\r",3);
			return;
			}
		}
	if(Download(fullname)==1) inoutput("\r[1A[K\n\r[36mZModem-Transfer [32msuccessful[34m...\n\n\r",3);
	else
    {
		inoutput("\r[1A[K\n\r[36mZModem-Transfer [31munsuccessful [34m!!!\n\n\r",3);
		return;
		};
	dlfiles+=1L;
	dlbytes+=totalsize;
	SPrintf(glob,"[36mChanged your DL-Stat to [33m%ld [36mFile(s) with [33m%ld [36mBytes.\n\n\r",dlfiles,dlbytes);
	inoutput(glob,3);
	SPrintf(glob,"%ld",dlfiles);
	change(glob,0,DT_DOWNLOADS);
	SPrintf(glob,"%ld",dlbytes);
	change(glob,0,DT_BYTEDOWNLOAD);
	}
