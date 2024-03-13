/*!*********************************************************************************************************************
@file find_it.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this find_it as a template:
 1. Copy both find_it.c and find_it.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "find_it" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "FindIt" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "FIND_IT" with "YOUR_NEW_TASK_NAME"
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
- void FindItInitialize(void)
- void FindItRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>FindIt"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32FindItFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "FindIt_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type FindIt_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 FindIt_u32Timeout;                           /*!< @brief Timeout counter used across states */
static u8 FindIt_au8WelcomeMessage[] = "Welcome to Find It!!";
static u8 FindIt_au8NumPlayerMessage[] = "Select the number of players below";

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindItInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void FindItInitialize(void)
{
  // Clear the lcd
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE2_START_ADDR, "1");
  LcdMessage(LINE2_END_ADDR, "2");
  LcdMessage(LINE1_START_ADDR, FindIt_au8NumPlayerMessage);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    FindIt_pfStateMachine = FindItSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    FindIt_pfStateMachine = FindItSM_Error;
  }

} /* end FindItInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void FindItRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void FindItRunActiveState(void)
{
  FindIt_pfStateMachine();

} /* end FindItRunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Waits for a user to choose single or multiplayer game */
static void FindItSM_Idle(void)
{
  static u16 counter = 0;
  counter++;
  
  if(counter == 250) {
    counter = 0;
    LcdCommand(LCD_CURSOR_RT_CMD);
  }
#if 0
  // Display Button options
  
  static u8 u8ToggleMessage = 0;
  static u16 u16Counter = 0;
  
  // Reset counter every second and toggle message
  if(u16Counter == U16_ONE_SECOND_COUNTER_PERIOD) {
    u16Counter = 0;
    u8ToggleMessage ^= 1;
  }
  
  // Display the welcome and player selection method in 1s intervals
  if(u16Counter == 0) {
    if(u8ToggleMessage == 0) {
      LcdMessage(LINE1_START_ADDR, FindIt_au8WelcomeMessage);
    } else {
      LcdMessage(LINE1_START_ADDR, FindIt_au8NumPlayerMessage);
    }
  }
  
  u16Counter++;
  

  // If button0 was pressed go to shuffling state
  if(WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    //FindIt_pfStateMachine = FindItSM_Shuffle;
  } 
  // If button3 was pressed go to player select state
  if(WasButtonPressed(BUTTON3)) {
    ButtonAcknowledge(BUTTON3);
    //FindIt_pfStateMachine = FindItSM_PlayerSelect;
  }
#endif
  
  
} /* end FindItSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void FindItSM_Error(void)          
{
  
} /* end FindItSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------*/
static void FindItSM_PlayerSelect(void){}
static void FindItSM_Shuffle(void){}

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
