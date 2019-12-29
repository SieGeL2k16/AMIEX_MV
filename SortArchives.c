// $VER: SortArchiveContents 0.02 (07.06.95)

#include <stdlib.h>
#include <strings.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <amiex/amiex_proto.h>
#include <ctype.h>
#include <clib/utility_protos.h>
#include <proto/utility.h>
#include "struct_ex.h"
#include "global_defines.h"
#include "proto.h"

#define   VORTEXT     "[36mLeft[32m([33m4[32m)[34m, "
#define		WEITERTEXT  "[36mRight[32m([33m6[32m)[34m, "
#define   TEXT_1      "\n\r[35mChoose[33m: [36mUp[32m([33m8[32m) [34m/ [36mDown[32m([33m2[32m)[34m, "
#define		TEXT_2      "[32m([33mSpace[32m)[34m, [32m([33m?[32m)[34m, [32m([33mQ[32m/[33mCR[32m)"
#define		LIST_FLAGS	"\f\rList of flagged Files\n\n\r"


long  pagecount=NULL,							// Anzahl gesamter Eintraege (Page-counting)
			pages,											// Anzahl benoetigter Seiten :)
			actpage;                    // Gerade dargestellte Seite

struct Liste *merge_start,*merge_end;

void 					free_elements (struct Liste *l);
struct Liste 	*quicksort (struct Liste *l);
struct Liste 	*merge (struct Liste *l1,struct Liste *l2);
int 					sorting (struct Liste *l1,struct Liste *l2);
void 					DisplayOnlineHelp(BOOL v,BOOL b);
void					Get_Key(void);

void DisplayThem(void)
	{
	int			actuallines,i;
	struct	Liste *lastentry,*firstentry,*mylist;
  BOOL		br=FALSE;
  char		listhead[200];

	*temppath='\0';
	SPrintf(listhead,"\f\n\r[36mListing of archive [32m%s[34m\n\n\r",FilePart(fullname));
	inoutput(listhead,3);
	MakePrevPtr();
	pages=1L;
	while(pagecount>mlines-4)
		{
		pagecount-=(mlines-4);
		if(pagecount>0) pages++;
		if(pagecount<=0) {pages--;break;};
		};
	actuallines=0;
	firstentry=mylist=Fib2;
	actpage=1L;
	while(mylist)
		{
		DisplaySingleLine(mylist,34,FALSE);
		inoutput("\n\r",3);
		lastentry=mylist;
		mylist=mylist->next;
		actuallines++;
    if(actuallines>mlines-5||!mylist)
			{
			switch(archivepause(firstentry,lastentry,actuallines-1))
				{
				case -1:	free_elements(Fib);
									wb(MY_LOST_CARRIER);
        case -2:  inoutput(listhead,3);
									mylist=firstentry;
                  actuallines=0;
									continue;
    		case 1:		break;				// CRSR Right, also weiter anzeigen

				case 2:   for(i=0;i!=mlines-4;i++)
										{
										if(firstentry->prev) firstentry=firstentry->prev;
										else break;
                    }
									mylist=firstentry;
									break;
				case 0:		br=TRUE;
									break;
    		};
			if(br==FALSE)
				{
				if(mylist) inoutput(listhead,3);
				else { SPrintf(temppath,"[H[%ldB\r[K\r",actuallines+4);inoutput(temppath,3);break;}
				};
			firstentry=mylist;
			actuallines=0;
			};
		if(br==TRUE) break;
		};
	}

// *start   = Ptr. auf das erste Element dieser Seite
// *end     = Ptr. auf das letzte Element dieser Seite
// lastline = Zeile des letzten Elements

