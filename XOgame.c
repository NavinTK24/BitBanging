#include <stdio.h>

int main() {
    char str[9];
    char gameArray[9];

    printf("Hello World\n");

    printf("Enter: ");
    scanf("%s", str);

    int m=0;
    for(int i=0; i<9; i++) {
        gameArray[i] = str[i];
        printf("%c ",gameArray[i]);
        m++;
        if(m%3 == 0) {
            printf("\n");
        }
    }

    
    int winArray[8][3] = {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {0,3,6},
        {1,4,7},
        {2,5,8},
        {0,4,8},
        {2,4,6}
    };
    
    printf("\n");

    int n =0;
    for(int i=0; i<8; i++) {
        if((gameArray[winArray[i][0]] == 'X') && (gameArray[winArray[i][1]] == 'X') && (gameArray[winArray[i][2]] == 'X')){
            n =1;
            break;
        } 
    }
    for(int i=0; i<8; i++) {
        if((gameArray[winArray[i][0]] == 'O') && (gameArray[winArray[i][1]] == 'O') && (gameArray[winArray[i][2]] == 'O')){
            n =2;
            break;
        } 
    }

    if(n ==1) printf("\nX WON");
    else if(n ==2) printf("\nO WON");
    else printf("\nGAME DRAW");


    
    return 0;
}