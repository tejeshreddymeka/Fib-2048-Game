#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>



void getRandomIndex(int boardSize);
int newUser(char name[1000],int boardSize);
void initializeBoard(int boardSize);
void printBoard(int boardSize);
void printInstructions();
int updateBoard(char move,int moves,int boardSize);
int left2right(int boardSize);
int right2left(int boardSize);
int top2bottom(int boardSize);
int bottom2top(int boardSize);
int addFibElements(int *temp,int boardSize);
int isGameOver(int boardSize);
int isGameWon(int boardSize);
int getScore(int boardSize);
void gameZone();
void printLeadershipBoard();
void addUsersFromFile();
void addUsersToFile();
int fileExists(const char *fname);
int readResumeBoard(const char *filename);
int getUserIndex(const char *name,int boardSize);

typedef struct _player {
    char name[100];
    int moves;
    int score;
    int boardSize;
}player;

player users[100];

 struct _index {
    int row;
    int column;
}index;

int no_of_players=0;

int **board;
int fib[24]={0,1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,1597,2584,4181,6765,10946,17711,28657};

int main()
{
    system("color 0b");
    addUsersFromFile();
    printf("\t\tWelcome to the Game....!\n");
    printf("\t    -----------------------------\n\n\n");

    int choice=-1;
    while(1)
    {
        if(choice!=-1)
            system("cls");
        printf("\n\t\t------------ Menu -----------\n");
        printf("\t\t 1.Game \n");
        printf("\t\t 2.Leadership Board\n");
        printf("\t\t 3.Get Instructions\n");
        printf("\t\t 0.Exit the Application\n");
        printf("\t\t-----------------------------\n\n");

        printf(" Enter your choice: ");scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            system("cls");
            gameZone();
            system("pause");
            break;
        case 2:
            system("cls");
            printLeadershipBoard();
            system("pause");
            break;
        case 3:
            system("cls");
            printInstructions();
            system("pause");
            break;
        case 0:
            addUsersToFile();
            exit(0);
        }

    //Quit loop
    }



    return 0;
}
int getUserIndex(const char *name,int boardSize)
{
    int ind;
    for(ind=0;ind<no_of_players;ind++)
    {
        if(strcmp(users[ind].name,name)==0)
        {
            return ind;
        }
    }
}

