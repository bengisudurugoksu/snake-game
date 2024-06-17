#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_OBS_VALUE 9
typedef struct{
	int row;
	int col;
}point;

typedef struct{
	char type;
	int value;
}block;



//We use malloc to initialize the 3D board.
block*** init_board(){
block*** board = (block***)malloc(10 * sizeof(block**));

     for(int i=0; i<10; i++){
        board[i] = (block**)malloc(10 * sizeof (block*));       
        for (int j = 0; j < 10; j++) {
            board[i][j] = (block*)malloc(10 * sizeof (block));
            board[i][j]->type = 'e';
            board[i][j]->value = 0; 
        }
    }

int x = rand() %10;   //Using rand to set the bait and obstacle cells.
int y = rand() %10;
int a = rand() %10;
int b = rand() %10;

if(x==0 && y==0){  //Avoiding the top-left cell which is occupied for the snake's head in the first move.
x = rand() %10;
y = rand() %10;
}
if(a==0 && b==0){
a = rand() %10;
b = rand() %10;
}

board[a][b][0].type = 'o';
board[a][b][0].value = 1;
board[x][y][0].type = 'b';

return board;  //Returning the board.
}

int draw_board(block*** board, point* snake, int n){  //This function draws the board and returns obstacle_c to use in other functions.
int obstacle_c=0,x=0,flag=0;
printf("Snake's coordinates: %dx, %dy\n", snake[0].row, snake[0].col);
printf("┌");
for (int i = 0; i < 10; i++) {
     printf("──");
}
printf("┐\n");
for(int i=0; i<10; i++){
	printf("│");
	for(int j=0; j<10; j++){
	flag=0;
		for(int x=1;x<n;x++){
	        if(snake[x].col==j && snake[x].row==i){
	        	printf("X ");
	        	flag=1;
	        	}
	        }
 		if (i == snake[0].row && j == snake[0].col) {          
                printf("O ");
               }	    
		else if(board[i][j][0].type == 'e' && flag!=1){
	        printf("  ");	  
		}
		else if(board[i][j][0].type == 'b'){
		printf(". ");
		}
	        else if(board[i][j][0].type == 'o'){
	        printf("%d ", board[i][j][0].value);
	        obstacle_c++;
	        }
	}
	printf("│\n");
}
 printf("└");
 for (int i = 0; i < 10; i++) {
        printf("──");
    }
    printf("┘\n");
printf("\n");
printf("\n");

return obstacle_c;
}

char move(point* snake, int n){  //This function moves the snake's head using switch-case and returns the direction to use in the update function.
char direction;
printf("What is your move?(Legal moves:w-a-s-d): ");
scanf(" %c", &direction);

switch(direction){

case 'w':
if(n>1){
	if(snake[1].row==snake[0].row-1 && snake[1].col==snake[0].col){
	break;
	}
	else{
	snake[0].row--;
	}
}
else{
snake[0].row--;
}
break;

case 'a':
if(n>1){
	if(snake[1].col==snake[0].col-1 && snake[1].row==snake[0].row){
	break;
	}
	else{
	snake[0].col--;
	}
}
else{
snake[0].col--;
}
break;

case 'd':
if(n>1){
	if(snake[1].col==snake[0].col+1 && snake[1].row==snake[0].row){
	break;
	}
	else{
	snake[0].col++;
	}
}
else{
snake[0].col++;
}
break;

case 's':
if(n>1){
	if(snake[1].row==snake[0].row+1 && snake[1].col==snake[0].col){
	break;
	}
	else{
	snake[0].row++;
	}
}
else{
snake[0].row++;
}
break;

default:
printf("Please enter a legal move!!!(w-a-s-d): ");

}

return direction;
}

int check_status(block*** board, point* snake, int n){   //check_status function checks the three conditions and returns 1 if any of them are true.
    for (int i = 1; i < n; i++) {
        if (snake[0].row == snake[i].row && snake[0].col == snake[i].col) {
            printf("\nYou hit yourself!\n");
            return 1;
        }
    }

    if (snake[0].row > 9 || snake[0].row < 0 || snake[0].col > 9 || snake[0].col < 0) {
        printf("\nYou hit the wall!\n");
        return 1;
    }

    int head_row = snake[0].row;
    int head_col = snake[0].col;
    if (board[head_row][head_col][0].type == 'o' && board[head_row][head_col][0].value > n) {
        printf("\nYou hit an obstacle that is larger than your body!\n");
        return 1;
    }

    return 0;


}

