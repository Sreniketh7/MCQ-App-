/*
NAME OF THE APPLICATION : MCQ APPLICATION
DESCRIPTION : THIS APPLICATION IS DEVELOPED TO GIVE AN MCQ (MULTIPLE CHOICE BASED EXAM)
REQUIREMENTS : THIS APPLICATION REQUIRES A TEXT DOCUMENT WHICH SHOULD CONTAIN THE QUESTIONS 
THE ORDER OF THE QUESTIONS IS TO BE FOLLOWED STRICTLY 
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#define FILE_OPEN_READ_MODE     "r"
#define FILE_OPEN_WRITE_MODE    "w"
#define QUE_FILENAME            "questions.txt"
#define QUESTION_END_CHARACTER  '$'
#define TIME_PER_QUES           2U                 //Time allocated for a single question(In minutes)
#define LOWER_CASE              ('A' ^ 'a')
#define TRUE                    1U
#define FALSE                   0U

static char AnswerFileName[32U];

//List of functions 
static unsigned char getNumberOfQuestions(void);
static void GetUserName(void);
static void ExamProcess(unsigned char queLeft, FILE *qFilePtr, FILE *ansFilePtr);
static unsigned char timeElapseMessage(unsigned long ulEndTime);
static int IsAnswerValid(int answer);



static void GetUserName(void)
{
    char *AnswersFile="_answers.txt"; //Points to the answer file
    char UserName[24U];//Get the name from the user's input
    gets(UserName);
    printf("Username is  %s\n",UserName);
    //To name the file in the format Username_answers.txt
    strcat(AnswerFileName,UserName);
    strcat(AnswerFileName,AnswersFile);
}

static unsigned char getNumberOfQuestions(void)
{
	FILE *fQuestionFilePtr = NULL;//Declaring a FILE POINTER (AS IT MUST RETURN A FILE OF QUESTIONS)
	unsigned char NumOfQuestions = 0;//Stores the Total no questions present in the question paper
	char cFileChar = 0;

	//Open file in read mode 
	fQuestionFilePtr = fopen(QUE_FILENAME, FILE_OPEN_READ_MODE);
	
    if(NULL != fQuestionFilePtr)
	{
		//Loop untill end of file is reached
		while((cFileChar = getc(fQuestionFilePtr)) != EOF)
		{
            if(QUESTION_END_CHARACTER == cFileChar)
			{
                ++NumOfQuestions;
            }
		}
		
		//Close file
		fclose(fQuestionFilePtr);

		//Message user
		printf("***********************************\n");
		printf("Number of questions: %d\n", NumOfQuestions);
		printf("Maximum time allocated: %d minute\n", TIME_PER_QUES * (NumOfQuestions));
		printf("***********************************\n");
	}
	else
	{
		printf("Failure: File open\n");
	}
    return NumOfQuestions;
}

static int IsAnswerValid(int answer)
{
    return ((answer | LOWER_CASE)<'a' || (answer | LOWER_CASE)> 'e');
}

static void ExamProcess(unsigned char queLeft, FILE *qFilePtr, FILE *ansFilePtr)
{
	unsigned char ucMarks = 0;
	unsigned char quesCount = 1;
	char cFileChar = 0;
	unsigned long ulStartTime;
	unsigned long ulCurrTime;
	unsigned long ulEndTime = 0;
		
	//Store current time count
	ulStartTime = ulCurrTime = time(NULL);
	
	//Calculate the maximum time allowed
	ulEndTime = ulStartTime + (60 * TIME_PER_QUES * queLeft);	

	//Loop untill end of file is reached
	while((cFileChar = getc(qFilePtr)) != EOF)
	{
		char answer = 0;

		//If character read matches with end of question character
		if(QUESTION_END_CHARACTER == cFileChar)
		{
            queLeft--;
		    if(TRUE == timeElapseMessage(ulEndTime)) 
            break;
            while(1)
            {
            	//Purge the data at input buffers
				fflush(stdin);
				if(kbhit())
				{
					//get user input for the answer option
					answer = getch();
					printf("%c", answer);
					if(TRUE == IsAnswerValid(answer))
					{
						printf("\n\tRE-ENTER ANSWER: ");
					}
		        	fprintf(ansFilePtr, "(%d)\t\t%c", quesCount++, answer);
		        	break;
		    	}
			}
		    if(TRUE == timeElapseMessage(ulEndTime)) break;
			
			cFileChar = getc(qFilePtr);
			
			if((answer | LOWER_CASE) == (cFileChar | LOWER_CASE))
			{
				++ucMarks;
				fprintf(ansFilePtr, "\tRight\n");
			}
			else fprintf(ansFilePtr, "\tWrong\n");
		    
		    if(queLeft)
			{
				ulCurrTime = time(NULL);

				printf("\n\n\t***********************************\n");
				printf("\tQuestions remained: %d\n", queLeft);
				printf("\tTime remained: %d minute ", (ulEndTime - ulCurrTime) / 60);
				printf("%d second\n", (ulEndTime - ulCurrTime) % 60);
				printf("\t***********************************\n");
			}
		}
		else
		{
			printf("%c", cFileChar);
		}
	}

	fprintf(ansFilePtr, "\nMarks obtained are: %d / %d\n", ucMarks, quesCount);
}

static unsigned char timeElapseMessage(unsigned long ulEndTime)
{
	unsigned long ulCurrTime = 0;

    //read current time
	ulCurrTime = time(NULL);
	//compare it with the maximum time allocated for the exam
	if(ulCurrTime > ulEndTime)
	{
		printf("\n\n\t***********************************\n");
		printf("\n\tTime elapsed\n");
		printf("\t***********************************\n");
		
		return TRUE;
	}
	
    return FALSE;
}


int main(void)
{
    FILE *fQuestionFilePtr=NULL;
    FILE *fAnswerFilePtr=NULL;
    unsigned char NoOfQuestions=0;
    unsigned int marks=0;
    //Purge the data at the input buffers
    fflush(stdin);
    //Start of the Application screen at the console
    printf("************MCQ APPLICATION***************\n");
    //To get the total count of the questions 
    NoOfQuestions=getNumberOfQuestions();
    //To get the name of the user and to print the name of the output text file 
    GetUserName();

    fAnswerFilePtr=fopen(AnswerFileName,FILE_OPEN_WRITE_MODE);
    if(NULL==fAnswerFilePtr)
    {
        printf("Failure : Open Answer File\n");
        return 0;
    }
    //Open file in read mode 
	fQuestionFilePtr = fopen(QUE_FILENAME, FILE_OPEN_READ_MODE);
	
    if(NULL !=fQuestionFilePtr)
    {
        ExamProcess(NoOfQuestions,fQuestionFilePtr,fAnswerFilePtr);
        fclose(fQuestionFilePtr);
    }
    else 
     printf("Failure:File Open\n");

    fclose(fAnswerFilePtr);
    return 0;
}



















































