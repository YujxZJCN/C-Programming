#include "acllib.h"
#include <stdio.h>
#include <string.h>

#define MLEN 64
void delete(int pos);
void insert(int pos, char letter);

char allWords[10000] = {0};
int lineCount = 1;
int letterCount = 0;
int posOfCaret = 0;

void paint()
{
    beginPaint();
    clearDevice();
    setTextSize(16);
    int i = 1;
    int j = 0;
    char temp[MLEN + 1];
    while(i <= lineCount) {
    	for(j = 64 * (i - 1); j <= 64 * i - 1; j++) {
    		temp[j - 64 * (i - 1)] = allWords[j];
		}
		temp[64] = 0;
    	paintText(15,16*(i),temp);
    	i++;
	}
	setCaretSize(2, 16);
	setCaretPos(15+9*(posOfCaret-64*(posOfCaret/64)),16*(posOfCaret/64 + 1));
	showCaret();
    endPaint();
}

void charEvent(char c)
{
	if((int)c == 13 || (int)c == 8 || (int)c == 37|| (int)c == 39) {
		return; 
	}
	insert(posOfCaret, c);
    ++letterCount;
    posOfCaret++;
    allWords[letterCount] = 0;
    if (letterCount % 64 == 0) {
    	lineCount++;
	}
    paint();
}

void keyboardEvent(int key,int event)
{
	if(key == 37 && !event) {
		if (posOfCaret == 0) {
			return;
		}
    	posOfCaret--;
    	paint();
	}
	if(key == 38 && !event) {
		if (posOfCaret - 64 < 0) {
			return;
		}
    	posOfCaret -= 64;
    	paint();
	}
	if(key == 39 && !event) {
		if (posOfCaret == strlen(allWords)) {
			return;
		}
    	posOfCaret++;
    	paint();
	}
	if(key == 40 && !event) {		
		if (posOfCaret + 64 > strlen(allWords)) {
			return;
		}
    	posOfCaret += 64;
    	paint();
	}
    if(key == 13 && !event) {
    	int i = 0;
    	while(i <= letterCount) {
    		printf("%c", allWords[i]);
    		i++;
    		if (i % 64 == 0) {
    			printf("\n");
			}
		}
		printf("\n");
	}
	if (key == 8 && !event) {
		if (letterCount == 0) {
			return;
		}
		delete(posOfCaret);
		posOfCaret--;
		letterCount--;
		if (letterCount % 63 == 0 && letterCount != 0) {
			lineCount--;
		}
		paint();
	}
}

int Setup()
{
	initConsole();
    initWindow("",DEFAULT,DEFAULT,605,600);

    registerCharEvent(charEvent);
    registerKeyboardEvent(keyboardEvent);
    allWords[0] = 0;
    paint();
    return 0;
}

void delete(int pos) {
	int i = pos - 1;
	if (pos == 1) {
		allWords[0] = 0;
		return;
	}
	for(i; i <= strlen(allWords) - 1; i++) {
		allWords[i] = allWords[i + 1];
	}
}

void insert(int pos, char letter) {
	int i;
	for(i = strlen(allWords); i >= pos; i--) {
		allWords[i + 1] = allWords[i];
	}
	allWords[pos] = letter;
}

