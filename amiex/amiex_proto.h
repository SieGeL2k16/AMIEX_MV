                      /* PROTOHEADER FOR AMIEX_NEU.H */

/* ALL PROTOTYPES USED IN THIS DOORHEADER */

extern void Register(long);
extern void ClosePort(void);
extern void ShutDown(void);
extern void Transfer(void);
extern void change(char *,int,int);
extern int 	JH_LI(char *,int);
extern int 	JH_HK(char *);
extern int 	JH_PM(char *,int);
extern int 	JH_EF(char *);
extern void JH_SF(char *);
extern void inoutput(char *,int);
extern int 	Load_Account(int,APTR u,APTR uk,APTR um);
extern LONG Choose_A_Name(APTR u,APTR uk,APTR,WORD option);
extern int 	Search_Account(int UserNum,APTR uk,APTR um);
extern void Save_Account(int UserNum,APTR u,APTR uk,APTR);
extern void Save_ConfDB(int UserNum,int conf,APTR dat);
extern void New_Account(APTR u,APTR uk,APTR);
extern void Load_ConfDB(int UserNum,int conf,APTR dat);
extern int 	Download(char *s);
extern int 	BatchDownload(APTR s);
extern int 	NetUpload(APTR s);
extern int 	Upload(char *s);
extern int 	NetDownload(char *s);
extern char Fhotkey(void);
extern void center(char *stri,int color);
extern void GetFiller1(APTR,int);
extern void PutFiller1(APTR,int);
extern int 	IsAccess(int);
extern void AcpCommand(char *,int ,int);
extern void smp(char *s,int nl);
extern int	CheckAnsiFlag(void);
extern void ToggleNonstop(int);
extern void CutAnsi(char *);
extern void FameFlag(char *, char *, long);
extern VOID __stdargs SPrintf(STRPTR buffer, char *formatString,...);
extern long Atol(STRPTR);

extern struct JHMessage
	{
 	struct 	Message Msg;
  	char 	String[200];
  	int 	Data;
  	int	Command;
  	int 	NodeID;
  	int 	LineNum;
  	unsigned long signal;
  	struct 	Process *aeproc;
  	APTR 	*Semi;
  	APTR 	Filler1;
  	APTR 	Filler2;
  	char 	*str;
  	APTR 	Filler3;
	};

extern	struct MsgPort *glueport;
extern	struct MsgPort *replymp;
extern	struct JHMessage *Jhmsg,*msg;



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
#define JH_CO 10
#define JH_BBSNAME	11  
#define JH_SYSOP 	12    
#define JH_FLAGFILE 	13 
#define JH_SHOWFLAGS 	14
#define JH_DL 		15       
#define JH_ExtHK 	15
#define JH_SIGBIT 	16   
#define JH_FetchKey 	17 
#define JH_FETCHKEY 	17 
#define JH_SO 		18
#define JH_HSM 19
/**************************** GET/SET Values from System ***********************/

#define DT_NAME           100
#define DT_PASSWORD       101
#define DT_LOCATION       102
#define DT_PHONENUMBER    103
#define DT_SLOTNUMBER     104
#define DT_SECSTATUS      105
#define DT_ACCESSLEVEL    105
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
#define DT_MSGCODE        543
#define ACP_COMMAND       544
#define DT_FILECODE       545
#define EDITOR_STRUCT     546
#define BYPASS_CSI_CHECK  547
#define SENTBY            548
#define MCIFLAG           551
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
#define DT_ADDBIT         1000
#define DT_REMBIT         1001
#define DT_QUERYBIT       1002
#define READIT 1
#define WRITEIT 0
#define SHUTDOWN 1
#define CHAIN_WAIT 0
#define NEXTMSG    1
#define NONEXTMSG  2
#define MSGSTATUS  0
#define CONSOLE_PORT 1
#define SERIAL_PORT 2
#define SM_LastConf      10000
#define SM_SaveMsgFile   10001
#define SM_FlagFile      10002

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
#define CRSR_LOST	-1
#define ENTER		13
#define NEXTMSG    1
#define NONEXTMSG  2
#define MSGSTATUS  0
#define CONSOLE_PORT 1
#define SERIAL_PORT 2
