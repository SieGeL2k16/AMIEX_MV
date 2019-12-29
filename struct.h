// Alle internen Variablen

#include "proto.h"

struct DiskObject *myicon=NULL;
struct FileInfoBlock *myfib=NULL;

char 	*NO_ICN ="\n\r[31mERROR: [36mmEGA-vIEW needs an icon to run !\n\n\r",
			*NO_MEM ="\n\r[31mNO MEMORY !\n\n\r",
			*NOICON ="\nmEGA-vIEW needs icon.library >=36 (Kickstart 2.++!) to run...\n\n",
			*NO_TEMP="[31mERROR !\n\n\rCan't find temporary file!\n\n\r",
      *NO_VIEW="\n\r[mSorry, but you have no access to view files !\n\r",
			*NO_FILE="\n\rFile not found !\n\r",
			*fehler ="[31mFAIL !\n\n\r",
			*MY_LOST_CARRIER="\n\n\r[31mLOST THE CARRIER, ABORTING ![m\n\n\r",
			*ALLRIGHT="[32mOK !\n\n\r[m",
			*ALLDONE="\n\r[36mAll Done ![m\n\n\r",
			*KILL_LINE="\r[K\r[m";

UWORD	fid_num;
UWORD	gfc;
char	fid_desc[50],
			TEMP[50];

char	*glob=NULL,												// Temp-Pointer fuer Ausgaben
			*pathbuf=NULL;

/*--- Variablen fuer die Viewfunktionen ---*/

int	mlines=NULL,		/* Max. Anzahl Zeilen des Users */
		slot=NULL,			/* Slotnummer des Users	*/
		sec=NULL,				/* AccessLevel des Users */
		dllevel=NULL,		/* Level zum Saugen von Files */
		actuallines=0;	/* Bisher angezeigte Zeilen (0=Default) */

/*--- BOOL-Variablen fuer ToolType-Werte usw. ---*/

BOOL	sysflag=FALSE,	ns=FALSE,
			special=FALSE,	comment=FALSE,
			view=FALSE,			extract=FALSE,
			dl=FALSE,       archives=TRUE;

/*--- Alle Chars fuer die ToolTypes ---*/

char	*lhapath=NULL, 		*dmspath=NULL,
			*exepath=NULL, 		*zippath=NULL,
			*arjpath=NULL, 		*guidepath=NULL,
			*dmsid=NULL,    	*exeid=NULL,
			*shrinkpath=NULL,	*zoopath=NULL,
			*zoompath=NULL,		*warppath=NULL,
			*lzxpath=NULL,    temp_path[200],
			*checkbuf=NULL,		*temppath=NULL,
			*tempname=NULL;


long	totalsize=0L,		// Lange des angezeigten Files (bytes)
			choosen=NULL,		// Anzahl ausgewaehlter Elemente
			node=NULL;			// NodeNummer

char 	*fname=NULL,		// FileName
			*fullname=NULL,	// Filename + Pfadangabe
			*t=NULL,				// Puffer fuer Filename aus Archiv
			vt[2][10];			// Puffer fuer 'VIEW' & 'TEST'

struct	PackerList
	{
	char  FullName[512],
				FileName[50],
				Datum[12],
				Uhrzeit[10];
	long	Bytes;
	char	Selected;
	}packerlist;

struct Liste
	{
	struct 	PackerList fib;					// Zeiger auf meine Liste
	struct 	Liste *prev;						// Ptr auf vorhergehendes Element
	struct 	Liste *next;		       	// Ptr auf naechstes Element
	}*Fib,*Fib2;

struct	PackerList *fib;