int readResumeBoard(const char *filename)
{
    FILE *file;
    file = fopen(filename,"r");
    int boardSize;
    fscanf(file,"%d",&boardSize);
   // printf("%d",boardSize);system("pause");
    int row,column;
    board = (int **)malloc(sizeof(int *)*boardSize);
    int ind;
    for(ind=0;ind<boardSize;ind++)
        board[ind] = (int *)malloc(sizeof(int)*boardSize);
    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
            fscanf(file,"%d",&board[row][column]);
         //   printf("%d ",board[row][column]);
        }
     //   printf("\n");
    }
    fclose(file);
    return boardSize;
 //   system("pause");
}
int fileExists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void addUsersToFile()
{
   FILE *records;
   records = fopen("records.txt","w");
   fprintf(records,"%d\n",no_of_players);
   int ind;
   for(ind=0;ind<no_of_players;ind++)
   {
       fprintf(records,"%s %d %d %d\n",users[ind].name,users[ind].boardSize,users[ind].score,users[ind].moves);
   }
   fclose(records);
}
void addUsersFromFile()
{
    FILE *records;
    records = fopen("records.txt","r");
    if(records!=NULL)
    {
        fscanf(records,"%d",&no_of_players);
        int ind;
        for(ind=0;ind<no_of_players;ind++)
        {
            char name[100];
            int boardSize,score,moves;
            fscanf(records,"%s %d %d %d",name,&boardSize,&score,&moves);
            strcpy(users[ind].name,name);
            users[ind].score = score;
            users[ind].boardSize = boardSize;
            users[ind].moves = moves;
        }
    }
    fclose(records);
}
void printLeadershipBoard()
{
    printf("\n\t\t Leadership Board\n");
    printf("\t      -----------------------\n");
    printf("\tname\tboardSize\tscore\tmoves\n");
    printf("\t      -----------------------\n");
    int ind;

    int ind2;
    for(ind2=0;ind2<no_of_players;ind2++)
    {
        for(ind=0;ind<no_of_players;ind++)
        {
            if(users[ind].score<users[ind2].score || (users[ind].score==users[ind2].score && users[ind].moves>users[ind2].moves))
            {
                player temp = users[ind];
                users[ind]=users[ind2];
                users[ind2] = temp;
            }

        }
    }

    for(ind=0;ind<no_of_players;ind++)
    {
        printf("\t%s\t%d\t%d\t%d\n",users[ind].name,users[ind].boardSize,users[ind].score,users[ind].moves);
    }

}
void gameZone()
{
    printf("\t\t Start the Game...\n\n");
    printf(" Enter you Name:");
    char name[100];
    scanf("%s",name);
    char filename[100];
    strcpy(filename,name);
    strcat(filename,".txt");
    int boardSize;
    int userInd;
    if(fileExists(filename))
    {
        printf("\t1.Resume\n");
        printf("\t2.New game\n");
        printf(" [*]Enter your choice:");
        int gameChoice;
        scanf("%d",&gameChoice);
        if(gameChoice==2)
            goto NEWUSER;
        //RESUME
        boardSize = readResumeBoard(filename);
        userInd = getUserIndex(name,boardSize);
        goto RESUME;
    }
    NEWUSER:
    printf(" Enter the board 2:(2X2) 4:(4X4):");

    scanf("%d",&boardSize);
    userInd = newUser(name,boardSize);
    initializeBoard(boardSize);
    RESUME:
    system("cls");
    printf("\n\t\t Play the Game :");

    printBoard(boardSize);
    char quitGame = 'n';
    char move;
    int score=0;
    int moves=0;
    while(quitGame=='n')

    {
        if(isGameOver(boardSize))
        {
            printf("hai");
            users[userInd].moves = moves;
            score = getScore(boardSize);
            users[userInd].score = score;
            printf("\n\t\t[*] Game over ......!\n");break;
            char delFile[100]="del ";
            strcpy(delFile,filename);
            system(delFile);
        }
        if(isGameWon(boardSize))
        {
            printf("\n\n\t[*] Hurry..! You won the Game... :-) ..!");
            printf("\n\n\tCongrats %s ...",users[userInd].name);
            users[userInd].moves = moves;
            score = getScore(boardSize);
            users[userInd].score = score;
            printf("\n\n\tYour score is %d in %d moves...",users[userInd].score,users[userInd].moves);
            break;
        }
        printf("\n\t\tYour move(w,s,a,d) or q[quit]: ");
        fflush(stdin);
        scanf("%c",&move);
        if(move=='q')
        {
            quitGame='y';
            users[userInd].moves = moves;
            score = getScore(boardSize);
            users[userInd].score = score;
            FILE *file;
            char filename2[100];
            strcpy(filename2,users[userInd].name);
            strcat(filename2,".txt");
            //sprintf(filename, "%s.txt",users[userInd].name);
            printf("%s",filename2);
            file = fopen(filename2,"w");

            fprintf(file,"%d\n",boardSize);

            int row,column;
            for(row=0;row<boardSize;row++)
            {
                for(column=0;column<boardSize;column++)
                {
                    fprintf(file,"%d ",board[row][column]);
                }
            }
            fclose(file);
        }

        else if(move!='w' && move!='s' && move!='a' && move!='d' && move!='q')
        {
            printf("\n [*] Enter a valid move [w,s,a,d,q]\n");
        }
        else
        {
            int prevMoves = moves;
            moves = updateBoard(move,moves,boardSize);
            if(prevMoves!=moves)
            {
                getRandomIndex(boardSize);
                board[index.row][index.column]=1;
                system("cls");
                printf("\n\t\t Play the Game :");
                printBoard(boardSize);
            }
            else
            {
                printf("\n\t\tMove not possible...!\n");
            }
        }

    }
    printf("\n");
}
int getScore(int boardSize)
{
    int row=0,column=0,maxScore=0;
    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
                if(board[row][column]>maxScore)maxScore=board[row][column];
        }
    }
    return maxScore;
}
int isGameWon(int boardSize)
{
    int row=0,column=0;
    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
                if(board[row][column]==fib[2*boardSize*boardSize])return 1;
        }
    }
    return 0;
}
int isGameOver(int boardSize)
{
    int haveZero=0,row=0,column=0;
    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
                if(board[row][column]==0){haveZero=1;break;}
        }
        if(haveZero)break;
    }
    if(haveZero)return 0;

    int temp[4]={0};
    //checking rows wise
    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
            temp[column]=board[row][column];
        }
        int isChanged = addFibElements(temp,boardSize);
        //printf("ischanged %d",isChanged);
        if(isChanged)return 0;
    }
    //checking column wise
    for(column=0;column<boardSize;column++)
    {
        for(row=0;row<boardSize;row++)
        {
            temp[row]=board[row][column];
        }
        int isChanged = addFibElements(temp,boardSize);
        if(isChanged)return 0;
    }
    return 1;
}

