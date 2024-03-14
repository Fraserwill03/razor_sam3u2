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
#include "stdlib.h"

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
static u8 FindIt_au8Symbols[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
static u8 FindIt_au8Deck[U8_DECK_SIZE][U8_SYMBOLS_PER_CARD];



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
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    FindIt_pfStateMachine = FindItSM_Welcome;
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

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_ShuffleSymbols()

@brief Shuffles the symbols array using a version of the Fisher-Yates shuffle algorithm 
(https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)

*/
void FindIt_ShuffleSymbols()
{
  if (U8_NUM_SYMBOLS > 1) {
        u8 i;
        for (i = 0; i < U8_NUM_SYMBOLS - 1; i++) {
          u8 j = i + rand() / (RAND_MAX / (U8_NUM_SYMBOLS - i) + 1);
          u8 t = FindIt_au8Symbols[j];
          FindIt_au8Symbols[j] = FindIt_au8Symbols[i];
          FindIt_au8Symbols[i] = t;
        }
    }
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_MakeDeck()

@brief uses the shuffled symbol array to create a deck with those symbols

*/
void FindIt_MakeDeck()
{
  for (int i = 0; i < U8_DECK_SIZE; i++) {
        FindIt_ShuffleSymbols();
        for (int j = 0; j < U8_SYMBOLS_PER_CARD; j++) {
            FindIt_au8Deck[i][j] = FindIt_au8Symbols[j];
        }
    }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Welcomes the player to the game */
static void FindItSM_Welcome(void) 
{
  // Message definintions:         01234567890123456789
  static u8 au8BoardWelcomeMessage[] = "Welcome to Find It!!";
  static u8 au8DebugWelcomeMessage[] = "Welcome to Find it!\n";
  static u8 au8DebugInstructions[] = "Please select the number of players\n\r";
  
  static u16 u16WelcomeMessageCounter;
  
  //Print Welcome Message to LCD
  if(u16WelcomeMessageCounter == 0) {
    for(u8 i = 0; i < 20; i++) {
      DebugLineFeed();
    }
    DebugPrintf(au8DebugWelcomeMessage);
    DebugLineFeed();
    LcdMessage(LINE1_START_ADDR, au8BoardWelcomeMessage);
  }
  else if (u16WelcomeMessageCounter == 5000){
    // Setting LCD message with button labels for next state
    LcdMessage(LINE2_START_ADDR, "1                  2");
    // Sending debug message with instruction
    DebugPrintf(au8DebugInstructions);
    DebugLineFeed();
    FindIt_pfStateMachine  = FindItSM_Idle;
  }
  u16WelcomeMessageCounter++;
  
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Waits for a user to choose single or multiplayer game */
static void FindItSM_Idle(void)
{
  //Message definition
                                   //0123456789012345678901234567890123456789
  static u8 au8NumPlayerMessage[] = "Select the number of players below:     ";
  static u8 au8Selection1[] = "1 player mode selected\n\r";
  static u8 au8Selection2[] = "2 player mode selected\n\r";
  
  static u32 u32ScrollTimer;
  u8 au8Temp[41]; 
  static u8 u8ResetIndex = 0;
  static u8 u8Index = 0;
  
    // Scrolling message
    // Sourced from ascii_board_test.c
  if(IsTimeUp(&u32ScrollTimer, 200))
  {
    u32ScrollTimer = G_u32SystemTime1ms;
    au8Temp[40] = NULL;
    u8Index = u8ResetIndex;
    for(u8 i = 0; i < 40; i++)
    { 
      if( u8Index == 40)
      {
        u8Index = 0; 
      }
      au8Temp[u8Index] = au8NumPlayerMessage[i];
      u8Index++;
    }
    
    LcdMessage(LINE1_START_ADDR, au8Temp);
    
    if(u8ResetIndex == 0)
    {
      u8ResetIndex = 41;
    }

    u8ResetIndex--;
  }

  // If button0 was pressed go to singl player state
  if(WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    LcdCommand(LCD_CLEAR_CMD);
    DebugPrintf(au8Selection1);
    FindIt_pfStateMachine = FindItSM_SinglePlayer;
  } 
  // If button3 was pressed go to player select state
  if(WasButtonPressed(BUTTON3)) {
    ButtonAcknowledge(BUTTON3);
    LcdCommand(LCD_CLEAR_CMD);
    DebugPrintf(au8Selection2);
    FindIt_pfStateMachine = FindItSM_PlayerSelect;
  }  
  
} /* end FindItSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void FindItSM_Error(void)          
{
  
} /* end FindItSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------*/
static void FindItSM_PlayerSelect(void){}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Initializes the deck & counts down */
static void FindItSM_InitGame(void)          
{
  static u8 u8DeckInitialized;
  
  if(!u8DeckInitialized) {
    srand(G_u32SystemTime1ms);
    FindIt_MakeDeck();    
  }
  else {
    static u32 u32CountDownStart;
    
    if(IsTimeUp(&u32CountDownStart, 3300)) {
      LcdMessage(LINE1_START_ADDR, "GO!");
      FindIt_pfStateMachine = FindItSM_SinglePlayer;
    } else if (IsTimeUp(&u32CountDownStart, 2300)) {
      LcdMessage(LINE1_START_ADDR, "1");
    } else if (IsTimeUp(&u32CountDownStart, 1300)) {
      LcdMessage(LINE1_START_ADDR, "2");
    } else if (IsTimeUp(&u32CountDownStart, 300)) {
      LcdMessage(LINE1_START_ADDR, "3");
    }
  }
} /* end FindItSM_InitGame() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Single player game state */
static void FindItSM_SinglePlayer(void)
{
  //TODO: Make a state that initializes the game and then counts down from 3 to start.
  //TODO: Game state (Obviously)
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
