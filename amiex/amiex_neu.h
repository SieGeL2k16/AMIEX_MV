/****************************************************************************
 *                                                                          *
 *                     /X - Includes by ByteAndi/MsT                        *
 *                                                                          *
 *                   Improved at 28.10.93 by S¡eGeL/MsT                     *
 *                                                                          *
 ****************************************************************************/

#include <dos.h>
#include <dos/datetime.h>
#include <exec/exec.h>      
#include <exec/types.h> 
#include <exec/memory.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <fcntl.h>      
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <proto/icon.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <time.h>
#include <devices/timer.h>
#include <ctype.h>

#define SEEK_SET 0
#define FOREVER for(;;)
#define NL NULL
/* #define JH_LI 		0 */
#define JH_REGISTER 	1
#define JH_SHUTDOWN 	2
#define JH_WRITE 	3
#define JH_SM 		4
/*
#define JH_PM 		5
#define JH_HK 		6
#define JH_SG 		7
#define JH_SF 		8
#define JH_EF 		9
*/
#define JH_CO 				10
#define JH_BBSNAME		11
#define JH_SYSOP 			12
#define JH_FLAGFILE 	13
#define JH_SHOWFLAGS 	14
#define JH_DL 				15
#define JH_ExtHK 			15
#define JH_SIGBIT 		16
#define JH_FetchKey 	17
#define JH_FETCHKEY 	17
#define JH_SO 				18
#define JH_HSM 				19
/**************************** GET/SET Values from System ***********************/

#define DT_NAME           100
#define DT_PASSWORD       101
#define DT_LOCATION       102
#define DT_PHONENUMBER    103
#define DT_SLOTNUMBER     104
#define DT_SECSTATUS      105
#define DT_SECBOARD       106
#define DT_SECLIBRARY     107
#define DT_RATIO	  			107
#define DT_SECBULLETIN    108
#define DT_MESSAGESPOSTED 109
#define DT_UPLOADS        110
#define DT_DOWNLOADS      111
#define DT_TIMESCALLED    112
#define DT_TIMELASTON     113
#define DT_TIMEUSED       114
#define DT_TIMELIMIT      115
#define DT_TIMETOTAL      116
#define DT_BYTESUPLOAD    117
#define DT_BYTEDOWNLOAD   118
#define DT_DAILYBYTELIMIT 119
#define DT_DAILYBYTEDLD   120
#define DT_EXPERT         121
#define DT_LINELENGTH     122
#define ACTIVE_NODES      123
#define DT_DUMP           124
#define DT_TIMEOUT        125
#define BB_CONFNAME       126
#define BB_CONFLOCAL      127
#define BB_LOCAL          128
#define BB_STATUS         129
#define BB_COMMAND        130
#define BB_MAINLINE       131
#define NB_LOAD           132
#define DT_USERLOAD       133
#define BB_CONFIG         134
#define CHG_USER          135
#define RETURNCOMMAND     136
#define ZMODEMSEND        137
#define ZMODEMRECEIVE     138
#define SCREEN_ADDRESS    139
#define BB_TASKPRI        140
#define RAWSCREEN_ADDRESS 141
#define BB_CHATFLAG       142
#define DT_STAMP_LASTON   143
#define DT_STAMP_CTIME    144
#define DT_CURR_TIME      145
#define DT_CONFACCESS     146
#define BB_PCONFLOCAL     147
#define BB_PCONFNAME      148
#define BB_NODEID         149
#define BB_CALLERSLOG     150
#define BB_UDLOG          151
#define EXPRESS_VERSION   152
#define UNICONIFY         153

#define BB_CHATSET        162
#define ENVSTAT           163

#define WRITESERVER	  177		/* Not documented yet ! */

#define GETKEY            500
#define RAWARROW          501
#define CHAIN             502

/****************** in progress ******************/

#define NODE_DEVICE       503
#define NODE_UNIT         504
#define NODE_BAUD         505
#define NODE_NUMBER       506
#define JH_MCI            507

/*************************************************/