int addFibElements(int *temp,int boardSize)
{

    //fib[0]=0;fib[1]=1;
    int ind;
    //for(ind=2;ind<=20;ind++)
    //{
    //    printf("%d ",fib[ind]);
    //    fib[ind]=fib[ind-1]+fib[ind-2];
    //}

    for(ind=0;ind<boardSize;ind++)
    {
        if(temp[ind]==0)break;
      int ind2=2;
      while(fib[ind2]!=temp[ind])ind2++;
           // printf("ind2 %d",ind2);
          if(temp[ind+1]==fib[ind2-1] || temp[ind+1]==fib[ind2+1])
          {
              temp[ind]+=temp[ind+1];
              temp[ind+1]=-1;
              break;
          }

    }


    int toAdd=0;
    for(ind=0;ind<boardSize;ind++)
    {
        if(temp[ind]==-1){toAdd=-1;temp[ind]=0;continue;}

        temp[ind+toAdd]=temp[ind];
    }
    if(toAdd==-1)
    {
        temp[boardSize-1]=0;
        return 1;
    }
    return 0;
}

int bottom2top(int boardSize)
{
    int changedColumns=0;

    int column;
    for(column=0;column<boardSize;column++)
    {
        int temp[4]={0},tempLen=0;
        int row;
        for(row=0;row<boardSize;row++)
        {
            if(board[row][column]!=0)
            {
                temp[tempLen]=board[row][column];
                tempLen++;
            }
        }


        int isChanged = addFibElements(temp,boardSize);
        int isSame=1;
        for(row=0;row<boardSize;row++)
        {
            if(temp[row]!=board[row][column])
            {
                isSame=0;break;
            }
        }
        if(isSame==0)
        {
            changedColumns++;
            for(row=0;row<boardSize;row++)
            {
                board[row][column]=temp[row];
            }

        }

    }
    return changedColumns;

}

int top2bottom(int boardSize)
{
    int changedColumns=0;

    int column;
    for(column=0;column<boardSize;column++)
    {
        int temp[4]={0},tempLen=0;
        int row;
        for(row=boardSize-1;row>=0;row--)
        {
            if(board[row][column]!=0)
            {
                temp[tempLen]=board[row][column];
                tempLen++;
            }
        }

        int isChanged = addFibElements(temp,boardSize);

        int isSame=1,tempInd=0;
        for(row=boardSize-1,tempInd=0;row>=0;row--,tempInd++)
        {
            if(temp[tempInd]!=board[row][column])
            {
                isSame=0;break;
            }
        }
        if(isSame==0)
        {
            changedColumns++;
            for(row=boardSize-1,tempInd=0;row>=0;row--,tempInd++)
            {
                board[row][column]=temp[tempInd];
            }

        }
    }
    return changedColumns;

}

