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

BOOL CheckProg(char *);		// Prueft auf Vorhandensein des Programs
void initvars(char *);		// Variablen initalisieren

void initvars(char *icon)
	{
	char	*entry;

	if(!(myicon=GetDiskObject(icon))) wb(NO_ICN);
	strcpy(vt[1],"view");
	strcpy(vt[0],"test");
	*glob='\0';
	change(glob,1,DT_LINELENGTH);
	mlines=Atol(glob);
	change(glob,1,DT_SECSTATUS);
	sec=Atol(glob);
	arjpath   = FindToolType(myicon->do_ToolTypes,"ARJ_PATH");
	lhapath   = FindToolType(myicon->do_ToolTypes,"LHA_PATH");
	lzxpath   = FindToolType(myicon->do_ToolTypes,"LZX_PATH");
	dmspath   = FindToolType(myicon->do_ToolTypes,"DMS_PATH");
	zippath   = FindToolType(myicon->do_ToolTypes,"ZIP_PATH");
	exepath   = FindToolType(myicon->do_ToolTypes,"EXE_PATH");
	zoopath   = FindToolType(myicon->do_ToolTypes,"ZOO_PATH");
	zoompath  = FindToolType(myicon->do_ToolTypes,"ZOOM_PATH");
	warppath  = FindToolType(myicon->do_ToolTypes,"WARP_PATH");
	guidepath = FindToolType(myicon->do_ToolTypes,"GUIDE_PATH");
	shrinkpath= FindToolType(myicon->do_ToolTypes,"SHRINK_PATH");
	dmsid     = FindToolType(myicon->do_ToolTypes,"DMS_FILEID");
	exeid     = FindToolType(myicon->do_ToolTypes,"EXE_FILEID");
	if(!(entry= FindToolType(myicon->do_ToolTypes,"SYSOPVIEWLEVEL"))) sysflag=FALSE;
	else
		{
		if(sec>=Atol(entry)) sysflag=TRUE;
		else sysflag=FALSE;
		}
	if(!(entry= FindToolType(myicon->do_ToolTypes,"ONLY_CHECK"))) view=FALSE;
	else
		{
		if(sec>=Atol(entry)) view=TRUE;
		else view=FALSE;
		}
	if(!(entry=FindToolType(myicon->do_ToolTypes,"TEMP_PATH"))) *temp_path='\0';
	else strncpy(temp_path,entry,198);
	if(!(entry=FindToolType(myicon->do_ToolTypes,"DL_LEVEL")))  dllevel=0L;
	else dllevel=Atol(entry);
	if(!(entry=FindToolType(myicon->do_ToolTypes,"CALLERSLOG"))) comment=FALSE;
	else
		{
		if(MatchToolValue(entry,"YES")==TRUE) comment=TRUE;
		else comment=FALSE;
		}
	if(!(entry=FindToolType(myicon->do_ToolTypes,"ALLOW_ARCHIVES"))) archives=TRUE;
	else
		{
		if(MatchToolValue(entry,"YES")==TRUE) archives=TRUE;
		else archives=FALSE;
		}
	if(!temp_path) strcpy(temp_path,"RAM:");
	*glob='\0';
	change(glob,1,DT_SLOTNUMBER);
	slot=Atol(glob);
	SPrintf(TEMP,"T:MeGa-VieW.@%ld",slot);
	if(sysflag==TRUE) view=TRUE;
	}

BOOL CheckProg(char *was)
	{
	BPTR	lo;

	if(!(lo=Lock(was,ACCESS_READ))) return(FALSE);
	else UnLock(lo);
	return(TRUE);
	}