#define PRV_COMMAND       508
#define PRV_GROUP         509
#define BB_CONFNUM        510
#define BB_DROPDTR        511
#define BB_GETTASK        512
#define BB_REMOVEPORT     513
#define BB_SOPT           514
#define NODE_BAUDRATE     516
#define BB_LOGONTYPE      517
#define BB_SCRLEFT        518
#define BB_SCRTOP         519
#define BB_SCRWIDTH       520
#define BB_SCRHEIGHT      521
#define BB_PURGELINE      522
#define BB_PURGELINESTART 523
#define BB_PURGELINEEND   524
#define BB_NONSTOPTEXT    525
#define BB_LINECOUNT      526
#define DT_LANGUAGE       527
#define DT_QUICKFLAG      528
#define DT_GOODFILE       529
#define DT_ANSICOLOR      530
#define MULTICOM          531
#define LOAD_ACCOUNT      532
#define SAVE_ACCOUNT      533
#define SAVE_CONFDB       534
#define LOAD_CONFDB       535
#define GET_CONFNUM       536
#define SEARCH_ACCOUNT    537
#define APPEND_ACCOUNT    538
#define LAST_ACCOUNTNUM   539
#define MOD_TYPE          540
#define DT_ISANSI         541
#define BATCHZMODEMSEND   542
#define DT_MSGCODE        	543
#define ACP_COMMAND       	544
#define DT_FILECODE      	 	545
#define EDITOR_STRUCT     	546
#define BYPASS_CSI_CHECK  	547
#define SENTBY            	548
#define MCIFLAG           		551
#define MSGBASE_COMMAND      600
#define MSGBASE_CONF         601
#define MSG_LASTREAD_CONF    602
#define MSG_LASTNEWREAD_CONF 603
#define MSGBASE_LOCATION     604
#define MSGBASE_SUPPLEMENT   605
#define DT_REALNAME          606
#define DT_STARTDATE         607
#define QUICK_KEY            608
#define DT_TOGGLE_SERIAL     609
#define NETDOWNLOAD          610
#define NETUPLOAD            611
#define BB_CONFLIST          612
#define BB_OFFHOOK           613
#define IS_CONFACCESS        614
#define BB_ENCRYPT           615
#define BB_NONSTOPSTATUS     616
#define JH_SF_NSF            617
#define JH_SG_NSF            618
#define BB_CHOOSEANAME       619
#define DT_ATTACHFILE        620
#define BB_GETMCISTR         621
#define DT_XIMPORT           622
#define DT_LASTMSGCMD        623
#define DT_ASL               624
#define DT_MSGCMDOVERIDE     625
#define DT_QWKBYTECOUNT      626
#define DT_RELCONF           627
#define RETURNPRVCMD         628
#define BB_TRANSFERABORT     629
#define BB_CLEARMSGQUE       630
#define BB_NOPROMPTDOWNLOAD  631
#define BB_CHECKFORFILE      632
#define LOAD_ACCOUNTMISC     633
#define SAVE_ACCOUNTMISC     634
#define BB_CHOOSEANAMEMISC   635
#define BB_CONFNAMETYPE      636
#define DT_USERNAME          637
#define DT_ADDBIT         	1000
#define DT_REMBIT         	1001
#define DT_QUERYBIT       	1002
#define READIT 1
#define WRITEIT 0
#define SHUTDOWN 1
#define CHAIN_WAIT 0
#define NEXTMSG    1
#define NONEXTMSG  2
#define MSGSTATUS  0
#define CONSOLE_PORT 1
#define SERIAL_PORT 2

// Defines for Usage of Amiex Doors in FAME BBS-Systems

#define FM_LASTCONF      10000 /* Data=number of Confs (is also the last)
																*/
#define FM_SaveMsgFile   10001 /* Data=ConfNr String=path of text
																* Filler1=filled struct FAMEMailHeader ptr
                                * Data=Result:
                                * Results are: 0 = Successfull.
                                *              1 = Your given file doesn't exists.
                                *              2 = Can't get conference datas of you given conf number.
                                *              3 = MsgBase lock failed.
                                *              4 = MsgStatus.dat can't be opened.
                                *              5 = MsgHeader.dat can't be opened.
                                *              6 = Filecopy of message failed.
                                *              7 = Your struct MailHeader isn't valid.
                                */
#define FM_FlagFile      10002 /* Data=ConfNr String=filename str=path to
																* filename LineNum=FreeDlFlag (1=Free/0=not)
																*/
