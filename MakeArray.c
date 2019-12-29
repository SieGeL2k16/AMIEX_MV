#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include "proto.h"
#include "struct_ex.h"

struct StringList {
  char *String ;
  struct StringList *next ;
} *sl1,*sl2 ;

void Free_Structs(void);

void INSERT (char *s)
	{
  if (sl1 == 0)
  	{
    if(!(sl1 = (struct StringList *) AllocVec (sizeof (struct StringList),MEMF_PUBLIC|MEMF_CLEAR))) wb(MY_LOST_CARRIER);
    sl2 = sl1 ;
  	}
  else
  	{
   	if(!(sl2->next = (struct StringList *) AllocVec (sizeof (struct StringList),MEMF_PUBLIC|MEMF_CLEAR))) wb(MY_LOST_CARRIER);
    sl2 = sl2->next ;
  	}
  sl2->next = NULL ;
  if(!(sl2->String = (char *) AllocVec ((strlen(s)+10),MEMF_PUBLIC|MEMF_CLEAR))) wb(MY_LOST_CARRIER);
  strcpy (sl2->String,s) ;
	}

void MoveStrings (char **res,struct StringList *sl)
	{
	struct 	StringList *slb ;
	char 		**strs;

  slb = sl;
  strs = res;
  while(slb)
  	{
    *strs = slb->String;
    strs++ ;
    slb = slb->next ;
  	}
  strs = NULL ;
	}
char **MakeArray (char *s)
	{
	char 	*str1,*str2,**result ;
	int 	i ;
  sl1 = NULL;
  str1 = s;
  i = 0;
  while ((str2 = strchr (str1,' ')))
  	{
    *str2 = 0 ;
    INSERT (str1) ;
    i++ ;
    *str2 = ' ' ;
    while ((*str2 != 0) && (*str2 == ' '))
    str2++ ;
    str1 = str2 ;
  	}
  if (*str1 != 0)
  INSERT (str1) ;
  i++ ;
  result = (char **) AllocMem ((i+1)*4,MEMF_PUBLIC|MEMF_CLEAR) ;
  MoveStrings (result,sl1) ;
  return (result) ;
	}
void FreeArray (char **strings)
	{
	char 	**strs ;
	int 	i ;

	i = 0 ;
  strs = strings ;
  while (*strs)
  	{
    i++ ;
    FreeVec (*strs) ;
    strs++ ;
  	}
  i++ ;
  FreeMem (strings,i*4) ;
	Free_Structs();
	}

void Free_Structs(void)
	{
	struct StringList *h=sl1;
	while(sl1)
		{
		h=sl1;
		sl1=sl1->next;
		FreeVec(h);
		}
	sl1=NULL;sl2=NULL;
	}
