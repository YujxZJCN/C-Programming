#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
	int first[100] = {0};
	int second[100] = {0};
	int result[100] = {0};
	char ysfuhao = '+';
	int flag = 0; //cishu
	int fuhao = 1;
	int indexNumber = 0;
	int indexCiShu = 0;
	char idx;
	printf("请输入第一个多项式：\n");
	while((idx = getchar()) != '\n') {
		if(idx != 'x') {
			if (flag) {
				if (idx == '+' || idx == '-') {
					flag = 0;
					if (indexCiShu == 0) {
						indexCiShu = 1;
					}
					if (indexNumber == 0) {
						first[indexCiShu] = fuhao;
					} else {
						first[indexCiShu] = indexNumber * fuhao;
					}
					fuhao = 1;
					indexCiShu = 0;
					indexNumber = 0;
					if (idx == '-') {
						fuhao = -1;
					}
				} else {
					indexCiShu *= 10;
					indexCiShu += idx - '0';
				}				
			} else {
				if (idx == '-') {
					fuhao = -1;
				}else {
					indexNumber *= 10;
					indexNumber += idx - '0';
				}
			}
		} else {
			flag = 1;
		}
	}
	if (flag && indexCiShu == 0) {
		indexCiShu = 1;
	}
	
	if (indexNumber == 0) {
		first[indexCiShu] = fuhao;
	} else {
		first[indexCiShu] = indexNumber * fuhao;
	}

	// for(size_t i = 0; i < 100; i++)
	// {
	// 	if(first[i] != 0) {
	// 		printf("%d = %d\n", i, first[i]);
	// 	}
	// }

	flag = 0; //cishu
	fuhao = 1;
	indexNumber = 0;
	indexCiShu = 0;

	printf("请输入运算符号(+或-或*)：\n");
	while((idx = getchar()) != '\n') {
		ysfuhao = idx;
	}

	printf("请输入第二个多项式：\n");

	while((idx = getchar()) != '\n') {
		if(idx != 'x') {
			if (flag) {
				if (idx == '+' || idx == '-') {
					flag = 0;
					if (indexCiShu == 0) {
						indexCiShu = 1;
					}
					if (indexNumber == 0) {
						second[indexCiShu] = fuhao;
					} else {
						second[indexCiShu] = indexNumber * fuhao;
					}
					fuhao = 1;
					indexCiShu = 0;
					indexNumber = 0;
					if (idx == '-') {
						fuhao = -1;
					}
				} else {
					indexCiShu *= 10;
					indexCiShu += idx - '0';
				}				
			} else {
				if (idx == '-') {
					fuhao = -1;
				}else {
					indexNumber *= 10;
					indexNumber += idx - '0';
				}
			}
		} else {
			flag = 1;
		}
	}
	if (flag && indexCiShu == 0) {
		indexCiShu = 1;
	}
	
	if (indexNumber == 0) {
		second[indexCiShu] = fuhao;
	} else {
		second[indexCiShu] = indexNumber * fuhao;
	}

	// for(size_t i = 0; i < 100; i++)
	// {
	// 	if(second[i] != 0) {
	// 		printf("%d = %d\n", i, second[i]);
	// 	}
	// }
	
	printf("运算结果：\n");
	switch (ysfuhao) {
		case '+':
			for(size_t i = 0; i < 100; i++) {
				result[i] = first[i] + second[i];
			}
			break;
		case '-':
			for(size_t i = 0; i < 100; i++) {
				result[i] = first[i] - second[i];
			}
			break;
		case '*':
			for(size_t i = 0; i < 100; i++) {
				for(size_t j = 0; j < 100; j++) {
					result[i + j] += first[i] * second[j];
				}
			}
		default:
			break;
	}

	int firstN = 1;
	for(int i = 99; i >= 2; i--) {
		if (result[i] != 0) {
			if (firstN) {
				if(result[i] == 1) {
					printf("x%d", i);
				} else {
					printf("%dx%d", result[i], i);
				}
				firstN = 0;
			} else {
				if (result[i] >= 0) {
					printf("+");
					if(result[i] == 1) {
						printf("x%d", i);
					} else {
						printf("%dx%d", result[i], i);
					}
				} else {
					if(result[i] == -1) {
						printf("-x%d", i);
					} else {
						printf("%dx%d", result[i], i);
					}
				}
			}
		}
	}

	if (result[1] != 0) {
		if (result[1] > 0) {
			if (firstN) {
				firstN = 0;
				printf("%dx", result[1]);
			} else {
				printf("+");
				printf("%dx", result[1]);
			}
		} else {
			printf("%dx", result[1]);
		}		
	}

	if (result[0] != 0) {
		if (result[0] > 0) {
			if (firstN) {
				printf("%d", result[0]);
			} else {
				printf("+%d", result[0]);
			}
		} else {
			printf("%d", result[0]);
		}
		printf("\n");
	}

	return 0;
}