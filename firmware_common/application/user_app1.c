/*********************************************************************************************************************
File: user_app1.c                                                                
*************HOT POTATO MATH GAME*************************************************************************************
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



//Variables to keep track of ANT messages
static u8 au8IncomingData[] = {0,0,0,0,0,0,0,0};
static u8 au8OutgoingData[] = {0,0,0,0,0,0,0,0};
static u8 u8GameVal = 0;
static u8 u8PreviousSent = 0;
static u8 u8RecentData = 0;
static u8 u8LastData = 0;
static bool bTurn;



//Strings to print on the LCD 
//Max string length is:             "                    "
static u8 au8WelcomeMessage[]     = "Hot Potato Math Game";
static u8 au8Instruction_1[]      = "Press B0 to go 1st";
static u8 au8Instruction_2[]      = "Press B3 to go 2nd";
static u8 au8Operations[]         = "+1    -3    x2    /2";
static u8 au8Disp_Val[]           = "Current Value:";
static u8 au8GameValChar[]        = "    ";
static u8 au8Wait_1[]             = "Waiting...";
static u8 au8Wait_2[]             = "It's not your turn.";
static u8 au8ANTFailConfig[]      = "Failed to Config ANT";
static u8 au8ANTFailInit[]        = "Failed to Initialize";
static u8 au8ErrorReset[]         = "Reset your board";
static u8 au8GenericError[]       = "Something went wrong";
static u8 au8Timeout[]            = "Timeout: Reset Board";
static u8 au8Win_1[]              = "Congratulations!";
static u8 au8Win_2[]              = "You won!";
static u8 au8Lose_1[]             = "You lose.";
static u8 au8Lose_2[]             = "Nice try.";
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
    ClearAll();
    LCDMessage(LINE1_START_ADDR, au8Instruction_1);
    LCDMessage(LINE2_START_ADDR, au8Instruction_2);
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

/* QoL FUNCTIONS */
static void LedOffAll(void)
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

static void ClearAll(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  UserApp1_u32Timeout = 0;
  LedOffAll();
}

static void DisplayEdit(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  au8GameValChar[0] = 48 + u8GameVal;
  LCDMessage(LINE1_START_ADDR, au8Disp_Val);
  LCDMessage(0x11, au8GameValChar);
  LCDMessage(LINE2_START_ADDR, au8Operations);
}

static void DisplayWait(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8Wait_1);
  LCDMessage(LINE2_START_ADDR, au8Wait_2);
}

static void AcknowledgeAll(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
  }
}
/* LED FUNCTIONS */
static void LedDisplay1(void)
{
  static u8 u8Color = 0;
  static u16 u16BlinkCount = 0;
  static u8 u8Bool = 0;
  static u8 u8Bool_2 = 0;
  static u8 u8ColorStep = 0;
  static LedRateType ePWM = LED_PWM_0;
  
  u16BlinkCount++;
  if(u16BlinkCount >= 40)
  {
    u16BlinkCount = 0;
    
    if(ePWM <= LED_PWM_0)
    {
      u8Bool = 0;
      u8ColorStep++;
    }
    if(ePWM >= LED_PWM_100)
    {
      u8Bool = 1;
      u8ColorStep++;
    }
    
    if(u8Bool == 0)
      ePWM++;
    else
      ePWM--;
    
    if(u8ColorStep >= 3)
    {
      u8ColorStep = 1;
    if(u8Bool_2 == 0)
      u8Color++;
    if(u8Bool_2 == 1)
      u8Color--;
    if(u8Color >= 6)
      u8Bool_2 = 1;
    if(u8Color <= 0)
      u8Bool_2 = 0;
    }
    switch(u8Color)
    {
    case 0: /*purple*/
        LedPWM(PURPLE, ePWM);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    break;
    case 1: /*blue*/
        LedPWM(BLUE, ePWM);
    LedOff(PURPLE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);      
    break;
    case 2: /*cyan*/
        LedPWM(CYAN, ePWM);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);      
    break;
    case 3: /*green*/
        LedPWM(GREEN, ePWM);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);      
    break;
    case 4: /*yellow*/
        LedPWM(YELLOW, ePWM);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(ORANGE);
    LedOff(RED);      
    break;
    case 5: /*orange*/
        LedPWM(ORANGE, ePWM);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(RED);      
    break;
    case 6: /*red*/
        LedPWM(RED, ePWM);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);  
    break;
    default: /*All PWM fade*/
      LedOffAll();
    break;
    }
  }
}

