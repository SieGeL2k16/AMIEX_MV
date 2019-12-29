/* $VER:MEGA-VIEW.PROTO 0.3 (18.12.95)
 *
 * Strongly reworked at 04.06.1996
 */


extern void		executeit(void);            // Abchecken/Ausfuehren des F-Types
extern void 	archivlha(long ID);					// LHA/LZX-Packersupport
extern void 	dmsinfo(void);							// DMS-Packersupport
extern void		zipinfo(void);							// ZIP-Packersupport
extern void		zooinfo(void);							// ZOO-Packersupport (!)
extern void		arjinfo(void);							// ARJ-Packersupport
extern void		exeinfo(void);							// EXE-Support
extern void		zoominfo(void);							// ZOOM-Packersupport (!)
extern void		warpinfo(void);							// WARP-Packersupport (!)
extern void		shrinkinfo(void);						// SHRINK-Packersupport
extern void		iconinfo(void);							// ICON-Support      (!)
extern void		allother(void);							// Alle unbekannten Formate
extern BOOL 	CTRL(void);									// CTRL-C (Break) Abfrage
extern void 	initvars(char *);						// Liest Icon aus
extern void 	errors(char *,long);				// Fehlermeldungen ausgeben
extern BOOL 	checkram(long);							// Auf Speicher pruefen
extern BOOL 	pathmemcheck(char *,long);	// Auf Platz pruefen
extern void 	pipecheck(char *);					// Archiv via Pipe: testen
extern BOOL 	checkformount(char *);			// Pruefe ob Device valid ist
extern void 	checkext(void);							// FileID checken
extern int 		checklines(int);						// Anzahl User-Lines
extern BOOL 	checkforfile(char *);				// Prueft Datei (Exist?)
extern void 	checkfortemp(char *);				// Temporaere Dateien loeschen
extern BOOL 	checkspecial(void);					// Auf Patterns checken
extern BOOL 	checkfile(void);						// Prueft File ab
extern void 	makeupper(char *s);					// Zeichenkette in UPPERCASE
extern BOOL 	readdir(void);							// Liest ein Directory ein (!)
extern void 	GetComment(char *);					// FileKommentar auslesen
extern void		down(void);									// Downloading
extern void		Loesche_File(char *);				// File loeschen+bits beachten
extern BOOL		CheckProg(char *);					// Prueft ob Datei existiert
extern void		showhex(char *);						// Hexanzeige eines Files
extern struct	DosLibrary *DOSBase;				// Fuer WildStar-Settings
extern char 	XYZ[80];										// Reg. String
extern char 	**MakeArray (char *s);			// Strings auswerten
extern void		FreeArray (char **strings);	// Strings Freigeben
extern int 		Show_Archiv_Header(char *);	// Standard-Archive-Meldung
extern void 	wb(char *);									// Programm beenden und alles free!
extern BOOL		AskBool(char *,BOOL);				// Standard Frage mit (y/n)
extern void		AddToList(void);						// Zu Liste adden
extern void		DisplayThem(void);					// Einträge anzeigen
extern void 	MakePrevPtr(void);					// *prev-Ptr initalisieren
extern void		Get_Key(void);							// Wartet auf Tastendruck
extern BOOL		getfilelength(char *);			// Liest Filelaenge aus
extern int		PromptForName(void);				// Wartet bei Archiven auf aktion
extern void		Write_Server(char *,BOOL);	// Schreibt Aktion in Server
extern void 	writeclog(char *);					// Meldung ins Callerslog schreiben
extern void		showhelp(void);							// Keyboard-Help
extern void		Show_DIZ(void);							// Zeigt FileID's an
extern void 	CutSpaces(char *s);					// Cuttet führende Spaces raus
extern void		CheckForVersion(char *);		// Prueft vorhandensein der Version
extern BOOL 	showtext(char *);						// Zeigt Text dem User an
extern void		free_elements(struct Liste *);
extern void 	DisplaySingleLine(struct Liste *,int,BOOL);
extern int 		archivepause(struct Liste *,struct Liste *,int);

// --- Protos defined in SieGeLAsm.o ---

extern STRPTR	__asm	StrChr(register __a0 STRPTR, register __d0 UBYTE);
extern LONG		__asm	StrMid(register __a0 STRPTR, register __a1 STRPTR,  register __d1 LONG, register __d0 LONG);
extern STRPTR	__asm	StrStr(register __a0 STRPTR, register __a1 STRPTR);
extern LONG 	__asm Atol(register __a0 STRPTR);     // Faster Than Light!
extern LONG __asm FSearch(register __a0 STRPTR, register __d0 BPTR);

// --- Protos defined in Decrypt.o ---

extern 	VOID 	__asm Decrypt(register __a0 STRPTR);
extern	ULONG __asm CheckRegCRC(register __d2 UWORD);

