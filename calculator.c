#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* transformToPostfiEexpression(char *former);
void push(char *flag, char con);
void pop(char *flag);
void deleteFirst(char *flag);
double calculate(char *expression);
void popCal(char p);

char expression[1000];
char transformedExpression[1000];
int flag = 1;
char resultStack[1000];
int stackNum = 0;
char flagStack[1000];
char flagNum = 0;

double result = 0.0;
int tempNum = 0;

char calculateStack[1000][1000];
int calculateNum = 0;

int main(int argc, char const *argv[])
{
	double result = 0.0;
	scanf("%s", expression);
	// transformedExpression = transformToPostfiEexpression(expression);
	printf("%s\n", transformToPostfiEexpression(expression));
	printf("%f\n", calculate(transformToPostfiEexpression(expression)));
	// result = calculate(transformedExpression);
	// printf("%f\n", result);
	return 0;
}

double calculate(char *expression) {
	int i = 0;
	int j = 0;
	for(i = 0; i < strlen(expression); i++) {
		if (expression[i] >= '0' && expression[i] <= '9') {
			tempNum *= 10;
			tempNum += expression[i] - '0';
		} else if (expression[i] == ' ') {
			if (tempNum != 0) {
				for (j = calculateNum - 1; j >= 0; j--) {
					strcpy(calculateStack[j + 1], calculateStack[j]);
				}
				sprintf(calculateStack[0], "%d", tempNum);
				calculateNum++;
				tempNum = 0;
			}
			continue;
		} else {
			popCal(expression[i]);
		}
	}
	return atof(calculateStack[0]);
}

void popCal(char p) {
	int i = 1;
	switch (p) {
		case '+':
			sprintf(calculateStack[0], "%f", atof(calculateStack[0]) + atof(calculateStack[1]));
			break;
		case '-':
			sprintf(calculateStack[0], "%f", atof(calculateStack[1]) - atof(calculateStack[0]));
			break;
		case '*':
			sprintf(calculateStack[0], "%f", atof(calculateStack[0]) * atof(calculateStack[1]));
			break;
		case '/':
			sprintf(calculateStack[0], "%f", atof(calculateStack[1]) / atof(calculateStack[0]));
			break;
		default:
			break;
	}
	
	for (i = 1; i < calculateNum - 1; i++) {
		strcpy(calculateStack[i], calculateStack[i + 1]);
	}
	calculateNum--;
}

char* transformToPostfiEexpression(char *former) {
	int i = 0;
	for(i = 0; i < strlen(expression); i++) {
		if (i == 0 && former[0] == '-') {
			resultStack[stackNum++] = former[0];
			continue;
		}
		if (former[i] >= '0' && former[i] <= '9') {
			if (resultStack[stackNum - 1] != ' ' && i != 1 && i != 0 && (resultStack[stackNum - 1] < '0' || resultStack[stackNum - 1] > '9')) {
				resultStack[stackNum++] = ' ';
			}
			resultStack[stackNum++] = former[i];
		} else if (former[i] == '(') {
			push(flagStack, former[i]);
		} else if (former[i] == ')') {
			while(flagStack[0] != '(' && flagNum >= 0) {
				pop(flagStack);
			}
			deleteFirst(flagStack);
		} else {
			resultStack[stackNum++] = ' ';
			if (former[i] == '+' || former[i] == '-') {
				if (flagStack[0] == '(') {
					push(flagStack, former[i]);
				} else {
					while(flagStack[0] != '(' && flagNum > 0) {
						pop(flagStack);
					}
					push(flagStack, former[i]);
				}
			} else if (former[i] == '*' || former[i] == '/') {
				if (flagStack[0] == '(') {
					push(flagStack, former[i]);
				} else {
					if (flagStack[0] == '+' || flagStack[0] == '-') {
						push(flagStack, former[i]);
					} else {
						while((flagStack[0] == '*' || flagStack[0] == '/') && flagNum > 0 && flagStack[0] != '(' ) {
							pop(flagStack);
						}
						push(flagStack, former[i]);
					}
				}
				
			}
		}
		if (i == strlen(expression) - 1) {
			if (resultStack[stackNum - 1] != ' ') {
				resultStack[stackNum++] = ' ';
			}
			while(flagNum > 0) {
				pop(flagStack);
			}
		}
	}
	return resultStack;
}

void push(char *flag, char con) {
	int i = 0;
	for (i = flagNum - 1; i >= 0; i--) {
		flagStack[i + 1] = flagStack[i];
	}
	flagStack[0] = con;
	flagNum++;
}

void pop(char *flag) {
	int i = 0;
	if (resultStack[stackNum - 1] != ' ') {
		resultStack[stackNum++] = ' ';
	}
	resultStack[stackNum++] = flag[0];
	for (i = 0; i < flagNum; i++) {
		flag[i] = flag[i + 1];
	}
	flagNum--;
}

void deleteFirst(char *flag) {
	int i = 0;
	for (i = 0; i < flagNum; i++) {
		flag[i] = flag[i + 1];
	}
	flagNum--;
}