static void LedDisplay2(void)
{
  static u16 u16BlinkCount = 0;
  static bool bLED_BOOL;
  static LedRateType ePWM = LED_PWM_0;
  
  u16BlinkCount++;
  if(u16BlinkCount >= 100)
  {
    u16BlinkCount = 0;
    
    if(ePWM <= LED_PWM_0)
    {
      bLED_BOOL = FALSE;
    }
    if(ePWM >= LED_PWM_100)
    {
      bLED_BOOL = TRUE;
    }
    
    if(!bLED_BOOL)
      ePWM++;
    else
      ePWM--;
    
    LedPWM(RED, ePWM);  
    LedPWM(ORANGE, ePWM);
    LedPWM(YELLOW, ePWM);
    LedPWM(GREEN, ePWM);
    LedPWM(CYAN, ePWM);
    LedPWM(BLUE, ePWM);
    LedPWM(PURPLE, ePWM);
    LedPWM(WHITE, ePWM);
  }
}

static void BuzzerBlip1(void)
{
  PWMAudioSetFrequency(BUZZER1, 262);
  if( IsButtonPressed(BUTTON0) || IsButtonPressed(BUTTON1) ||
      IsButtonPressed(BUTTON2) || IsButtonPressed(BUTTON3) )
  {
    PWMAudioOn(BUZZER1);
  }
  else
  {
    PWMAudioOff(BUZZER1);    
  }
}

/* GAMEPLAY FUNCTIONS */
/*
static void RedundancyCheck(void)
{
  if(u8GameVal == u8LastData)
  {
    
  }
}
*/
static void RandomNumber(void)
{
  u8GameVal = G_u32SystemTime1ms % 10;
  if(u8GameVal == 1 || u8GameVal == 0)
  {
    u8GameVal = 2;
  }
}

static void Operations(void)
{
  if( bTurn )
  {
    Countdown();
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      GameAdd();
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      GameSub();
    }
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      GameMult();
    }
    if(WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      GameDiv();
    }
  }
  else
  {
    LedOffAll();
    UserApp1_u32Timeout = 0;
  }
  //au8OutgoingData[2] = 1;
}
     
static void GameAdd(void)
{
  //u8PreviousSent = u8GameVal;
  u8GameVal += 1;
  au8OutgoingData[0] = u8GameVal;
  u8RecentData = u8GameVal;
  bTurn = FALSE;
  DisplayWait();
}
     
static void GameSub(void)
{
  u8GameVal -= 3;
  au8OutgoingData[0] = u8GameVal;
  u8RecentData = u8GameVal;
  bTurn = FALSE;
  DisplayWait();
}
     
static void GameMult(void)
{
  u8GameVal *= 2;
  au8OutgoingData[0] = u8GameVal;
  u8RecentData = u8GameVal;
  bTurn = FALSE;
  DisplayWait();
}
     
static void GameDiv(void)
{
  if(u8GameVal%2)
    u8GameVal--;
  u8GameVal /= 2;
  au8OutgoingData[0] = u8GameVal;
  u8RecentData = u8GameVal;
  bTurn = FALSE;
  DisplayWait();
}

static void CheckGameState(void)
{
  if(au8IncomingData[1] == 1 || u8GameVal == 1)
  {
    ClearAll();
    LedOn(PURPLE);
    LCDMessage(LINE1_START_ADDR, au8Win_1);
    LCDMessage(LINE2_START_ADDR, au8Win_2);
    PWMAudioOff(BUZZER1);
    UserApp1_StateMachine = UserApp1SM_Win;
  }
  if(au8IncomingData[1] == 2 ||  u8GameVal < 0 ||  u8GameVal > 10)
  {
    ClearAll();
    LedBlink(RED, LED_4HZ);
    LCDMessage(LINE1_START_ADDR, au8Lose_1);
    LCDMessage(LINE2_START_ADDR, au8Lose_2);
    PWMAudioOff(BUZZER1);
    UserApp1_StateMachine = UserApp1SM_Lose;
  }
}

