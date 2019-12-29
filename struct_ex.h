// Alle internen Variablen (als EXTERN)

#include "proto.h"

extern struct Library 		*FileIDBase;
extern struct Library 		*IconBase;
extern struct Library			*UtilityBase;
extern struct DiskObject 	*myicon;
extern struct	FileInfoBlock *myfib;

extern char	*NO_ICN,
						*NO_MEM,
						*NOICON,
						*NO_TEMP,
						*NO_VIEW,
						*NO_FILE,
						*fehler,
						*glob,												// Temp-Pointer fuer Ausgaben
            *pathbuf,                     // 1K Buffer
						*MY_LOST_CARRIER,
	     			*ALLRIGHT,
						*ALLDONE,
						*KILL_LINE;


extern UWORD	fid_num;
extern UWORD	gfc;
extern char		fid_desc[50],
							TEMP[50];

//--- Variablen fuer die Viewfunktionen ---

extern int	mlines,					// Max. Anzahl Zeilen des Users
						slot,						// Slotnummer des Users
						sec,						// AccessLevel des Users
						dllevel,				// Level zum Saugen von Files
						actuallines;		// Bisher angezeigte Zeilen (0=Default)

//--- BOOL-Variablen fuer ToolType-Werte usw. ---

extern BOOL	sysflag,	ns,
						special,	comment,
						view,			extract,
						dl,				archives;

//--- Alle Chars fuer die ToolTypes ---

extern char	*lhapath, 			*dmspath,
						*exepath, 			*zippath,
						*arjpath, 			*guidepath,
						*dmsid,         *exeid,
						*shrinkpath,		*zoopath,
						*zoompath,			*warppath,
						*lzxpath,       temp_path[200],
						*checkbuf,			*temppath,			
						*tempname;


extern long	totalsize,			// Lange des angezeigten Files (bytes)
						choosen,				// Anzahl ausgewaehlter Elemente
						node;						// NodeNummer

extern char *fname;					// FileName
extern char	*fullname;			// Filename + Pfadangabe
extern char	*t;							// Puffer fuer Filename aus Archiv
extern char	vt[2][10];			// Puffer fuer 'VIEW' & 'TEST'

extern struct	PackerList
	{
	char  FullName[512],
				FileName[50],
				Datum[12],
				Uhrzeit[10];
	long	Bytes;
	char	Selected;
	}packerlist;

extern struct	PackerList *fib;

extern struct Liste
	{
	struct PackerList fib;		// Zeiger auf meine Liste
	struct Liste *prev;				// Prev-Pointer auf vorgehendes Element
	struct Liste *next;       // Next-Pointer auf naechstes Element
	}*Fib,*Fib2;
