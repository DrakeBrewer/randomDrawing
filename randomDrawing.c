#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// structure
struct Employee
{
    char employeeId[10];
    char firstName[50];
    char lastName[50];
};

// function declarations
struct Employee shuffle (struct Employee employees[], int length);
void validationCheck (double *field0, double *field1, int *field2, double *field3, int *field4, int numEntrees);
void errorCatchDouble (double *input);
void errorCatchInt (int *input);

int main (void)
{

// variable declarations
    struct Employee employees[501]; 
    int read = 0, records = 0, i, numBigWinners, numMedWinners;
    double bigPrize, medPrize, prizePot;
    char fileName[25], newFileName[25], consent;

// take read file name inputs
    printf("Enter the name of the .csv file you would like to read:\n");
    scanf("%s", fileName);

// read file pointer
    FILE *fp;

// open read file
    fp = fopen(fileName, "r");

// check if read file can be opened
    if (fp == NULL)
    {
        printf("encountered an issue reading the read file.\nProgram Terminating ...\n");
        return 1;
    }



// check if write file can be created
    printf("\nEnter the name you would like to use for your new .csv file:\n");
    scanf("%s", newFileName);

// write file pointer
    FILE *nfp;

// open write file
    nfp = fopen(newFileName, "w");

// check if write file can be opened
    if (nfp == NULL)
    {
        printf("encountered an issue creating the write file.\nProgram Terminating ...\n");
        return 1;
    }


// check if we are at the end of the file
    while(!feof(fp))
    {

// for each field fscanf runs, read will go up by 1
        read = fscanf(fp,
                    "%10[^,],%49[^,],%s\n",
                    employees[records].employeeId,
                    employees[records].firstName,
                    employees[records].lastName);
        

// check if each line has 3 fields
        if (read == 3) records++;

// if not, throw error
        if (read != 3 && !feof(fp))
        {
            printf("file format incorrect.\n");
            return 1;
        }

// check for error reading file
        if (ferror(fp))
        {
            printf("error reading file.\n");
            return 1;
        }

    }

    printf("\n*NOTE: All entries should be real numbers*\n");
// take prize related inputs
    validationCheck (&prizePot, &bigPrize, &numBigWinners, &medPrize, &numMedWinners, records);
    
// calculate small prize based on number of entries
    double smallPrize = (prizePot - ((bigPrize*numBigWinners)+(medPrize*numMedWinners))) / (records - (numBigWinners + numMedWinners)) - 0.01;

// check if user accepts small prize amount
    printf("\nThe small prize was calculated to be $%.2lf based on the given CSV file and prize entries. Is this okay? y/n:\n", smallPrize);
    scanf(" %c", &consent);

    if (smallPrize < 0 && (consent == 'y' || consent == 'n'))
    {
        printf("\nThe small prize was calculated to be less than 0. Are you sure? y/n:\n");
        scanf(" %c", &consent);
    }
    

// catch if input was not "y" or "n"
    while (!(consent == 'n') && !(consent == 'y'))
    {
        printf("\nPlease use a valid entry...");
        printf("\nThe small prize was calculated to be $%.2lf based on the given CSV file and prize entries. Is this okay? y/n:\n", smallPrize);
        scanf(" %c", &consent);

        if (smallPrize < 0 && (consent == 'y'))
        {
            printf("\nThe small prize was calculated to be less than 0. Are you sure? y/n:\n");
            scanf(" %c", &consent);
        }
    
    }

// if input was "n" re-enter values
    while (consent == 'n')
    {

        validationCheck (&prizePot, &bigPrize, &numBigWinners, &medPrize, &numMedWinners, records);

    // calculate small prize based on number of lines and input values for new entries
        double smallPrize = (prizePot - ((bigPrize*numBigWinners) + (medPrize*numMedWinners))) / (records - (numBigWinners + numMedWinners)) - 0.01;

        printf("\nThe small prize was calculated to be $%.2lf based on the given CSV file and prize entries. Is this okay? y/n:\n", smallPrize);
        scanf(" %c", &consent);

        if (smallPrize < 0 && (consent == 'n'))
        {
            printf("\nThe small prize was calculated to be less than 0. Are you sure? y/n:\n");
            scanf(" %c", &consent);
        }
    }

// close file
    fclose(fp);

// number of employees that were read
    printf("\n%d records read from '%s'...\n", records, fileName);
    

// call shuffle function on employees array and choose winners
    shuffle(employees, records);
    printf("Winners chosen...\n");


// print each employee (for testing for full list, replace 20 with records) 
    for (int i = 0; i < records; i++)
    {


// write winners to new csv file
        if (i < numBigWinners)
        {
            fprintf(nfp, "%s,%s,%s,$%.2lf\n", employees[i].employeeId, employees[i].firstName, employees[i].lastName, bigPrize);
        }
        else if (i > (numBigWinners - 1) && i < (numBigWinners + numMedWinners))
        {
            fprintf(nfp, "%s,%s,%s,$%.2lf\n", employees[i].employeeId, employees[i].firstName, employees[i].lastName, medPrize);
        }
        else
        {
            fprintf(nfp, "%s,%s,%s,$%.2lf\n", employees[i].employeeId, employees[i].firstName, employees[i].lastName, smallPrize);
        }
        
    }

// close new file
    fclose(nfp);

    printf("New file '%s' created in currrent directory.\n\n", newFileName);

    return 0;

}



/////////////////////////////FUNCTIONS/////////////////////////////

// shuffle function
struct Employee shuffle (struct Employee employees[], int length)
{

    struct Employee temp;

    srand(time(NULL));

// loop through given array and randomize each index
    for (int i = 0; i < length; i++)
    {
        int swapIndex = rand() % length;
        temp = employees[i];
        employees[i] = employees[swapIndex];
        employees[swapIndex] = temp;
    }

    return *employees;
}

// user entry fields
void validationCheck (double *field0, double *field1, int *field2, double *field3, int *field4, int numEntrees)
{
    printf("\nEnter the total prize pot:\n");
    errorCatchDouble(field0);

    printf("\nEnter the large prize:\n");
    errorCatchDouble(field1);

    printf("\nEnter the number of large winners:\n");
    errorCatchInt(field2);

    printf("\nEnter the medium prize:\n");
    errorCatchDouble(field3);

    printf("\nEnter the number of medium winners:\n");
    errorCatchInt(field4);
}

// check if type double entries are valid
void errorCatchDouble (double *input)
{
    int temp, status;

    status = scanf("%lf", input);
	while(status!=1){
		while((temp=getchar()) != EOF && temp != '\n');
		printf("Invalid input... please enter a number: \n");
		status = scanf("%lf", input);
	}
}

// check if type int entries are valid
void errorCatchInt (int *input)
{
    int temp, status;

    status = scanf("%d", input);
	while(status!=1){
		while((temp=getchar()) != EOF && temp != '\n');
		printf("Invalid input... please enter a number: \n");
		status = scanf("%d", input);
	}
}