int update(point** snake, block*** board, int n, int count, char direction, int obstacle_c){
if(count%5==0){  //Count represents the number of moves to add and obstacle or increase the obstacle height.
	if(obstacle_c==3){  //If the obstacle_c equals to 3,it stops increasing and the heights of the obstacles start increasing.
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				if(board[i][j][0].type=='o'){
					if(board[i][j][0].value<MAX_OBS_VALUE){  //If the height is smaller than 9,the height increases.If it is not the increasing stops.This is to avoid corrupting the boards formatting.
					board[i][j][0].value++;
					}					
				}
			}
		}
	}
	else{
		int x = rand() %10;       //If obstacle_c is smaller than 3, we make a random empty cell an obstacle cell using rand once again.
		int y = rand() %10;
		if(board[x][y][0].type=='e'){
			for(int a=0;a<n;a++){
				if((*snake)[a].col==y && (*snake)[a].row==x){
					x = rand() %10;
					y = rand() %10;
					}
							
				}
			board[x][y][0].type = 'o';
			board[x][y][0].value = 1;
		}
		else{
			x = rand() %10;
			y = rand() %10;
			board[x][y][0].type = 'o';
			board[x][y][0].value = 1;
		}
	}	
}	
for(int i=0;i<10;i++){
	for(int j=0;j<10;j++){
		if(board[i][j][0].type=='b'){  //We check the cells one by one in these loops.If the cell is a b type cell and it encounters the snake's head, the b type cell becomes an e type cell and the snake's size is increased with realloc.
			if((*snake)[0].col==j && (*snake)[0].row==i){
			n++;
			(*snake) = realloc((*snake), (n+1) * sizeof(point));
			board[i][j][0].type = 'e';
			int x = rand() %10;
			int y = rand() %10;  //After those actions we create another random b type cell using the rand function.
			if(board[x][y][0].type=='e'){
						for(int a=0;a<n;a++){
							if((*snake)[a].col==y && (*snake)[a].row==x){
								x = rand() %10;
								y = rand() %10;
							}
							
						}
					board[x][y][0].type = 'b';
					}
					else{
						x = rand() %10;
						y = rand() %10;
						board[x][y][0].type = 'b';
					}
			}
		}
		if(board[i][j][0].type=='e'){ //If the cell is an e type cell we move the snake's body except it's head since we already moved the head in the move function.
			if((*snake)[0].col==j && (*snake)[0].row==i){
   		        for (int i = n-1; i>1; i--){
      		        	(*snake)[i].row = (*snake)[i-1].row;   //Starting from the tail of the snake we move towards the head.We use a for loop and set the last element to the element before it and do this up to the first element.
      		  		(*snake)[i].col = (*snake)[i-1].col;
   		 	}
   		 	
			switch(direction){  //To set the first element's position we use the snake's head's new coordinate's and the direction from the head's move.
			
			case 'w':		
			(*snake)[1].row=(*snake)[0].row+1;  
			(*snake)[1].col=(*snake)[0].col;	
			
			break;
			
			case 'a':
			(*snake)[1].row=(*snake)[0].row;
			(*snake)[1].col=(*snake)[0].col+1;
			
			break;
			
			case'd':
			(*snake)[1].row=(*snake)[0].row;
			(*snake)[1].col=(*snake)[0].col-1;
			
			break;
			
			case 's':
			(*snake)[1].row=(*snake)[0].row-1;
			(*snake)[1].col=(*snake)[0].col;
			
			break;
			}

		}
		}
		if(board[i][j][0].type=='o'){  //If the cell is an o type cell we check if it encountered the snake's head.
			if((*snake)[0].row==i && (*snake)[0].col==j){
				if(board[i][j][0].value<=n){ //If it encountered the snake's head and the height of the obstacle which is shown with board[i][j][0].value is smaller or equal to the to the snake's length the o type cell becomes an e type cell and we create another random cell obstacle cell.
					board[i][j][0].type = 'e'; 
					board[i][j][0].value = 0;
					int x = rand() %10;
					int y = rand() %10;
					if(board[x][y][0].type=='e'){
						for(int a=0;a<n;a++){
							if((*snake)[a].col==y && (*snake)[a].row==x){
								x = rand() %10;
								y = rand() %10;
							}
							
						}
					board[x][y][0].type = 'o';
					board[x][y][0].value = 1;
					}
					else{
						x = rand() %10;
						y = rand() %10;
						board[x][y][0].type = 'o';
						board[x][y][0].value = 1;
					}
				}
			}
		}
	}
}

return n;
}

void play(block*** board){
int n=1,check=0,obstacle_c,count=0;
char direction;
point* snake=NULL;
snake=realloc(snake, sizeof(point)*(n));
snake[0].row=0;
snake[0].col=0;

while(check==0){
	obstacle_c=draw_board(board, snake, n);
	direction=move(snake, n);
	count++;
	check=check_status(board, snake, n);
	if(check==0){
		n=update(&snake,board,n,count,direction,obstacle_c);

	}
	else{
	printf("Game Over!\n");
	printf("\n");
	}		
}
free(snake);
}


int main(){
srand(time(NULL));
printf("Welcome to the Snake Game!\n");
block*** board=init_board();
play(board);

for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        free(board[i][j]);
    }
    free(board[i]);
}
free(board);
return 0;
}











