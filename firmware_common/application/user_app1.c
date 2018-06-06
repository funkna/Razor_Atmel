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

static u32 u32Time = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


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
  LedOff(GREEN);
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

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
void UserApp1MorseShort(LedNumberType eLED_)
{
  while(u32Time <= 80000)
  {
    u32Time++;
    LedOn(eLED_);
  }
  u32Time = 0;
  LedOff(eLED_);
}
void UserApp1MorseLong(LedNumberType eLED_)
{
  while(u32Time <= 400000)
  {
    u32Time++;
    LedOn(eLED_);
  }
  u32Time = 0;
  LedOff(eLED_);
}
void UserApp1MorsePauseShort(void)
{
  while(u32Time <= 800000)
  {
    u32Time++;
  }
  u32Time = 0;
}
void UserApp1MorsePauseLong(void)
{
  while(u32Time <= 4000000)
  {
    u32Time++;
  }
  u32Time = 0;
}

void UserApp1MorseLetter(LedNumberType eLED_, u8 u8Letter)
{
  if(u8Letter == 'A')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'B')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();

  }
  if(u8Letter == 'C')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'D')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'E')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  
  if(u8Letter == 'F')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'G')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'H')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'I')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'J')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'K')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'L')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'M')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'N')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'O')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'P')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'Q')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ();
  }
  if(u8Letter == 'R')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'S')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'T')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'U')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'V')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'W')
  {
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'X')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'Y')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseLong();
  }
  if(u8Letter == 'Z')
  {
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseLong(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseShort();
    UserApp1MorseShort(eLED_);
    UserApp1MorsePauseLong();
  }
}
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  UserApp1MorseLetter(WHITE, 'M');
  UserApp1MorseLetter(PURPLE, 'O');
  UserApp1MorseLetter(BLUE, 'B');
  UserApp1MorseLetter(CYAN, 'Y');
  UserApp1MorseLetter(GREEN, 'D');
  UserApp1MorseLetter(YELLOW, 'I');
  UserApp1MorseLetter(ORANGE, 'C');
  UserApp1MorseLetter(RED, 'K');  
} /* end UserApp1SM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