#define FM_CallFlgFiProc 10003 /* Data!=0 no From/NumFrom String=default
																* path usage
																*/
#define FM_ClrFileFlgLst 10004 /* Data=ConfNr to clear the whole file flag
                                * list. Data<1 is currentconf.
                                */
#define FM_SaveMsgFileFF 10005 /* The same as FM_SaveMsgFile but if
																* Str=path to a file which shall Mainpart
																*     attach to the mail! The filed in the
																*     header for files attached will
																*     automaticly be set by MainPart.
                                */
#define FM_SaveMsgFileFD 10006 /* The same as FM_SaveMsgFile but if
																* Str=path to a directory which contents Mainpart
																*     shall attach to the mail! The filed in the
																*     header for files attached will
																*     automaticly be set by MainPart.
                                */

// Additional Defines used for CRSR-Keys & other stuff

#define QUIT 				 "\n\n\r[34mQUIT[m\n\n\r"
#define NO_CLI       "\n\nTHIS TOOL IS ONLY FOR AMIEX V3/4.++ !!!\n\n"
#define LOST_CARRIER "\n\n\r[31mLOST THE CARRIER[34m...[31mABORTING !\n\n\r"
#define ANYKEY       "[36mPress [m<[36mRETURN[m> [36mto continue"

#define zufall(min,max) ((rand()%(int) (((max)+1) - (min) ) ) +(min) )

#define CRSR_UP		4
#define CRSR_DOWN	5
#define CRSR_LEFT	3
#define CRSR_RIGHT	2
#define CRSR_LOST	45
#define ENTER		13

struct JHMessage
	{
 	struct 	Message Msg;
  char 		String[200];
  int Data;
  int Command;
  int NodeID;
  int LineNum;
  unsigned long signal;
  struct Process *aeproc;
  APTR *Semi;
  APTR Filler1;
  APTR Filler2;
  char *str;
  APTR Filler3;
	};

#define NEXTMSG    1
#define NONEXTMSG  2
#define MSGSTATUS  0
#define CONSOLE_PORT 1
#define SERIAL_PORT 2

struct MsgPort *glueport;
struct MsgPort *replymp;
struct JHMessage *Jhmsg,*msg;

char PortName[20];
int maxlines;

void CutAnsi(char *);

VOID Register(long node)
	{
  ULONG 	portsig;

  if(!(Jhmsg=(struct JHMessage *)AllocVec(sizeof(struct JHMessage),MEMF_CLEAR|MEMF_PUBLIC)))
  	{
    Printf("\nNot enough Memory for message structure !\n");
    exit(0);
  	};
	Jhmsg->signal=0;		// Fuer Optimizer !
  if(!(replymp=CreatePort(0L,0L)))
  	{
    Printf("\nCouldn't create reply port !\n");
    FreeVec(Jhmsg);
    exit(0);
  	};
 	Jhmsg->Msg.mn_Node.ln_Type=NT_MESSAGE;
 	Jhmsg->Msg.mn_Length=sizeof(struct JHMessage);
 	Jhmsg->Msg.mn_ReplyPort=replymp;
 	strcpy(Jhmsg->String,"");
 	Jhmsg->Command=JH_REGISTER;
 	Jhmsg->Data=2;
 	Jhmsg->NodeID=-1;
 	Jhmsg->LineNum=0;
 	sprintf(PortName,"AEDoorPort%ld",node);
	Forbid();
 	glueport=FindPort(PortName);
	Permit();
	if(!glueport)
		{
		FreeVec(Jhmsg);
		Printf("\nUnable to find '%s'\n\n",PortName);
		exit(0);
		};
 	PutMsg(glueport,(struct Message *)Jhmsg);
 	portsig=1<<replymp->mp_SigBit;
	Wait(portsig);
	msg=(struct JHMessage *)GetMsg((struct MsgPort *)replymp);
 	maxlines=msg->Command;
	}

void ClosePort(void)
	{
  while(msg=(struct JHMessage *)GetMsg((struct MsgPort *)replymp));
  DeletePort((struct MsgPort *)replymp);
  FreeVec(Jhmsg);
	}

