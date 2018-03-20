/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u32 G_u32AntApiCurrentMessageTimeStamp;                    
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;  



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout = 0;                      /* Timeout counter used across states */

AntAssignChannelInfoType sChannelInfo;

static u8 au8IncomingData[] = {0,0,0,0,0,0,0,0};
static u8 au8OutgoingData[] = {0,0,0,0,0,0,0,0};

static u8 u8GameVal = 0;
static u8 u8RecentData = 0;
static u8 u8LastData = 0;
static bool bTurn;

static u8 au8PrintChar[5] = "     ";
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
    CLEAR_ALL();
    
     /* Configure ANT for this application */
    ANT_INIT();
    
    LCDMessage(LINE1_START_ADDR, "Press B0 to gen #");
    LCDMessage(LINE2_START_ADDR, "Press B3 to wait");
    LedBlink(YELLOW, LED_2HZ);
    
    UserApp1_StateMachine = UserApp1SM_Gen_or_Wait;
} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void Led_OFF(void)
{
  LedOff(RED);
  LedOff(ORANGE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(CYAN);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(WHITE);
}

static void CLEAR_ALL(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  UserApp1_u32Timeout = 0;
  Led_OFF();
}

static void DISPLAY_EDIT(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  au8PrintChar[0] = 48 + u8GameVal;
  LCDMessage(LINE1_START_ADDR, "Current Value:");
  LCDMessage(0x11, au8PrintChar);
  LCDMessage(LINE2_START_ADDR, "+1    -3    x2    /2");
}

static void DISPLAY_WAIT(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "Waiting...");
  LCDMessage(LINE2_START_ADDR, "Not your turn.");
}

static void RNG(void)
{
  u8GameVal = G_u32SystemTime1ms % 10;
}

/* ANT FUNCTIONS */
static void ANT_INIT(void)
{
  sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;
  sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
}
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Gen_or_Wait(void)
{
  UserApp1_u32Timeout++;
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    CLEAR_ALL();
    
    RNG();
    
    sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
    bTurn = TRUE;
    UserApp1_StateMachine = UserApp1SM_ANT_Init;
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    CLEAR_ALL();
    
    
    
    sChannelInfo.AntChannelType = CHANNEL_TYPE_SLAVE;
    bTurn = FALSE;
    UserApp1_StateMachine = UserApp1SM_ANT_Init;
  }
  // ADD TIMEOUT
} /* end UserApp1SM_M_or_S() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_ANT_Init(void)
{
  if( AntAssignChannel(&sChannelInfo))
  {
    CLEAR_ALL();
    UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
  }
  else
  {
    CLEAR_ALL();
    LCDMessage(0x00, "Failed to Init");
    LedOn(RED);
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_ANT_ChannelAssign(void)
{
  UserApp1_u32Timeout++;
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    CLEAR_ALL();
    LedOn(GREEN);
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    DISPLAY_EDIT();
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  if(UserApp1_u32Timeout == 5000)
  {
    CLEAR_ALL();
    LCDMessage(0x00, "Failed to Config");
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if( bTurn )
  {
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      u8GameVal++;
      au8OutgoingData[0] = u8GameVal;
      u8RecentData = u8GameVal;
      bTurn = FALSE;
      DISPLAY_WAIT();
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      u8GameVal -= 3;
      au8OutgoingData[0] = u8GameVal;
      u8RecentData = u8GameVal;
      bTurn = FALSE;
      DISPLAY_WAIT();
    }
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      u8GameVal *= 2;
      au8OutgoingData[0] = u8GameVal;
      u8RecentData = u8GameVal;
      bTurn = FALSE;
      DISPLAY_WAIT();
    }
    if(WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      u8GameVal /= 2;
      au8OutgoingData[0] = u8GameVal;
      u8RecentData = u8GameVal;
      bTurn = FALSE;
      DISPLAY_WAIT();
    }
  }
  
  /*
  if(bTurn)
  {
    //UPDATE LASTDATA AND RECENT DATA to incoming data
      //When GameVal is changed, update RECENTDATA and bTurn = False
  }
  if(!bTurn && (au8IncomingData[0] != u8RecentData))
  {
    //DO NOT UPDATE LASTDATA
  }
  if(!bTurn && (au8IncomingData[0] == u8RecentData))
  {
    //UPDATE LASTDATA
  }
  if(!bTurn && (au8IncomingData[0] != u8LastData))
  {
    //UPDATE GameVal and bTurn = TRUE
  }
  */
  
  if( AntReadAppMessageBuffer() )
  {
    au8IncomingData[0] = G_au8AntApiCurrentMessageBytes[0];
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(bTurn)
      {
        DISPLAY_EDIT();
        LedOn(GREEN);
        u8RecentData = au8IncomingData[0];
        u8LastData = au8IncomingData[0];
      }
      else if(!bTurn && (au8IncomingData[0] == u8RecentData))
      {
        LedOn(YELLOW);
        u8LastData = au8IncomingData[0];
      }
      else if(!bTurn && (au8IncomingData[0] != u8LastData))
      {
        LedOn(BLUE);
        u8GameVal = au8IncomingData[0];
        bTurn = TRUE;
      }
      else
      {
        LedOn(ORANGE);
        //Don't update u8LastData
      }
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {}
    AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8OutgoingData);
  }
  Led_OFF();
} 
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
