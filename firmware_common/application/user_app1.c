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
static u8 FindIt_u8PlayerScore;
static u32 FindIt_u32GameStartTime;
static u32 FindIt_u32GameEndTime;



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
@fn void FindIt_ShuffleArray()

@brief Shuffles the symbols array using a version of the Fisher-Yates shuffle algorithm 
(https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)

*/
void FindIt_ShuffleArray(u8 u8SymbolArr[], u8 u8ArrSize)
{
      for (u8 i = 0; i < u8ArrSize - 1; i++) {
        u8 j = i + rand() / (RAND_MAX / (u8ArrSize - i) + 1);
        u8 t = u8SymbolArr[j];
        u8SymbolArr[j] = u8SymbolArr[i];
        u8SymbolArr[i] = t;
      }
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_ShuffleDeck()

@brief Shuffles the symbols array using a version of the Fisher-Yates shuffle algorithm 
(https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)
Essentially the same algorithm as ShuffleArray(), but shuffles the 2D deck.

*/
void FindIt_shuffleDeck(void) {
    for (u8 i = 0; i < U8_SYMBOLS_PER_CARD; i++) {
        FindIt_ShuffleArray(FindIt_au8Deck[i], U8_SYMBOLS_PER_CARD); // Shuffle each row
    }
    //Shuffle the deck
    for (u8 i = 0; i < U8_DECK_SIZE; i++) {
        u8 j = i + rand() / (RAND_MAX / (U8_DECK_SIZE - i) + 1);
        u8 temp[4];
        for (u8 k = 0; k < U8_SYMBOLS_PER_CARD; k++) {
            temp[k] = FindIt_au8Deck[i][k]; // Swap rows
            FindIt_au8Deck[i][k] = FindIt_au8Deck[j][k];
            FindIt_au8Deck[j][k] = temp[k];
        }
    }
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_MakeDeck()

@brief uses the shuffled symbol array to create a deck with those symbols

*/
void FindIt_MakeDeck(void)
{
  u8 n = U8_SYMBOLS_PER_CARD - 1;

    // Shuffle symbols
    FindIt_ShuffleArray(FindIt_au8Symbols, U8_NUM_SYMBOLS);

    // Add first set of n+1 cards (e.g. 4 cards)
    for (u8 i = 0; i < n + 1; i++) {
        // Add new card with first symbol
        FindIt_au8Deck[i][0] = 1;
        // Add n+1 symbols on the card (e.g. 4 symbols)
        for (u8 j = 0; j < n; j++) {
            FindIt_au8Deck[i][j + 1] = (j + 1) + (i * n) + 1;
        }
    }

    // Add n sets of n cards
    for (u8 i = 0; i < n; i++) {
        for (u8 j = 0; j < n; j++) {
            // Append a new card with 1 symbol
            FindIt_au8Deck[(n + 1) + (i * n) + j][0] = i + 2;
            // Add n symbols on the card (e.g. 7 symbols)
            for (u8 k = 0; k < n; k++) {
                u8 val = (n + 1 + n * k + (i * k + j) % n) + 1;
                FindIt_au8Deck[(n + 1) + (i * n) + j][k + 1] = val;
            }
        }
    }
  
    // Shuffle the entire deck
    FindIt_shuffleDeck();
    FindIt_shuffleDeck();
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_CheckMatch()

@brief Checks if a users guess was correct or not
*/
u8 FindIt_CheckMatch(u8 u8ConsoleIndex, u8 u8PlayerIndex, u8 u8Guess){
  for(u8 i = 0; i < 4; i++) {
    if(FindIt_au8Symbols[FindIt_au8Deck[u8ConsoleIndex][i]] == FindIt_au8Symbols[FindIt_au8Deck[u8PlayerIndex][u8Guess]])
      return 1;
  }
  return 0;
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_HandleCorrect()

@brief Handles a correct guess
*/
void FindIt_HandleCorrect(u8* u8LcdFlag, u32* u32LcdTimer){
  LcdMessage(LINE1_START_ADDR, "      CORRECT!      ");
  FindIt_u8PlayerScore++;
  LedOn(LCD_GREEN);
  LedOff(LCD_RED);
  LedOff(LCD_BLUE);
  *u8LcdFlag = 1;
  *u32LcdTimer = G_u32SystemTime1ms;
}

/*!--------------------------------------------------------------------------------------------------------------------
@fn void FindIt_HandleIncorrect()

@brief Handles an incorrect guess
*/
void FindIt_HandleIncorrect(u8* u8LcdFlag, u32* u32LcdTimer){
  LcdMessage(LINE1_START_ADDR, "     INCORRECT!     ");
  LedOn(LCD_RED);
  LedOff(LCD_GREEN);
  LedOff(LCD_BLUE);
  *u8LcdFlag = 1;
  *u32LcdTimer = G_u32SystemTime1ms;
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
  static u8 au8DebugInstructions[] = "Press the left most button to start!\n\r";
  
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
  else if (u16WelcomeMessageCounter == 3000){
    // Setting LCD message with button labels for next state
    LcdMessage(LINE2_START_ADDR, "START               ");
    // Sending debug message with instruction
    DebugPrintf(au8DebugInstructions);
    DebugLineFeed();
    FindIt_pfStateMachine  = FindItSM_Idle;
    u16WelcomeMessageCounter = 0;
  }
  u16WelcomeMessageCounter++;
  
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Waits for a user to choose single or multiplayer game */
static void FindItSM_Idle(void)
{
  //Message definition
                                   //0123456789012345678901234567890123456789
  static u8 au8NumPlayerMessage[] = "Press the left most button to start:    ";
  static u8 au8Selection1[] = "   Get Ready!       \r";
  
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

  // If button0 was pressed go to single player state
  if(WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    LcdCommand(LCD_CLEAR_CMD);
    for(u8 i = 0; i < 30; i++) {
      DebugLineFeed(); 
    }
    DebugPrintf(au8Selection1);
    FindIt_pfStateMachine = FindItSM_InitGame;
    
    //Reset static vars
    u32ScrollTimer = 0;
    u8ResetIndex = 0;
    u8Index = 0;
    
  } 
  
} /* end FindItSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void FindItSM_Error(void)          
{
  
} /* end FindItSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Initializes the deck & counts down */
static void FindItSM_InitGame(void)          
{
  static u8 u8DeckInitialized;
  static u32 u32CountDownStart;
  static u8 u8RandSeeded;
  
  u8* au8LcdCountDown[4] = {
   //01234567890123456789
    "         3!         ",
    "         2!         ",
    "         1!         ",
    "         GO         "
  };
  
  if(!u8DeckInitialized) {
    if(!u8RandSeeded) {
      srand(G_u32SystemTime1ms);
      u8RandSeeded = 1;
    }
    FindIt_MakeDeck();
    u8DeckInitialized ^= 1;
  }
  else {    
    static u8 u8Index;
    if(IsTimeUp(&u32CountDownStart, 1000)) {
      LcdMessage(LINE1_START_ADDR, au8LcdCountDown[u8Index]);
      u32CountDownStart = G_u32SystemTime1ms;
      
      if(u8Index == 3) {           
        FindIt_pfStateMachine = FindItSM_SinglePlayer;
        u8DeckInitialized = 0;
        u32CountDownStart = 0;   
        
        //Acknowledge any incidental button presses
        ButtonAcknowledge(BUTTON0);
        ButtonAcknowledge(BUTTON1);
        ButtonAcknowledge(BUTTON2);
        ButtonAcknowledge(BUTTON3);
        
        FindIt_u32GameStartTime = G_u32SystemTime1s;
        FindIt_u8PlayerScore = 0;
        u8Index = 0;
        return;
      }
      u8Index++;
    }
  }
} /* end FindItSM_InitGame() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Single player game state */
static void FindItSM_SinglePlayer(void)
{
  static u32 u32LcdTimer;
  static u8 u8LcdColourOn;
  static u8 u8PlayerCardIndex;
  static u8 u8DeckCardIndex = 1;
  static u8 u8RoundNumber = 1;
  
  static u8 messageSent;
  
  // Move to game over state if user went through entire deck
  // OR if 60 seconds have passed since game started
  if((u8RoundNumber > U8_DECK_SIZE - 1) || (G_u32SystemTime1s - FindIt_u32GameStartTime >= 60)) {
    // Set state to GameOver state
    FindIt_pfStateMachine = FindItSM_GameOver;
    
    // Track time to finish
    FindIt_u32GameEndTime = G_u32SystemTime1s;
    
    // Clear Lcd, print to debug, and reset static vars
    LcdCommand(LCD_CLEAR_CMD);
    DebugPrintf("   Game Over!     \r");
    u8PlayerCardIndex = 0;
    u8DeckCardIndex = 1;
    u8RoundNumber = 1;
    messageSent = 0;
    
    //Reset Lcd backlight
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    u8LcdColourOn = 0;
    
    return;
  }    
  
  if(!messageSent) {
    u8 au8DeckCard[] = {
      ' ', ' ', ' ',
      FindIt_au8Symbols[FindIt_au8Deck[u8DeckCardIndex][0]], ' ', ' ',
      FindIt_au8Symbols[FindIt_au8Deck[u8DeckCardIndex][1]], ' ', ' ',
      FindIt_au8Symbols[FindIt_au8Deck[u8DeckCardIndex][2]], ' ', ' ',
      FindIt_au8Symbols[FindIt_au8Deck[u8DeckCardIndex][3]], '\r'
    };
    DebugPrintf(au8DeckCard);
    u8 au8SymbolOne[2] = {FindIt_au8Symbols[FindIt_au8Deck[u8PlayerCardIndex][0]], '\0'};
    u8 au8SymbolTwo[2] = {FindIt_au8Symbols[FindIt_au8Deck[u8PlayerCardIndex][1]], '\0'};
    u8 au8SymbolThree[2] = {FindIt_au8Symbols[FindIt_au8Deck[u8PlayerCardIndex][2]], '\0'};
    u8 au8SymbolFour[2] = {FindIt_au8Symbols[FindIt_au8Deck[u8PlayerCardIndex][3]], '\0'};
    LcdMessage(LINE2_START_ADDR, au8SymbolOne);
    LcdMessage(LINE2_START_ADDR + 6, au8SymbolTwo);
    LcdMessage(LINE2_START_ADDR + 13, au8SymbolThree);
    LcdMessage(LINE2_END_ADDR, au8SymbolFour);
    
    messageSent ^= 1;
  }
  
  if(WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    if(FindIt_CheckMatch(u8DeckCardIndex, u8PlayerCardIndex, 0)) {
      FindIt_HandleCorrect(&u8LcdColourOn, &u32LcdTimer);
    } else {
      FindIt_HandleIncorrect(&u8LcdColourOn, &u32LcdTimer);
    }
    
    u8PlayerCardIndex++;
    u8DeckCardIndex++;
    u8RoundNumber++;
    messageSent^= 1;
  } else if (WasButtonPressed(BUTTON1)) {
    ButtonAcknowledge(BUTTON1);
    if(FindIt_CheckMatch(u8DeckCardIndex, u8PlayerCardIndex, 1)) {
      FindIt_HandleCorrect(&u8LcdColourOn, &u32LcdTimer);
    } else {
      FindIt_HandleIncorrect(&u8LcdColourOn, &u32LcdTimer);
    }
    
    u8PlayerCardIndex++;
    u8DeckCardIndex++;
    u8RoundNumber++;
    messageSent^= 1;
  } else if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
    if(FindIt_CheckMatch(u8DeckCardIndex, u8PlayerCardIndex, 2)) {
      FindIt_HandleCorrect(&u8LcdColourOn, &u32LcdTimer);
    } else {
      FindIt_HandleIncorrect(&u8LcdColourOn, &u32LcdTimer);
    }
    
    u8PlayerCardIndex++;
    u8DeckCardIndex++;
    u8RoundNumber++;
    messageSent^= 1;
  } else if (WasButtonPressed(BUTTON3)) {
    ButtonAcknowledge(BUTTON3);
    if(FindIt_CheckMatch(u8DeckCardIndex, u8PlayerCardIndex, 3)) {
      FindIt_HandleCorrect(&u8LcdColourOn, &u32LcdTimer);
    } else {
      FindIt_HandleIncorrect(&u8LcdColourOn, &u32LcdTimer);
    }
    
    u8PlayerCardIndex++;
    u8DeckCardIndex++;
    u8RoundNumber++;
    messageSent^= 1;
  }
  
    
  if(u8LcdColourOn && IsTimeUp(&u32LcdTimer, 250)){
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    u8LcdColourOn = 0;
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* Game state to display score and ask user to play again */
static void FindItSM_GameOver(void) {
  static u32 u32MessageTimer;
  static u8 u8MessageIndex;
  static u8 u8MessageSent;
  
  //Static Message definitions
                                  //01234567890123456789
  static u8 au8ThankYouMessage[] = "THANK U FOR PLAYING!";
  
  static u8 au8ScoreMessage1[] = "SCORE: ";
  static u8 au8ScoreMessage3[] = " IN ";
  
  static u8 au8PlayAgainMessage[] = "    PLAY AGAIN??    ";
  
  static u8 au8ButtonTitleYes[] = "YES";
  static u8 au8ButtonTitleNo[] = "NO";
  
  if(IsTimeUp(&u32MessageTimer, 2000)) {
    u32MessageTimer = G_u32SystemTime1ms;
    u8MessageSent = 0;
  }
  
  // Cycle through the messages in 1s intervals
  if(!u8MessageSent) {
    if(u8MessageIndex == 0){
      LcdMessage(LINE1_START_ADDR, au8ThankYouMessage);
      u8MessageIndex++;
      u8MessageSent = 1;
    } else if (u8MessageIndex == 1) {
      
      // Caclulating stats and coverting digits to single bytes
      u8 u8ScoreLower = FindIt_u8PlayerScore % 10;
      u8 u8ScoreUpper = FindIt_u8PlayerScore / 10;
      u8 u8TotalScoreLower = (U8_DECK_SIZE - 1) % 10;
      u8 u8TotalScoreUpper = (U8_DECK_SIZE - 1) / 10;
      u8 u8TotalTime = (u8) FindIt_u32GameEndTime - FindIt_u32GameStartTime;
      u8 u8TimeLower = u8TotalTime % 10; 
      u8 u8TimeUpper = u8TotalTime / 10;
      
      //Messages based on these calculations
      u8 au8ScoreMessage2[] = { u8ScoreUpper + '0', u8ScoreLower + '0', '/',
                                u8TotalScoreUpper + '0', u8TotalScoreLower + '0' , '\0'};
      u8 au8ScoreMessage4[] = { u8TimeUpper + '0', u8TimeLower + '0', 's', '!', '\0' };
      
      LcdMessage(LINE1_START_ADDR, au8ScoreMessage1);
      LcdMessage(LINE1_START_ADDR + 7, au8ScoreMessage2);
      LcdMessage(LINE1_START_ADDR + 12, au8ScoreMessage3);
      LcdMessage(LINE1_START_ADDR + 16, au8ScoreMessage4);
      u8MessageIndex++;
      u8MessageSent = 1;
      
    } else if (u8MessageIndex == 2) {
      LcdMessage(LINE1_START_ADDR, au8PlayAgainMessage);
      LcdMessage(LINE2_START_ADDR, au8ButtonTitleYes);
      LcdMessage(LINE2_START_ADDR + 18, au8ButtonTitleNo);
      u8MessageIndex = 0;
      u8MessageSent = 1;
    }
  }
  
 //If button 0 was pressed restart the game
  if(WasButtonPressed(BUTTON0)) {
    u8MessageIndex = 0;
    ButtonAcknowledge(BUTTON0);
    LcdCommand(LCD_CLEAR_CMD);
    FindIt_pfStateMachine = FindItSM_InitGame;
    DebugPrintf("   Get Ready!       \r");
  } 
  
  //If button 3 was pressed go to idle
  if(WasButtonPressed(BUTTON3)) {
    u8MessageIndex = 0;
    ButtonAcknowledge(BUTTON3);
    LcdCommand(LCD_CLEAR_CMD);
    FindIt_pfStateMachine = FindItSM_Welcome;
    LcdMessage(LINE1_START_ADDR, "Welcome to Find It!!");
    DebugPrintf("Welcome to Find It!\r\n\n\n");
  }

}

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