VOID ShutDown(VOID)
	{
 	ULONG 	portsig,signal;
 	portsig=1<<replymp->mp_SigBit;
  Jhmsg->Command=JH_SHUTDOWN;
  PutMsg(glueport,(struct Message *)Jhmsg);
  signal=Wait(portsig);
  ClosePort();
	}

void Transfer( void )
	{
	PutMsg (glueport,(struct Message *)Jhmsg);
	WaitPort(replymp);
	GetMsg(replymp) ;
	}

struct User
	{
 	char  Name[31],
				Pass[9],
				Location[30],
				PhoneNumber[13];
 USHORT	Slot_Number;
 USHORT Sec_Status,
				Sec_Board,               /* File or Byte Ratio */
    		Sec_Library,                 /* Ratio              */
    		Sec_Bulletin,                /* Computer Type      */
    		Messages_Posted;
 ULONG 	NewSinceDate,
				CRCPassword,
				ConfRead2,
				ConfRead3;
 UWORD 	ZoomAscii;
 UWORD  xx;
 UWORD 	xxx;
 UWORD 	Area;
 UWORD 	XferProtocol,
				Filler2;
 UWORD 	Lcfiles,
				BadFiles;
 ULONG 	AccountDate;
 UWORD 	ScreenType,
				EditorType;
 char   Conference_Access[10];
 USHORT Uploads,
				Downloads,
				ConfRJoin,
				Times_Called;
 long   Time_Last_On,
				Time_Used,
				Time_Limit,
				Time_Total;
 ULONG  Bytes_Download,
				Bytes_Upload,
				Daily_Bytes_Limit,
				Daily_Bytes_Dld;
 char   Expert;
 ULONG  ChatRemaining,
				ChatLimit,
				CreditDays,
				CreditAmount,
				CreditStartDate,
				CreditTotal,
				CreditTotalDate,
        CreditBits,
				ConfYM9;
 long   BeginLogCall;
 UBYTE  Protocol,
				UUCPA,
				LineLength,
				New_User;
 	}user;

struct RTSUser
	{
 char 	 pad1[8];
 char    Name[31],
	 Pass[9],
	 Location[30],
	 PhoneNumber[13];
 USHORT  Slot_Number;              /* max 255 should be UBYTE */
 USHORT  Sec_Status,
	 Sec_Board,                   /* File or Byte Ratio */
    	 Sec_Library,                 /* Ratio              */
    	 Sec_Bulletin,                /* Computer Type      */
    	 Messages_Posted;
 ULONG   NewSinceDate;
 ULONG   ConfRead[9];
 char    Conference_Access[10];
 USHORT  Uploads,
	 Downloads,
	 ConfRJoin,
	 Times_Called;
 long    Time_Last_On,
	 Time_Used,
	 Time_Limit,
	 Time_Total;
 ULONG   Bytes_Download,
	 Bytes_Upload,
	 Daily_Bytes_Limit,
	 Daily_Bytes_Dld;
 char    Expert;
 ULONG   ConfYM[9];
 long    BeginLogCall;
 UBYTE   Protocol,
	 UUCPA,
	 LineLength,
	 New_User;
 char 	 pad2[16];
 }rtsuser;

struct OLDUserKeys
	{                 /* changes to UserKeys (RTS) */
 	char    UserName[31];
 	long    Number;
 	UBYTE   New_User;
 	UWORD   UPcps;              /* highest upload cps rate */
 	UWORD   DNcps;              /* highest dnload cps rate */
 	UWORD   Userflags;          /*                         */
 	char    Pad[11];            /* ?? should be 15         */
	}olduserkeys;

struct UserKeys
	{                 /* changes to UserKeys (RTS) */
 	char    UserName[31];
 	long    Number;
 	UBYTE   New_User;
 	UWORD   UPcps;              /* highest upload cps rate */
 	UWORD   DNcps;              /* highest dnload cps rate */
 	UWORD   Userflags;          /*                         */
 	UWORD   baud_rate;          /* last online baud rate   */
 	char    Pad[9];             /* ?? should be 15         */
	}userkeys;

struct MiscData
	{
  char username[10];
  char realname[30];
  ULONG Flags[10];
  ULONG ConfAccess[32];
  ULONG SecurityAccess[10];
	}miscdata;

