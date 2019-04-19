#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "acllib.h"

struct token {
	
	int type;
	int subType;
	
	int isBinary;
	int precedence;
	int leftAssociative;
};

#define G 1.6180339887498948482
#define E 2.7182818284590452354
#define PI 3.14159265358979323846

#define OP_COUNT 10
#define FUNC_COUNT 21
#define STRUCT_H 1

#define OPERATOR 0
#define FUNCTION 1
#define PARENTHESIS 2

char* operators = "+-*/%^$~_!";
char* functions[] = { "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "asinh", "acosh", "atanh", "exp", "floor", "ceil", "round", "log", "ln", "sqrt", "abs", "sgn" };

int isFunction(char* s) {
	
	int i = FUNC_COUNT - 1;
	
	for(; i >= 0 && strcmp(functions[i], s) != 0; i--);
	
	return i;
}

int isOperator(char c) {
	
	int i = OP_COUNT - 1;
	
	for(; i >= 0 && *(operators + i) != c; i--);
	
	return i;
}

double isSymbol(char* s) {
	
	if(strcmp(s, "e") == 0)
		return E;
	
	if(strcmp(s, "pi") == 0)
		return PI;
	
	if(strcmp(s, "inf") == 0)
		return INFINITY;
	
	if(strcmp(s, "g") == 0)
		return G;
	
	return 0.0;
}

double factorial(double d) {
	
	double result = 1;
	
	for(; d > 1; d--)
		result *= d;
	
	return result;
}

double toRadians(double d) {
	
	return d * (PI / 180.0);
}

double toDegrees(double d) {
	
	return d * (180.0 / PI);
}


static int USE_DEGREE = 0;

static int resultCount;
static double* results;

static int opCount;
static struct token* opStack;

static int outCount;
static double* outStack;

static const int binary[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
static const int precedence[] = { 0, 0, 1, 1, 1, 2, 3, 4, 4, 5 };
static const int association[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
char finalResult[1000] = {0};

enum { SIN, COS, TAN, ASIN, ACOS, ATAN, SINH, COSH, TANH, ASINH, ACOSH, ATANH, EXP, FLOOR, CEIL, ROUND, LOG, LN, SQRT, ABS, SGN };

int execute(struct token temp) {
	
	double d1, d2, result;
	
	if(temp.type == OPERATOR) {
		if(temp.isBinary) {
			if(outCount < 2) {
				return 0;
			}
			
			d2 = outStack[--outCount];
			d1 = outStack[--outCount];
			
			switch(temp.subType) {
				case '+':
					result = d1 + d2;
					break;
				case '-':
					result = d1 - d2;
					break;
				case '*':
					result = d1 * d2;
					break;
				case '/':
					result = d1 / d2;
					break;
				case '%':
					result = fmod(d1, d2);
					break;
				case '^':
					result = pow(d1, d2);
					break;
			}
		}
		
		else {
			if(outCount < 1) {
				return 0;
			}
			
			d1 = outStack[--outCount];
			
			switch(temp.subType) {
				case '_':
					result = d1;
					break;
				case '~':
					result = -d1;
					break;
				case '!':
					if(floor(d1) != d1 || d1 < 0.0 || d1 == INFINITY) {
						return 0;
					}
					
					result = factorial(d1);
					break;
				case '$':
					if(floor(d1) != d1 || d1 <= 0 || d1 > resultCount) {
						return 0;
					}
					
					result = results[(int) d1 - 1];
					break;
			}
		}
	}
	
	else {
		if(outCount < 1) {
			return 0;
		}
		
		d1 = outStack[--outCount];
		
		if(temp.subType == SIN)
			result = USE_DEGREE ? sin(toRadians(d1)) : sin(d1);
		
		else if(temp.subType == COS)
			result = USE_DEGREE ? cos(toRadians(d1)) : cos(d1);
		
		else if(temp.subType == TAN)
			result = USE_DEGREE ? tan(toRadians(d1)) : tan(d1);
		
		else if(temp.subType == SINH)
			result = USE_DEGREE ? sinh(toRadians(d1)) : sinh(d1);
		
		else if(temp.subType == COSH)
			result = USE_DEGREE ? cosh(toRadians(d1)) : cosh(d1);
		
		else if(temp.subType == TANH)
			result = USE_DEGREE ? tanh(toRadians(d1)) : tanh(d1);
		
		else if(temp.subType == ASIN)
			result = USE_DEGREE ? toDegrees(asin(d1)) : asin(d1);
		
		else if(temp.subType == ACOS)
			result = USE_DEGREE ? toDegrees(acos(d1)) : acos(d1);
		
		else if(temp.subType == ATAN)
			result = USE_DEGREE ? toDegrees(atan(d1)) : atan(d1);
		
		else if(temp.subType == ASINH)
			result = USE_DEGREE ? toDegrees(asinh(d1)) : asinh(d1);
		
		else if(temp.subType == ACOSH)
			result = USE_DEGREE ? toDegrees(acosh(d1)) : acosh(d1);
		
		else if(temp.subType == ATANH)
			result = USE_DEGREE ? toDegrees(atanh(d1)) : atanh(d1);
		
		else if(temp.subType == LOG)
			result = log10(d1);
		
		else if(temp.subType == LN)
			result = log(d1);
		
		else if(temp.subType == EXP)
			result = exp(d1);
		
		else if(temp.subType == ROUND)
			result = round(d1);
		
		else if(temp.subType == CEIL)
			result = ceil(d1);
		
		else if(temp.subType == FLOOR)
			result = floor(d1);
		
		else if(temp.subType == SQRT)
			result = sqrt(d1);
		
		else if(temp.subType == ABS)
			result = fabs(d1);
		
		else if(temp.subType == SGN)
			result = d1 > 0 ? 1 : (d1 < 0 ? -1 : 0);
	}
	
	outStack[outCount++] = result;
	
	return 1;
}

int emptyOpStack() {
	
	while(opCount != 0) {
		if(opStack[opCount - 1].type == PARENTHESIS) {
			return 0;
		}
		
		if(!execute(opStack[--opCount]))
			return 0;
	}
	
	if(outCount != 1) {
		return 0;
	}
	
	*(results + resultCount++) = outStack[--outCount];
	
	return 1;
}

int shuntYard(char* expr) {
	
	int i;
	char c;
	char* s;
	double d;
	int unary = 1;
	struct token temp;
	
	while((c = tolower(*expr++)) != '\0') {
		
		if(isspace(c))
			continue;
		
		if(isalpha(c)) {
			i = 0;
			s = (char*) malloc((strlen(expr) + 2) * sizeof(char));
			
			if(!s) {
				exit(-2);
			}
			
			do {
				*(s + i++) = c;
			} while(isalpha(c = tolower(*expr++)));
			
			*(s + i) = '\0';
			
			if((i = isFunction(s)) != -1) {
				temp.subType = i;
				temp.precedence = 3;
				temp.type = FUNCTION;
				temp.leftAssociative = 0;
				
				opStack[opCount++] = temp;
				unary = 1;
			}
			
			else if((d = isSymbol(s))) {
				outStack[outCount++] = d;
				unary = 0;
			}
			
			else {
				return 0;
			}
			
			--expr;
			free(s);
		}
		
		else if(isdigit(c) || c == '.') {
			d = strtod(--expr, &s);
			
			if(expr == (char*) s) {
				return 0;
			}
			
			outStack[outCount++] = d;
			unary = 0;
			expr = (char*) s;
		}
		
		else if(c == '(') {
			temp.type = PARENTHESIS;
			
			opStack[opCount++] = temp;
			unary = 1;
		}
		
		else if(c == ')') {
			while(1) {
				if(opCount == 0) {
					return 0;
				}
				
				temp = opStack[--opCount];
				
				if(temp.type == PARENTHESIS)
					break;
				
				if(!execute(temp))
					return 0;
			}
			unary = 0;
		}
		
		else if(c == ',') {
			if(!emptyOpStack())
				return 0;
			unary = 1;
		}
		
		else if((i = isOperator(c)) != -1) {
			if(unary == 1) {
				if(c == '-') {
					c = '~';
					i = 7;
				}
				
				else if(c == '+') {
					c = '_';
					i = 8;
				}
			}
			
			temp.subType = c;
			temp.type = OPERATOR;
			temp.isBinary = binary[i];
			temp.precedence = precedence[i];
			temp.leftAssociative = association[i];
			
			while(opCount != 0 && opStack[opCount - 1].type != PARENTHESIS && temp.precedence < (opStack[opCount - 1].precedence + temp.leftAssociative))
				if(!execute(opStack[--opCount]))
					return 0;
			
			unary = 1;
			opStack[opCount++] = temp;
		}
		
		else {
			return 0;
		}
	}
	
	if(!emptyOpStack())
		return 0;
	for(i = 0; i < resultCount; i++)
		sprintf(finalResult, "%.10G%s", *(results + i), resultCount - i == 1 ? "" : ",");
			
	return 1;
}

void evaluate(char* expr) {
	
	opCount = 0;
	opStack = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	outCount = 0;
	outStack = (double*) malloc(strlen(expr) * sizeof(double));
	
	resultCount = 0;
	results = (double*) malloc(strlen(expr) * sizeof(double));
	
	if(!opStack || !outStack || !results) {
		return;
	}
	shuntYard(expr);
	
	free(opStack);
	free(outStack);
	free(results);
}

int Setup()
{
	initConsole();
    initWindow("",DEFAULT,DEFAULT,800,600);
    int num = 0;
    beginPaint();
    setPenWidth(2);
    line(5, 300, 795, 300);
    line(795, 300, 790, 295);
    line(795, 300, 790, 305);
    line(400, 5, 400, 595);
    line(400, 5, 395, 10);
    line(400, 5, 405, 10);
    for(num = -8; num <= 8; num++) {
    	char chrOfNum[100] = {0};
    	sprintf(chrOfNum, "%d", num);
    	paintText(800*(num+9)/18, 305, chrOfNum);
    	line(800*(num+9)/18, 295, 800*(num+9)/18, 300);
	}
	for(num = -9; num <= 9; num++) {
		if(num == 0) {
			continue;
		}
    	char chrOfNum[100] = {0};
    	sprintf(chrOfNum, "%d", -num*3);
    	paintText(405, 300+num*30, chrOfNum);
    	line(395, 300+num*30, 400, 300+num*30);
	}
    endPaint();
    char expression[1000] = {0};
	char calExpression[1000] = {0};
	double result = 0;
	double formerResult = 0;
	printf("请输入表达式，格式为：2*x+1（请勿省略*）\n");
    scanf("%s", expression);
    beginPaint();
    setTextSize(50);
    paintText(10, 10, "y=");
    paintText(60, 10, expression);
    endPaint();
    double i = 0;
	int j = 0;
    int cnt = 0;
    int idx = 0;
    for(i = -9; i <= 9; i+= 0.1) {
		for(j = 0; j < strlen(expression); j++) {
			if (expression[j] == 'x') {
				int m = 0;
				char n[100] = {0};
				sprintf(n, "%f", i);
				for(m = 0; m < strlen(n); m++) {
					calExpression[cnt++] = n[m];
				}
			} else {
				calExpression[cnt++] = expression[j];
			}
		}
		calExpression[cnt] = 0;
		evaluate(calExpression);
		printf("%s\n", finalResult);
		beginPaint();
		setPenWidth(2);
		line(400+(i-0.1)*400/9, 300+10*(-formerResult), 400+i*400/9, 300+(-atof(finalResult)*10));
		formerResult = atof(finalResult);
		endPaint();
		cnt = 0;
	}
}

