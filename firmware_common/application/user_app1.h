/*!*********************************************************************************************************************
@file find_it.h                                                                
@brief Header file for find_it

----------------------------------------------------------------------------------------------------------------------
To start a new task using this find_it as a template:
1. Follow the instructions at the top of find_it.c
2. Use ctrl-h to find and replace all instances of "find_it" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "FindIt" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "FIND_IT" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

**********************************************************************************************************************/

#ifndef __FIND_IT_H
#define __FIND_IT_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void FindItInitialize(void);
void FindItRunActiveState(void);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void FindItSM_Welcome(void);
static void FindItSM_Idle(void);    
static void FindItSM_Error(void);
static void FindItSM_PlayerSelect(void);
static void FindItSM_SinglePlayer(void);
static void FindItSM_TwoPlayer(void);


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/*
* From https://www.petercollingridge.co.uk/blog/mathematics-toys-and-games/dobble/
* We can have n number of symbols per card, where n-1 is any prime number.
* From this we can create n^2 - n + 1 cards that each contain only a single 
* symbol in common with any other card.
* We are also limited to using a total of n^2 - n + 1 different symbols, which 
* will each appear n times within the deck.
*/

#define U8_SYMBOLS_PER_CARD (u8) 4
#define U8_DECK_SIZE (u8) (U8_SYMBOLS_PER_CARD * U8_SYMBOLS_PER_CARD - U8_SYMBOLS_PER_CARD + 1)
#define U8_NUM_SYMBOLS (u8) 26

#define U16_ONE_SECOND_COUNTER_PERIOD (u16) 1000

#endif /* __FIND_IT_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