struct MailHeaderOld30 {
	char	Status[1];
	long	MsgNumb;
  	long 	ActualMsgNumb;
	char	ToName[31],
		FromName[31],
		Subject[31];
  	char 	ToNet[8],
		FromNet[8];
	long	MsgDate,
		Recv;
  	long 	threaded;
	char 	Pad;
	}mailheaderold30;

struct MailHeader {
	char	Status[1];
	long	MsgNumb;
	char	ToName[31],
		FromName[31],
		Subject[31];
	long	MsgDate,
		Recv;
	char 	Pad;
	}mailheader;

struct MailStat
	{
	long	LowestKey,
		HighMsgNum,
		LowestNotDel;
	char 	Pad[6];
	}mailstat;

struct ConfBase
{
  char Handle[31];
  ULONG NewSinceDate, ConfRead,ConfYM;
  ULONG Bytes_Download,Bytes_Upload;
  ULONG LastEMail,Daily_Bytes_Dld;
  USHORT Upload,Downloads,RatioType,Ratio,Messages_Posted;
  UWORD Access;
  UWORD Active;
}confbase;

struct MyData
{
  int Type,Size,misc1,Version;
}mydata;

struct MasterData
{
   struct MyData   Code;
   struct ConfBase Conf;
}masterdata;

/* stupid defines , theres gotta be a better way , will have to
   consult the Lattice Manual */
struct Globals
{
  char SystemPrompt[60];
  char NamePrompt  [60];
  char ConfDB      [40];
  char UserDataLoc [80];
  char UserKeyLoc  [80];
  char NodeString  [80];
  UBYTE NodeWork   [80]; 
  char FcheckLoc   [80];
  char Conference_Location[80];
  char Conference_Name[80];
  char MsgBase_Location[80];
  char Upload_Location[80];
  char Screen_Location[80];
  UBYTE GSTR1[1024];
  UBYTE GSTR2[1024];
  UBYTE GSTR3[1024];
  char  WORKB[1024];
  char  flaglist[2048];
  char  rfinal[3072];
  char RecFileNames[1024];
  BOOL Do_DebugLog;
  BOOL PhoneCheck; 
  BOOL ViewPassword;
  BOOL SerialTrap;
  BOOL StealthMode;
  BOOL QuietNode;
  BOOL UserQuietNode;
  BOOL WhenceTheCommand;
  char UserMiscLoc[80];
}globals;

struct StartOption
{
  SHORT LeftEdge;
  SHORT TopEdge;
  SHORT Width;
  SHORT Height;
  int BitPlanes;
  BOOL StatBar;
  BOOL Interlace;
  BOOL DupeCheck;
  BOOL QLogon;
  BOOL TakeCredits;
  BOOL SeenIt;
  BOOL TrapDoor;
  BOOL Iconify;
  BOOL Eall_level;
  BOOL A2232;
  BOOL Toggles[20];
  char Logoff[80];
  char ShutDown[80];
  char NameData[80];
  char RamPen[80];
  char BBSConfig[80];
  char FilesNot[80];
  char UserData[80];
  char UserKey[80];
  char OffHook[80];
  char NodeScreen[80];
  APTR t;
  APTR s;
  ULONG IO_Address;
  ULONG acp_window;
}startoption;

void change(char str [],int dir,int fct)
	{
	Jhmsg->Command=fct;
	Jhmsg->Data=dir;
	if(!dir) strcpy(Jhmsg->String,str);
	Transfer();
	if(dir==1) strcpy(str,Jhmsg->String);
	}

int JH_LI(char str [],int len)
	{
	Jhmsg->Command=0;
	strcpy(Jhmsg->String,str);
	Jhmsg->Data=len;
	Transfer();
	strcpy(str,Jhmsg->String);
	return(Jhmsg->Data);
	}
int JH_HK(char str [])
	{
	Jhmsg->Command=6;
	strcpy(Jhmsg->String,str);
	Transfer() ;
	strcpy(str,Jhmsg->String);
	return(Jhmsg->Data);
	}

int JH_PM(char str [],int len)
	{
	Jhmsg->Command=5;
	strcpy(Jhmsg->String,str);
	Jhmsg->Data=len;
	Transfer();
	strcpy(str,Jhmsg->String);
	return(Jhmsg->Data);
	}