int left2right(int boardSize)
{
    int changedRows=0;

    int row;
    for(row=0;row<boardSize;row++)
    {
        int temp[4]={0},tempLen=0;
        int column;
        for(column=boardSize-1;column>=0;column--)
        {
            if(board[row][column]!=0)
            {
                temp[tempLen]=board[row][column];
                tempLen++;
            }
        }

        int isChanged = addFibElements(temp,boardSize);

        int isSame=1,tempInd=0;
        for(column=boardSize-1,tempInd=0;column>=0;column--,tempInd++)
        {
            if(temp[tempInd]!=board[row][column])
            {
                isSame=0;break;
            }
        }
        if(isSame==0)
        {
            changedRows++;
            for(column=boardSize-1,tempInd=0;column>=0;column--,tempInd++)
            {
                board[row][column]=temp[tempInd];
            }

        }
    }
    return changedRows;

}

int right2left(int boardSize)
{
    int changedRows=0;
    int row;
    for(row=0;row<boardSize;row++)
    {
        int temp[4]={0},tempLen=0;
        int column;
        for(column=0;column<boardSize;column++)
        {
            if(board[row][column]!=0)
            {
                temp[tempLen]=board[row][column];
                tempLen++;
            }
        }

        int isChanged = addFibElements(temp,boardSize);

        int isSame=1;
        for(column=0;column<boardSize;column++)
        {
            if(temp[column]!=board[row][column])
            {
                isSame=0;break;
            }
        }
        if(isSame==0)
        {
            changedRows++;
            for(column=0;column<boardSize;column++)
            {
                board[row][column]=temp[column];
            }

        }
    }
    return changedRows;

}

int updateBoard(char move,int moves,int boardSize)
{
    int status=1;
    switch(move)
    {
    case 'w':
        status = bottom2top(boardSize);
        break;
    case 's':
        status = top2bottom(boardSize);
        break;
    case 'a':
        status = right2left(boardSize);
        break;
    case 'd':
        status = left2right(boardSize);
        break;
    default:
        printf("\nCheck whether you typed correct key...!\n");
    }
    if(status!=0)moves++;
    return moves;
}

int newUser(char name[1000],int boardSize)
{

    strcpy(users[no_of_players].name,name);
    users[no_of_players].boardSize=boardSize;
    users[no_of_players].moves=0;
    no_of_players++;
    return no_of_players-1;
}

void getRandomIndex(int boardSize)
{
    int doExist=1;
    while(doExist)
    {
        int row = rand()%boardSize;
        int column = rand()%boardSize;
        if(board[row][column]==0)
        {
            index.column=column;
            index.row = row;
            doExist=0;
        }
    }
}

void initializeBoard(int boardSize)
{
    int row,column;
    board = (int **)malloc(sizeof(int *)*boardSize);
    int ind;
    for(ind=0;ind<boardSize;ind++)
        board[ind] = (int *)malloc(sizeof(int)*boardSize);

    for(row=0;row<boardSize;row++)
    {
        for(column=0;column<boardSize;column++)
        {
            board[row][column]=0;
        }
    }
    getRandomIndex(boardSize);
    board[index.row][index.column]=1;
    getRandomIndex(boardSize);
    board[index.row][index.column]=1;


}

void printBoard(int boardSize)
{
    int row;
    int column;
    int ind=0;
    printf("\n\t\t");
    for(ind=0;ind<boardSize;ind++)printf("----");
    printf("\n");
    for(row=0;row<boardSize;row++)
    {
        printf("\t\t");
        for(column=0;column<boardSize;column++)
        {
            printf("| %d ",board[row][column]);
        }
        printf("|\n");
        printf("\t\t");
        for(ind=0;ind<boardSize;ind++)printf("----");
        printf("\n");
    }
    //printing directions

}

void printInstructions()
{
    printf("\n\t\tInstructions of the Game\n");
    printf("\n\t     -------------------------\n\n");
    printf("\t1. w to move up.\n");
    printf("\t2. s to move down.\n");
    printf("\t3. a to move left.\n");
    printf("\t4. d to move right.\n");
    printf("\t5. q to quit the game.\n");
    printf("\n\n");
}
