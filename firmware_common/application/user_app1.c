/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

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

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */
static u8 G_au8Password[10] = {1, 2, 3, 0, 0, 0, 0, 0, 0, 0}; /* Password, init to default */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */ 
bool checkPassword(u8 au8Password[], u8 au8EnteredPassword[])
{
  for(u8 i = 0; i < 10; i++) {
    if(au8EnteredPassword[i] != au8Password[i])
      return FALSE;
  }
  return TRUE;
}

bool enterPassword(u8 au8Password[]) { 
  static bool bBlueOn = FALSE;
  static u8 u8Index = 0;
  static bool bWhiteOn = FALSE;
  
  if(bWhiteOn)
    LedOff(WHITE);
  
  if(u8Index != 10){
    //Check if buttons were pressed
    if(WasButtonPressed(BUTTON0)) {
      ButtonAcknowledge(BUTTON0);
      au8Password[u8Index] = 1;
      u8Index++;
    }
    if(WasButtonPressed(BUTTON1)) {
      ButtonAcknowledge(BUTTON1);
      au8Password[u8Index] = 2;
      u8Index++;
    }
    if(WasButtonPressed(BUTTON2)) {
      ButtonAcknowledge(BUTTON2);
      au8Password[u8Index] = 3;
      u8Index++;
    }
  } 
  else {
    // If password is at max digits blink blue
    if(!bBlueOn) {
      LedBlink(BLUE, LED_2HZ);
      bBlueOn = TRUE;
    }
  }
  
  if(IsButtonHeld(BUTTON0, 2000) && IsButtonHeld(BUTTON1, 2000)){
    LedOff(BLUE);
    bBlueOn = FALSE;
    // Reset password and index
    for(u8 u8i = 0; u8i < 10; u8i++) {
      au8Password[u8i] = 0;
    }
    u8Index = 0;
    LedOn(WHITE);
    bWhiteOn = TRUE;
  }
  //Lastly check if button 3 was pressed
  if(WasButtonPressed(BUTTON3)) {
    ButtonAcknowledge(BUTTON3);
    LedOff(BLUE);
    bBlueOn = FALSE;
    u8Index = 0;
    return TRUE;
  }
  return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  //Initialize all LEDs to off
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(PURPLE);
  LedOff(RED);
  LedOff(BLUE);
  LedOff(WHITE);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = ButtonExercise_StartUp;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
static void ButtonExercise_StartUp(void)
{
  LedOn(YELLOW);
  // 3 second timer
  static u16 u16Counter = 0;
  if(u16Counter < 3000) {
    //Check if button was pressed
    if(WasButtonPressed(BUTTON3)){
      // Acknowledge and set state to settings
      ButtonAcknowledge(BUTTON3);
    
      UserApp1_pfStateMachine = ButtonExercise_Setting;
      LedOff(YELLOW);
    }
    //Increment counter
    u16Counter++;
  }
  // If it has been 3 seconds set state to locked
  else {
    UserApp1_pfStateMachine = ButtonExercise_Locked;
    LedOff(YELLOW);
  }
}

static void ButtonExercise_Setting(void)
{
  // This is so that LedBlink is only called once
  static bool bLedsOn = FALSE;
  if(!bLedsOn) {
    LedBlink(RED, LED_1HZ);
    LedBlink(GREEN, LED_1HZ);
    bLedsOn = TRUE;
  }
  if(enterPassword(G_au8Password)) {
    // if new password was set, set next state to locked
    // And stop yellow light
    UserApp1_pfStateMachine = ButtonExercise_Locked;
    LedOff(RED);
    LedOff(GREEN);
    bLedsOn = FALSE;
  }
}

static void ButtonExercise_Locked(void)
{
  static bool bRedOn = FALSE;
  static u8 au8EnteredPassword[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  if(!bRedOn) {
    LedOn(RED);
    bRedOn = TRUE;
  }
  
  if(enterPassword(au8EnteredPassword)) {
    if(checkPassword(G_au8Password, au8EnteredPassword)){
      LedOff(RED);
      bRedOn = FALSE;
      LedBlink(GREEN, LED_4HZ);
      UserApp1_pfStateMachine = ButtonExercise_Idle;
    }
    else {
      LedOff(RED);
      LedBlink(RED, LED_4HZ);
      bRedOn = FALSE;
      UserApp1_pfStateMachine = ButtonExercise_Idle;
    }
    //Reset entered password
    for(u8 i = 0; i < 10; i++){
      au8EnteredPassword[i] = 0;
    }
  }
}

/* What does this state do? */
static void ButtonExercise_Idle(void)
{       
  static ButtonNameType aeButtons[4] = {BUTTON0, BUTTON1, BUTTON2, BUTTON3};
  for(u8 u8i = 0; u8i < 4; u8i++){
    if(WasButtonPressed(aeButtons[u8i])){
    ButtonAcknowledge(aeButtons[u8i]);
    LedOff(RED);
    LedOff(GREEN);
    UserApp1_pfStateMachine = ButtonExercise_Locked;
    }
  }
} /* end ButtonExercise_Unlocked() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)
{

  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