int JH_EF(char fname [])
 	{
  Jhmsg->Command=9;
  strcpy(Jhmsg->String,fname);
  Jhmsg->Data=0;
  Transfer();
  return(Jhmsg->Data);
 	}
void JH_SF(char str [])
  	{
 	Jhmsg->Command=8;
  	strcpy(Jhmsg->String,str);
  	Transfer() ;
  	}

void inoutput(char str [],int Command)
	{
	Jhmsg->Command=Command;
	strcpy (Jhmsg->String,str) ;
	Transfer () ;
	}

int Download(char *s)
	{
 	strcpy(Jhmsg->String,s);
 	Jhmsg->Command=ZMODEMSEND;
	Transfer();
	switch(Jhmsg->Data)
		{
		case	0:	return(0);
		case  1:	return(1);
		case -2:  return(-2);
		}
 	}

int BatchDownload(APTR s)
	{
  Jhmsg->Filler1=s;
  Jhmsg->Command=BATCHZMODEMSEND;
  Transfer();
	switch(Jhmsg->Data)
		{
		case 	0:	return(0);
		case 	1:	return(1);
		case -2:  return(-2);
		}
 	}

int NetUpload(APTR s)
	{
 	Jhmsg->Filler1=s;
 	Jhmsg->Command=NETUPLOAD;
	Transfer();
	switch(Jhmsg->Data)
		{
		case	0:	return(0);
		case  1:	return(1);
		case -2:  return(-2);
		}
 	}

int Upload(char *s)
	{
 	strcpy(Jhmsg->String,s);
 	Jhmsg->Command=ZMODEMRECEIVE;
	Transfer();
	switch(Jhmsg->Data)
		{
		case	0:	return(0);
		case  1:	return(1);
		case -2:  return(-2);
		}
 	}

int NetDownload(char *s)
	{
 	strcpy(Jhmsg->String,s);
 	Jhmsg->Command=NETDOWNLOAD;
	Transfer();
	switch(Jhmsg->Data)
		{
		case	0:	return(0);
		case  1:	return(1);
		case -2:  return(-2);
		}
 	}

char Fhotkey(void)
	{
	Jhmsg->String[0]='\0';
 	Jhmsg->LineNum=0;
 	Jhmsg->Command=6;		/* Hotkey! */
	Transfer();
 	if(Jhmsg->Data==-1) return((char) -1);
  else return(*(Jhmsg->String));
	}

void center(char *stri,int farbe)
	{
	char 	r[200],s[200];
	long	mitte;

	mitte=strlen(stri);
  strcpy(r,stri);
	CutAnsi(r);
	mitte=((78-strlen(r))/2);
	sprintf(s,"\r[%ldC[%ldm%s\0",mitte,farbe,stri);
	inoutput(s,3);
	}

void GetFiller1(APTR Filler,int Command)
	{
	Jhmsg->Command=Command;
	Jhmsg->Data=0;
	Jhmsg->Filler1=Filler;
	PutMsg(glueport,(struct Message *)Jhmsg);
	WaitPort(replymp);
	GetMsg(replymp);
	}
void PutFiller1(APTR Filler,int Command)
	{
  Jhmsg->Command=Command;
  Jhmsg->Data=1;
  Jhmsg->Filler1=Filler;
  PutMsg(glueport,(struct Message *)Jhmsg);
  WaitPort(replymp);
  GetMsg(replymp);
	}

int Load_Account(int UserNum,APTR u,APTR uk,APTR um)
	{
	Jhmsg->Command = LOAD_ACCOUNTMISC;
  Jhmsg->Data=UserNum;
  Jhmsg->Filler1=u;
  Jhmsg->Filler2=uk;
  Jhmsg->Filler3=um;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  return(Jhmsg->Data);
	}

LONG Choose_A_Name(APTR u,APTR uk,APTR um,WORD option)
	{
	Jhmsg->Command = BB_CHOOSEANAMEMISC;
  Jhmsg->Data=option;
  Jhmsg->Filler1=u;
  Jhmsg->Filler2=uk;
  Jhmsg->Filler3=um;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  return((LONG)Jhmsg->Data);
	}

int Search_Account(int UserNum,APTR uk,APTR um)
	{
	Jhmsg->Command = SEARCH_ACCOUNT;
  Jhmsg->Data=UserNum;
  Jhmsg->Filler1=uk;
	Jhmsg->Filler2=um;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  return(Jhmsg->Data);
	}