static void Countdown(void)
{
  UserApp1_u32Timeout++;
  if(UserApp1_u32Timeout < 500)
  {
    LedOn(WHITE);
  }
  if(UserApp1_u32Timeout == 500)
  {
    LedOn(PURPLE);
  }
  if(UserApp1_u32Timeout == 1000)
  {
    LedOn(BLUE);
  }
  if(UserApp1_u32Timeout == 1500)
  {
    LedOn(CYAN);
  }
  if(UserApp1_u32Timeout == 2000)
  {
    LedOn(GREEN);
  }
  if(UserApp1_u32Timeout == 2500)
  {
    LedOn(YELLOW);
  }
  if(UserApp1_u32Timeout == 3000)
  {
    LedOn(ORANGE);
  }
  if(UserApp1_u32Timeout == 3500)
  {
    LedOn(RED);
  }
  if(UserApp1_u32Timeout == 4000)
  {
    ClearAll();
    u8GameVal = 15;
    au8OutgoingData[1] = 2;
  }
}

/* ANT FUNCTIONS */
static void AntInit(void)
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
static void AntMasterConfig(void)
{
  AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
  if(AntAssignChannel(&sChannelInfo))
  {
    UserApp1_u32Timeout++;
    UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
    {
      LedOff(YELLOW);
      LedOn(GREEN);
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_StateMachine = UserApp1SM_Game_State;
    }
    if(UserApp1_u32Timeout == 5000)
    {
      ClearAll();
      LCDMessage(LINE1_START_ADDR, au8ANTFailConfig);
      LCDMessage(LINE2_START_ADDR, au8ErrorReset);
      LedOn(RED);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
  }
  else
  {
    ClearAll();
    LCDMessage(LINE1_START_ADDR, au8ANTFailInit);
    LCDMessage(LINE2_START_ADDR, au8ErrorReset);
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

static void AntSlaveConfig(void)
{
  AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_SLAVE;
  if(AntAssignChannel(&sChannelInfo))
  {
    UserApp1_u32Timeout++;
    UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
    {
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_StateMachine = UserApp1SM_Game_State;
    }
    if(UserApp1_u32Timeout == 5000)
    {
      ClearAll();
      LCDMessage(LINE1_START_ADDR, au8ANTFailConfig);
      LCDMessage(LINE2_START_ADDR, au8ErrorReset);
      LedOn(RED);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
  }
  else
  {
    ClearAll();
    LCDMessage(LINE1_START_ADDR, au8ANTFailInit);
    LCDMessage(LINE2_START_ADDR, au8ErrorReset);
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for Initialization */
static void UserApp1SM_Gen_or_Wait(void)
{
  UserApp1_u32Timeout++;
  LedDisplay1();
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ClearAll();
    RandomNumber();
    bTurn = TRUE;
    DisplayEdit();
    AntMasterConfig();
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    ClearAll();
    bTurn = FALSE;
    DisplayWait();
    AntSlaveConfig();
  }
  if(UserApp1_u32Timeout == 6000000)
  {
    ClearAll();
    LedOn(RED);
    LCDMessage(LINE1_START_ADDR, au8Timeout);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1SM_Gen_or_Wait() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for AntInit() */
static void UserApp1SM_ANT_ChannelAssign(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Game_State;
  }
  UserApp1_u32Timeout++;
  if(UserApp1_u32Timeout == 5000)
  {
    ClearAll();
    LCDMessage(LINE1_START_ADDR, au8ANTFailConfig);
    LCDMessage(LINE2_START_ADDR, au8ErrorReset);
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
  AcknowledgeAll();
} /* end UserApp1SM_ChannelAssign() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT Config */
static void UserApp1SM_Game_State(void)
{
  Operations();
  BuzzerBlip1();
  if( AntReadAppMessageBuffer() )
  {
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      au8IncomingData[i] = G_au8AntApiCurrentMessageBytes[i];
    }
    
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(bTurn)
      {
        DisplayEdit();
        u8RecentData = au8IncomingData[0];
        u8LastData = au8IncomingData[0];
      }
      else
      {
        /*if(au8IncomingData[2] == 1)
        {
          u8LastData = -1;
        }*/
        
        if(au8IncomingData[0] == u8RecentData)
        {
          u8LastData = au8IncomingData[0];
        }
        else if(au8IncomingData[0] != u8LastData)
        {
          u8GameVal = au8IncomingData[0];
          bTurn = TRUE;
        }
        else
        {
          //Don't update LastData
        }
        AcknowledgeAll();
      }
    }
    AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8OutgoingData);
    //au8OutgoingData[2] = 0;
    CheckGameState();
  }
  
} /* end UserApp1SM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* GameVal == 1 */
static void UserApp1SM_Win(void)          
{

} /* end UserApp1SM_Win() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* 0 < GameVal > 10 */
static void UserApp1SM_Lose(void)          
{

} /* end UserApp1SM_Lose() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