int archivepause(struct Liste *start,struct Liste *end,int lastline)
	{
	struct	Liste *zwi;
	BOOL		br=FALSE,vor=FALSE,back=FALSE;
	long		mylength;

  *glob='\0';
	inoutput(TEXT_1,3);
	if(start->prev) {inoutput(VORTEXT,3);		vor=TRUE;};
	if(end->next) 	{inoutput(WEITERTEXT,3);back=TRUE;};
	inoutput(TEXT_2,3);
	SPrintf(glob,"Page: %ld/%ld",actpage,pages);
	mylength=78-(strlen(glob));
	SPrintf(glob,"\r[%ldC[36mPage[33m: [32m%ld[34m/[32m%ld",mylength,actpage,pages);
	inoutput(glob,3);
  *glob='\0';
	inoutput("[H\n\n\n\r",3);
	change("",0,RAWARROW);
  zwi=start;
	do
		{
		DisplaySingleLine(zwi,33,TRUE);
		glob[0]='\0';
		glob[0]=Fhotkey();
		switch((int) glob[0])
			{
      case ENTER:
			case 27:
      case 113:
			case 81:					br=TRUE;
												break;

			case	63:         change("",0,RAWARROW);
												DisplayOnlineHelp(vor,back);
												return(-2);				// DocFile anzeigen!
			case	50:
			case	CRSR_DOWN:  DisplaySingleLine(zwi,34,FALSE);
												if(zwi==end||!zwi)
													{
													zwi=start;
													inoutput("[H\n\n\n\r",3);
                          }
												else
													{
													zwi=zwi->next;
													inoutput("\n\r",3);
													}
												break;
			case  56:
			case	CRSR_UP:		DisplaySingleLine(zwi,34,FALSE);
												if(zwi==start||!zwi)
													{
													zwi=end;
													SPrintf(glob,"[%ldB",lastline);
													inoutput(glob,3);
													}
												else
                          {
													zwi=zwi->prev;
													inoutput("[1A",3);
													}
                        break;
      case	54:
			case 	CRSR_LEFT:	if(back==TRUE) {change("",0,RAWARROW);actpage++;return(1);};
												break;
      case	52:
			case	CRSR_RIGHT:	if(vor==TRUE) { change("",0,RAWARROW);actpage--;return(2);};
                        break;

			case 32:  				DisplaySingleLine(zwi,34,FALSE);
												if(zwi->fib.Selected==' ') { zwi->fib.Selected='*';choosen++;}
												else { zwi->fib.Selected=' ';choosen--;};
												break;

   		case CRSR_LOST:		change("",0,RAWARROW);
                      	return(-1);
												break;

			};
		}while(br==FALSE);
	change("",0,RAWARROW);
	return(0);
  }

void DisplayOnlineHelp(BOOL v,BOOL b)
	{
	static int i;
	static char *hilfe[7]="\f\n\r[26C[36mHelp on Key-Commands\n\r[m[26C~~~~~~~~~~~~~~~~~~~~\n\n\r",
  											"[20C[32mCRSR Up/Down [34m= [33mChoose Entries\n\r",
												"[20C[32mCRSR Left    [34m= [33mJump to previous Page\n\r",
												"[20C[32mCRSR Right   [34m= [33mJump to next Page\n\r",
												"[20C[32m  SPACE      [34m= [33mSelect/Deselect an Entry\n\r",
												"[20C[32m Q/ENTER     [34m= [33mExit and start extracting\n\r",
												"[20C[32m    ?        [34m= [33mThis Command-Help\n\r";

	for(i=0;i<7;i++) inoutput(hilfe[i],3);
	Get_Key();
	}

void DisplaySingleLine(struct Liste *which,int color,BOOL wie)
	{
	*tempname='\0';
	switch(wie)
		{
		case FALSE:	SPrintf(tempname,"\r  [%ldm%-30.30s %7ld %9s %8s[35m%lc\r",color,FilePart(which->fib.FileName),which->fib.Bytes,which->fib.Datum,which->fib.Uhrzeit,which->fib.Selected);
								break;
		case TRUE:	SPrintf(tempname,"\r [35m>[%ldm%-30.30s %7ld %9s %8s[35m%lc\r",color,FilePart(which->fib.FileName),which->fib.Bytes,which->fib.Datum,which->fib.Uhrzeit,which->fib.Selected);
								break;
		};
	inoutput(tempname,3);
	}

