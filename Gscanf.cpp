/*
 * Gscanf.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: tsw
 *
 *  Description:
 *  Gscanf is like scanf but with it you can limit the length. When you press enter you will get you string.
 */


//HOW TO COMPILE:
//Install your NCurses library and later:
// gcc Gscanf.cpp -o gscanf -lncurses

#include <ncurses.h>
#include <string.h>

#include <stdlib.h>

#define ENTER 10
#define SCAPE 27
#define KEY_BACK 263
#define KEY_DEL 330
#define KEY_START 262
#define KEY_INSERT 331

#define TRUE 1
#define FALSE 0

int INSERT_MODE = 0;


char* gscanf(int y, int x , int length);
char* gscanf(int y, int x , int length){

	//char value [length ];
	char* value = (char*)malloc(length + 1);

	char finalValue [length ];

	int key;

	int char_pointer = 0;

	int cursor_pointer = x;

	int MAX_X_CHAR_POINTER = (length + x) - 1;

	noecho();

	//Here we fill our array with white spaces
	//the same with the finalValue array
	for (int i = 0 ; i < length -1 ; i++){
		value[i] = ' ';
		finalValue[i] = ' ';
	}

	//don't forge put the \0 in the last character or you will get garbage
	value[length-1] = '\0';
	finalValue[length-1] = '\0';


	//write in the screen you empty "scanf"
	move(y,x);
	printw("%s" , value);

	//put again the cursor in the first letter.
	move(y,x);


	while (1) {

		//get the key pressed
		key = getch();
		//get the ascii value
		char ascii = key;

		//If the keys are : a to z or A to Z numbers 0 to 9 or a white space
		if ( (key >= 97 && key <= 122) || (key >= 48 && key <= 57) || (key >= 41 && key <= 90)  || key == 32 ){

			//if the pointer is less than the array lenght AND the last position is not a white space
			if ( char_pointer < length - 1 && value[length -2] == ' '  ){

				//if we are not in insert mode
				if( INSERT_MODE == FALSE){

					//when we press a key , the first thing is rotate all
					//character one position to the right. With this we can emulate the same behaviour
					//like a promt terminal
					for ( int i = length - 1  ; i > char_pointer  ; i-- ){
						if ( i > 0){
							value[ i ] = value[ i-1 ];
						}
					}

					//later , where is the pointer , we insert the key pressed
					//and we increment the char_pointer and the cursor_pointer
					value[char_pointer] = ascii;
					char_pointer ++;
					cursor_pointer++;

					//with this we control the extreme cases, the last position.
					//if the char_pointer its in the last , we go back one position
					if ( char_pointer == length - 1 ){
						char_pointer --;
						cursor_pointer--;
					}

				}
				else //if we are in INSERT_MODE
				{
					//we insert the key pressed in the actual char_pointer position
					//and we increment the char_pointer and the cursor_pointer
					value[char_pointer] = ascii;
					char_pointer ++;
					cursor_pointer++;

					//here we control the extreme case like above
					if ( char_pointer == length - 1 ){
						char_pointer --;
						cursor_pointer--;
					}
				}

				//finally we insert the \0 to indicate the last string. The C compiler need it.
				value[length - 1 ] = '\0';

			}

		}
		//KEY INSERT
		else if (key == KEY_INSERT) {

			//here we activate the INSERT MODE
			if ( INSERT_MODE == FALSE ){
				INSERT_MODE = TRUE;
			}
			else{
				INSERT_MODE = FALSE;
			}
		}
		//ROW LEFT
		else if (key == KEY_LEFT) {

			//if whe are inside the array
			//we increment the char_pointer and cursor_pointer
			if ( char_pointer > 0){
				char_pointer--;
				cursor_pointer--;
			}
			else
			{
				//if we are in the left limit , we reset the char_pointer and cursor_pointer
				char_pointer = 0;
				cursor_pointer = x;
			}
		}
		//ROW RIGHT
		else if (key == KEY_RIGHT) {

			//this is the same like above but with right direction
			if ( char_pointer < length -2 ){
				char_pointer++;
				cursor_pointer++;
			}
			else{
				char_pointer = length -2;
				cursor_pointer = MAX_X_CHAR_POINTER - 1 ;
			}
		}
		//REMOVE ( Key above ENTER )
		else if (key == KEY_BACK) {

			//if we are inside the array
			if ( char_pointer > 0  ){

				//we rotate all one position to the left
				for ( int i = char_pointer ; i < length - 1 ; i++){
					value[ i-1 ] = value[ i ];
				}

				//and we control if the last position is not a white space
				//we insert one. If we dont do this , we will have a letters shadow
				if (value[ length -1 ] != ' ' ){
					value[ length -2 ] = ' ';
					value[ length -1 ] = '\0';
				}

				char_pointer--;
				cursor_pointer--;

			}
		}
		//DEL KEY
		else if (key == KEY_DEL) {

			//the del key is a special case.

			//First we get the actual char where is the cursor
			char lastChar = value[char_pointer + 1];

			//later we rotate all one position to the left
			for ( int i = char_pointer ; i < length -1 ; i++){
				value[i] = value[ i + 1 ];
			}

			//later where is the cursor , we insert the last character
			value[char_pointer] = lastChar;

			//and we control if the last position is not a white space
			//we insert one. If we dont do this , we will have a letters shadow
			if (value[ length -1 ] != ' ' ){
				value[ length -2 ] = ' ';
			}

			value[length - 1 ] = '\0';
		}
		else if (key == SCAPE) {
			endwin();
		}
		else if (key == ENTER) {
			//if we press ENTER we return the pointer where is our array
			return value; //Pointer;

			//char* valuePointer;
			//valuePointer = value;
			//return strcpy(finalValue,value);
		}

		//With this keys we only put the cursor on the start or end array
		else if (key == KEY_START) {
			char_pointer = 0;
			cursor_pointer = x;
		}
		else if (key == KEY_END) {
			char_pointer = length - 2;
			cursor_pointer = MAX_X_CHAR_POINTER -1;
		}

		//Finally we write the our actual string on the screen
		move(y,x);
		printw("%s" , value);

		//and later we locate again our cursor pointer
		move(y,cursor_pointer );

		//don't forget refresh ;)
		refresh();

	}
}


int main(){

		//We init ncurses mode
		initscr();

		//we activate the rows keys
		keypad(stdscr, TRUE);

		//clean the screen
		erase();

		//we start the color
		start_color();
		init_pair(1,COLOR_YELLOW , COLOR_RED);
		attron(COLOR_PAIR(1));

		//we prepare our buffer and we call to gscanf(y,x,length)
		int length = 10;

		char final[11];
		char* wordResult = gscanf(10,15, length + 1 );
		strcpy(final , wordResult);

		//when we have the result , we write on the screen
		move(1,1);
		printw("THE RESULT IS %s ", final);
		
		

getch();
		endwin();
}