void Save_Account(int UserNum,APTR u,APTR uk,APTR um)
	{
	Jhmsg->Command = SAVE_ACCOUNTMISC;
  Jhmsg->Data=UserNum;
  Jhmsg->Filler1=u;
  Jhmsg->Filler2=uk;
  Jhmsg->Filler3=um;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
	}

void Save_ConfDB(int UserNum,int conf,APTR dat)
	{
  Jhmsg->Command = SAVE_CONFDB;
  Jhmsg->Data=UserNum;
  Jhmsg->NodeID=conf;
  Jhmsg->Filler1=dat;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
	}

void New_Account(APTR u,APTR uk,APTR um)
	{
  Jhmsg->Command = APPEND_ACCOUNT;
  Jhmsg->Filler1=u;
  Jhmsg->Filler2=uk;
  Jhmsg->Filler3=um;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
	}

void Load_ConfDB(int UserNum,int conf,APTR dat)
	{
  Jhmsg->Command = LOAD_CONFDB;
  Jhmsg->Data=UserNum;
  Jhmsg->NodeID=conf;
  Jhmsg->Filler1=dat;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
	}

int Get_ConfName(APTR n,APTR l,int num)
	{
  char *s;
  strcpy((char *)n,"");
  Jhmsg->Command = GET_CONFNUM;
  Jhmsg->Data=num;
  Jhmsg->Filler1=n;
  Jhmsg->Filler2=l;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  s=(char *)n;
  if(*s=='\0') return(0);
  return(1);
	}

int IsAccess(int acs)
	{
  Jhmsg->Command = DT_QUERYBIT;
  Jhmsg->Data=acs;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  return(Jhmsg->Command);
	}

void AcpCommand(char mstring[],int command,int node)
	{
	int Line;

  strcpy(Jhmsg->String,mstring);
  Line=Jhmsg->LineNum;
  Jhmsg->LineNum=node;
  Jhmsg->Data=command;
  Jhmsg->Command=ACP_COMMAND;
	PutMsg(glueport,(struct Message *)Jhmsg);
	(void)WaitPort(replymp);
	(void)GetMsg(replymp);
  Jhmsg->LineNum=Line;
	}

void ToggleNonstop(int wie)
	{
	Jhmsg->Command=BB_NONSTOPTEXT;
	Jhmsg->Data=wie;
	PutMsg(glueport,(struct Message *)Jhmsg);
	WaitPort(replymp);
	GetMsg(replymp);
	}

void smp(char *s,int nl)
	{
 	Jhmsg->Data = nl;				// 1=\n\r
  Jhmsg->Command=JH_HSM;
  Jhmsg->str=s;
  PutMsg(glueport,(struct Message *)Jhmsg);
  WaitPort(replymp);
  GetMsg(replymp);
	}

int	CheckAnsiFlag(void)
	{
	Jhmsg->Command=DT_ISANSI;
	PutMsg(glueport,(struct Message *)Jhmsg);
	WaitPort(replymp);
	GetMsg(replymp);
	return(Jhmsg->Data);
	}

void CutAnsi(char *e)
	{
	char 	*d,
				*s,
				*b;
	int 	i;

	d=s=e;
	while(*d)
		{
		if(*d=='' || *d=='›')
			{
			i=1;
			b=d;
			while(i<=5 && i>0)
				{
				switch(*b)
					{
					case	';'	:	i=1;
											*b++;
											break;
					case	'm'	:	i=NULL;
											*b++;
											break;
					case	'\0':	i=6;
											break;
					default		:	*b++; i++;
					}
				}
			if(!i)	d=b;
			else if(*d) *s++=*d++;
			}else *s++=*d++;
		}
	*s='\0';
	}

/*

void FameFlag(char *FileName, char *Path, long confnum)
	{
	Jhmsg->Command=SM_FlagFile;
	Jhmsg->Data=confnum;
	strcpy(Jhmsg->String,FileName);
	Jhmsg->str=Path;
	Jhmsg->LineNum=0;
  PutMsg(glueport,(struct Message *)Jhmsg);
  WaitPort(replymp);
  GetMsg(replymp);
	}
*/