int sorting (struct Liste *l1,struct Liste *l2)
	{
	int 	i,erg;

  if (!l1)  return(0);
  if (!l2)  return(1);
  i=0;
  erg=2;
  do
		{
		if (ToLower (l1->fib.FileName [i]) < ToLower (l2->fib.FileName [i]))
    		erg = 1 ;
    if (ToLower (l1->fib.FileName [i]) > ToLower (l2->fib.FileName [i]))
     		erg = 0 ;
    if (l1->fib.FileName [i] == '\0')
      	erg = 1 ;
    if (l2->fib.FileName [i] == '\0')
     		erg = 0 ;
    i++;
  	}while(erg==2);
  return (erg) ;
	}

struct Liste *merge (struct Liste *l1,struct Liste *l2)
	{
  merge_end = NULL ;
  do
		{
		if (sorting (l1,l2))
    	{
			if (merge_end == NULL)
      	{
				merge_end = l1 ;
        merge_start = merge_end ;
      	}
      else
      	{
				merge_start->next = l1 ;
        merge_start = merge_start->next ;
      	}
      l1 = l1->next ;
    	}
    else
    	{
			if (merge_end == NULL)
      	{
				merge_end = l2 ;
        merge_start = merge_end ;
      	}
      else
      	{
				merge_start->next = l2 ;
        merge_start = merge_start->next ;
      	}
      l2 = l2->next ;
    	}
  	} while ((l1 != NULL) || (l2 != NULL)) ;
  merge_start->next = NULL ;
  return (merge_end) ;
	}

struct Liste *quicksort (struct Liste *l)
	{
	struct Liste *l_help1,*l_help2 ;
	int i,j ;
  if (!l)  return (l) ;
  if (!l->next) return (l) ;
  else
  	{
		i = 1 ;
    l_help1 = l ;
    while (l_help1)
    	{
			l_help1 = l_help1->next ;
      i++ ;
    	}
    l_help1 = l ;
    for (j = 1 ; j < ((i/2)-1) ; j++)	l_help1 = l_help1->next ;
    l_help2 = l_help1->next ;
    l_help1->next = NULL ;
    l_help1 = l ;
    l_help1 = quicksort (l_help1) ;
    l_help2 = quicksort (l_help2) ;
    return (merge (l_help1,l_help2)) ;
  	}
	}

void AddToList(void)
	{
	if(!Fib)
		{
		if(!(Fib=(struct Liste *)AllocVec(sizeof(struct Liste),MEMF_CLEAR|MEMF_PUBLIC))) wb(NO_MEM);
		Fib2=Fib;
		}
	else
		{
		if(!(Fib2->next=(struct Liste *)AllocVec(sizeof(struct Liste),MEMF_CLEAR|MEMF_PUBLIC))) wb(NO_MEM);
		Fib2=Fib2->next;
		}
  CopyMem((APTR) fib,(APTR) Fib2,sizeof(struct PackerList));
	Fib2->next=NULL;
	Fib2->prev=NULL;
	pagecount++;
	}

void MakePrevPtr(void)
	{
	struct	Liste *te,*first;

  Fib=quicksort(Fib);
	Fib2=te=first=Fib;
	Fib2=Fib=Fib2->next;
	while(Fib2)
		{
		Fib->prev=Fib2->prev=te;
    te=Fib2;
		Fib2=Fib=Fib2->next;
		};
	Fib2=Fib=first;
	}

void free_elements (struct Liste *l)
	{
	struct Liste *l_help1,*l_help2 ;
  l_help1 = l ;
  while(l_help1)
  	{
		l_help2 = l_help1 ;
    l_help1 = l_help1->next ;
    FreeVec(l_help2);
  	}
	Fib=NULL;Fib2=NULL;choosen=pagecount=NULL;
  }

void Get_Key(void)
	{
	char	str[3];
	inoutput("\n\n\r",3);
	center(ANYKEY,32);
	str[0]='\0';
	if(JH_PM(str,0)<0) wb(MY_LOST_CARRIER);
	inoutput("\r[K\r",3);
	}
