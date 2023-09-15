// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#include "Math/UnrealMathUtility.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame(); // Initializing

    PrintLine(TEXT("The numb of possible words is %i"), Words.Num());
}

void UBullCowCartridge:: SetupGame()
{
    // Welcome the player
    PrintLine(TEXT("Welcome to Space Bull"));

    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)]; // Set Hiddenword randomly
    Health = HiddenWord.Len(); // Set health
    GameOver = false;

    PrintLine(TEXT("Guess the %i letter word!!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives "), Health);
    PrintLine(TEXT("Type in your guess and \n press enter to continue..."));
    PrintLine(TEXT("The hidden word is: %s."), *HiddenWord); // Debug line    
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (!GameOver)
    {
        if (Guess == HiddenWord) // VICTORY
        {
            PrintLine(TEXT("You have won!"));
            EndGame();
            return;
        }

        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The hidden word is %i characters long, try again!"), HiddenWord.Len());
            PrintLine(TEXT("Sorry, try guessing again!\nYou have %i lives remaining."), Health);
            return;
        }

        //Check if Isogram
        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeating letters, guess again"));
            return;
        }

        // Remove life
        PrintLine(TEXT("Lost health"));
        --Health;

        if (Health <= 0)
        {
            ClearScreen();
            PrintLine(TEXT("You have no health left"));
            PrintLine(TEXT("The hidden word was: %s."), *HiddenWord); // Debug line

            EndGame();
            return;
        }

        // Show the player Bulls and cows
        GetBullsCows(Guess);
        PrintLine(TEXT("Guess again, you have %i lifes left."), Health);        
    }

    else
    {
        SetupGame();
    }
}

void UBullCowCartridge::EndGame()
{
    GameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //if (GameOver)
    //{
    //    ClearScreen();
    //    SetupGame();
    //}

    //if (Input == "0")
    //{
    //    SetupGame();
    //}

    ClearScreen();
    ProcessGuess(Input);
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                //PrintLine(TEXT("%c"), Word[i]);
                return false;
            }
        }
    }

    return true;
}

void UBullCowCartridge::GetBullsCows(FString Guess) const
{
    int32 BullCount = 0;    //  Letters correct in the right position.
    int32 CowCount = 0;     //  Letters correct but in the wrong position.

    // For every index Guess is same as index Hidden, BullCount++
    // if not a bull, was it a cow? if yes, CowCount++

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])      // Bull
        {
            //PrintLine(TEXT("%c"), Guess[Index]);
            BullCount++;
        }
        else                                        // Cow
        {
            for (int32 j = 0; j < Guess.Len(); j++)
            {
                if (Guess[i] == HiddenWord[j])
                {
                    //PrintLine(TEXT("%c"), Guess[Index]);
                    CowCount++;
                }
            }
        }
    }

    PrintLine(TEXT("Bulls: %i"), BullCount);
    PrintLine(TEXT("Cows: %i"), CowCount);
}