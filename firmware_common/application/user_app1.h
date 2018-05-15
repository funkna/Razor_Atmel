/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Required constants for ANT channel configuration */
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0         // Channel 0 - 7
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x00              // LO; 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x20              // HI; 0x0001 - 0x7fff
#define ANT_DEVICEID_LO_USERAPP         (u8)0xF0              // Device # Low byte
#define ANT_DEVICEID_HI_USERAPP         (u8)0x25              // Device # High byte
#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 // 1-127; MSB is pairing
#define ANT_FREQUENCY_USERAPP           (u8)50                // 2400MHz + 0 - 99 MHz
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   // Max tx power


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
//Functions to make tame this beast
static void LedOffAll(void);
static void ClearAll(void);
static void DisplayEdit(void);
static void DisplayWait(void);
static void AcknowledgeAll(void);

//Functions for LED patterns
static void LedDisplay1(void);
static void LedDisplay2(void);
static void BuzzerBlip1(void);

//Functions to edit game values
static void RandomNumber(void);
static void Operations(void);
static void GameAdd(void);
static void GameSub(void);
static void GameMult(void);
static void GameDiv(void);
static void CheckGameState(void);
static void Countdown(void);

//Functions that make ANT tolerable
static void AntInit(void);
static void AntMasterConfig(void);
static void AntSlaveConfig(void);

/*   ANT MESSAGE BYTE USAGE:
0 - Game Value
1 - Game Status Flag
2 - Flag for Value Edit
3 - Empty
4 - Empty
5 - Empty
6 - Empty
7 - Empty
*/

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Intro(void);

static void UserApp1SM_Game_State(void);    

static void UserApp1SM_ANT_ChannelAssign(void);

static void UserApp1SM_Gen_or_Wait(void);

static void UserApp1SM_Error(void);         

static void UserApp1SM_Win(void);

static void UserApp1SM_Lose(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
