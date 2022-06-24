
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HEXLEN 32	//멤버변수 hexnum의 길이
#define RHEXLEN 16  //실제 hexnum 입출력에 이용되는 길이인 16저장
#define BITLEN 256  //hexnum크기가 32이므로 이를 bit로 바꾸었을 때의 길이 32*8=256
#define RBITLEN 128 //실제로는 16바이트까지만 필요하므로 이에 해당하는 bit 길이 16*8=128

/*
큰 수를 저장하는 구조체로 16진수 형태와 2진수 형태로 저장가능하다.
최대 입력 크기는 16byte로 제한하지만, 계산 과정상의 필요로 32byte를 받을 수 있게 하였다.
2진수를 저장하는 배열은 NULL까지 고려해 256+1을 배열크기로 정하였다.
*/
struct Bignum {
	unsigned int hexnum[HEXLEN];
	char bitnum[BITLEN+1];
};

/*
hexnum에 저장된 16진수를 bitnum에 2진수로 변환해 저장하는 함수.
1BYTE는 8bit이므로 쉬프트 연산의 결과를 1과 &해 저장하는 방식으로 구현하였다.
bitnum은 char 이므로 +'0'을 통해 char로 저장하였다.
*/
void Hex2Bin(struct Bignum* x) {
	int tmp_hex2bin = 0;//1byte 저장
	int result_hex2bin = 0;//tmp_hex2bin에 저장된 바이트를 1비트씩 뽑아내어 저장

	int i = 0;
	int j = 0;
	for (i = HEXLEN - 1; i >= 0; i--) {
		tmp_hex2bin = x->hexnum[i];
		for (j = 7; j >= 0; j--) {
			result_hex2bin = tmp_hex2bin >> j & 1;
			x->bitnum[8 * i + j] = result_hex2bin + '0';
		}
	}
}

/*
binnum에 저장되어 있는 이진수를 hexnum에 저장하는 함수
1BYTE인 8bit씩 나누어 진행하였다.
*/
void Bin2Hex(struct Bignum* x) {
	int tmp_bin2hex = 0;	//char로 저장되어 있는 '1','0'을 int로 바꾸어 저장
	int result_bin2hex = 0;//tmp_bin2hex에 저장되어 있는 bit 8개를 더해 byte 처럼 저장
	
	int i = 0, j = 0;
	for (i = HEXLEN - 1; i >= 0; i--) {
		result_bin2hex = 0;
		for (j = 7; j >= 0; j--) {
			tmp_bin2hex = (x->bitnum[8 * i + j] - '0');
			result_bin2hex += tmp_bin2hex << j;
		}
		x->hexnum[i] = result_bin2hex;
	}
}

/*
큰수를 생성하는 함수
16진수를 입력받아 hexnum에 저장한 후 이를 Hex2Bin함수로 bitnum에 저장하도록 하였다. 
*/
void CreateBignum(struct Bignum* x) {
	int i = 0;
	
	for (i = RHEXLEN - 1; i >= 0; i--) {
		scanf_s("%x", &(x->hexnum[i]));
	}
	memset(x->hexnum + RHEXLEN, 0, 16);

	Hex2Bin(x);
}

/*
hexnum 출력하는 함수 
상위 바이트가 먼저 출력된다.
*/
void PrintHex(struct Bignum* x) {
	int i = 0;
	for (i = RHEXLEN-1; i >= 0; i--) {
		printf("%.2X ", x->hexnum[i]);
	}
	printf("\n");
}

/*
bitnum을 출력하는 함수
상위 digit이 먼저 출력된다.
*/
void PrintBin(struct Bignum* x) {
	int i = 0,j=0;
	for (i = RHEXLEN - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			printf("%c", x->bitnum[i * 8 + j]);
		}
		printf("\n");
	}
	printf("\n");
}
/*
매개변수 2진수를 저장하는 op1과 op2의 합을 result에 저장하는 함수
*/
void AddBinary(char* op1, char* op2, char* result) {

	int carry_addbin = 0; //연산 과정에서 발생하는 caary저장
	int tmp_addbin = 0;
	for (int i = 0; i < BITLEN; i++) {
		tmp_addbin = (op1[i] - '0') + (op2[i] - '0') + carry_addbin;
		if (tmp_addbin < 2) {
			carry_addbin = 0;
		}
		else {
			carry_addbin = 1;
		}
		result[i] = (tmp_addbin % 2) + '0';
	}
}

/*
매개변수로 전달된 2진수를 1만큼 증가시키는 함수
*/
void IncBinary(char* op) {
	int tmp_incbin=0;
	int carry_incbin = 1;
	for (int i = 0; i < BITLEN; i++) {
		tmp_incbin = (op[i] - '0') + carry_incbin;
		if (tmp_incbin == 2) {
			carry_incbin = 1;
		}
		else {
			carry_incbin = 0;
		}
		op[i] = (tmp_incbin % 2) + '0';
	}
}

/*
매개변수로 전달된 문자열을 2의 보수로 바꾸는 함수
1,0을 바꾼 다음 1 증가시켰다.
 */
void NegBinary(char* op) {
	int i = 0;
	//1의 보수를 취한 형태로 바꿈
	for (i = BITLEN-1; i >= 0; i--) {
		if (op[i] == '0') {
			op[i] = '1';
		}
		else {
			op[i] = '0';
		}

	}
	//위의 결과에 1을 더한 형태로 바꿈
	IncBinary(op);
}

/*
 매개변수 op1과 op2를 뺀 결과를 result에 저장하는 함수
 op1-op2 결과를 op1+(-op2)를 이용해 구하였다.
 즉, NegBinary(op2)를 결과를 op1과 AddBinary 함수를 통해 더하였다.
 */
void SubBinary(char* op1, char* op2, char* result) {
	
	char tmp_subbin[BITLEN+1];
	memset(tmp_subbin, 0,BITLEN);
	tmp_subbin[BITLEN] = '\0';
	
	strncpy_s(tmp_subbin,sizeof(tmp_subbin), op2, BITLEN);
	NegBinary(tmp_subbin);
	AddBinary(op1, tmp_subbin, result);

}

/*
 매개변수 op1과 op2를 곱한 값을 result에 저장하는 함수
 이진수의 특성을 이용하여 1101*101=1101+110100과 같이 연산

 */
void MulBinary(char* op1, char* op2, char* result) {
	char tmp_mulbin[BITLEN+1];
	memset(result, '0', BITLEN);
	tmp_mulbin[BITLEN] = '\0';

	for (int i = 0; i < RBITLEN; i++) {
		
		memset(tmp_mulbin, '0', BITLEN);

		if (op2[i] == '1') {
			memcpy(tmp_mulbin + i, op1, 128);
			AddBinary(tmp_mulbin, result, result);
			
		}
	}

}

/*
 매개변수 r0/r1의 값은 q에 저장하고 r0%r1의 값은 r에 저장하는 함수
 먼저 r0를 tmp에 복사한 후
 모듈러의 특성을 이용하여 tmp가 음수가 될 때까지 tmp_mod=tmp_mod-(r1->bitnum)을 진행
 음수가 되기 직전 tmp값이 나머지이고 연산을 진행한 횟수가 몫이 됨
 */
void ModDivision(struct Bignum* r0, struct Bignum* r1, struct Bignum* q, struct Bignum* r) {
	
	char tmp_mod[BITLEN + 1];
	memset(tmp_mod, '0', BITLEN);
	tmp_mod[BITLEN] = '\0';

	memcpy(tmp_mod, r0->bitnum, BITLEN);
	SubBinary(tmp_mod, r1->bitnum, tmp_mod);
	while (tmp_mod[RBITLEN-1] != '1') {

		IncBinary(q->bitnum);//몫 증가
		SubBinary(tmp_mod, r1->bitnum, tmp_mod);//tmp_mod=tmp_mod-r1-bitnum

	}

	AddBinary(tmp_mod, r1->bitnum, r->bitnum);//음수인 tmp_mod에 r1->bitnum을 더해 나머지 r을 구함

}

/*
 s나 t를 계산하는 함수로, 계산에 필요한 q, x0, x1을 통해 x를 계산하는 함수(x는 s or t)
 tmp_calc=x1*q
 x=x0-tmp_calc
 위와 같은 방식으로 계산
 */
void Cal_s_t(struct Bignum* q, struct Bignum* x0, struct Bignum* x1, struct Bignum* x) {

	char tmp_calc[BITLEN+1];
	memset(tmp_calc, '0', BITLEN);
	tmp_calc[BITLEN] = '\0';
	
	MulBinary(x1->bitnum, q->bitnum, tmp_calc);//tmp_calc=x1*q

	SubBinary(x0->bitnum, tmp_calc, x->bitnum);//x=x0-tmp_calc

}


/*
 각 round의 상태를 출력하는 함수
 */
void PrintRound(struct Bignum* q, struct Bignum* r0, struct Bignum* r1, struct Bignum* r, struct Bignum* s0, struct Bignum* s1, struct Bignum* s, struct Bignum* t0, struct Bignum* t1, struct Bignum* t) {
	Bin2Hex(q);

	Bin2Hex(r0);
	Bin2Hex(r1);
	Bin2Hex(r);

	Bin2Hex(s0);
	Bin2Hex(s1);
	Bin2Hex(s);

	Bin2Hex(t0);
	Bin2Hex(t1);
	Bin2Hex(t);

	printf("\tq  = ");
	PrintHex(q);

	printf("\tr0 = ");
	PrintHex(r0);

	printf("\tr1 = ");
	PrintHex(r1);

	printf("\tr  = ");
	PrintHex(r);

	printf("\ts0 = ");
	PrintHex(s0);

	printf("\ts1 = ");
	PrintHex(s1);

	printf("\ts  = ");
	PrintHex(s);

	printf("\tt0 = ");
	PrintHex(t0);

	printf("\tt1 = ");
	PrintHex(t1);

	printf("\tt  = ");
	PrintHex(t);

}

/*
 마지막 round의 변숫값 출력
 */
void PrintLastRound(struct Bignum* q, struct Bignum* r0, struct Bignum* r1, struct Bignum* r, struct Bignum* s0, struct Bignum* s1, struct Bignum* s, struct Bignum* t0, struct Bignum* t1, struct Bignum* t) {
	Bin2Hex(q);
	Bin2Hex(r0);
	Bin2Hex(r1);
	Bin2Hex(r);
	Bin2Hex(s0);
	Bin2Hex(s1);
	Bin2Hex(s);
	Bin2Hex(t0);
	Bin2Hex(t1);
	Bin2Hex(t);

	printf("\tq  = ");
	printf("N/A\n");

	printf("\tr0 = ");
	PrintHex(r0);
	printf("\tr1 = ");
	PrintHex(r1);
	printf("\tr  = ");
	printf("N/A\n");

	printf("\ts0 = ");
	PrintHex(s0);
	printf("\ts1 = ");
	PrintHex(s1);
	printf("\ts  = ");
	printf("N/A\n");

	printf("\tt0 = ");
	PrintHex(t0);
	printf("\tt1 = ");
	PrintHex(t1);
	printf("\tt  = ");
	printf("N/A\n");

}

int main(void) {

	/*
	변수 선언과 초기화
	각 변수명과 역할은 ppt에 주어진 것과 동일
	*/
	struct Bignum a,b,q,r0,r1,r,s0,s1,s,t0,t1,t;
	memset(&a, 0, sizeof(struct Bignum));
	memset(&b, 0, sizeof(struct Bignum));

	memset(&q, 0, sizeof(struct Bignum));

	memset(&r0, 0, sizeof(struct Bignum));
	memset(&r1, 0, sizeof(struct Bignum));
	memset(&r, 0, sizeof(struct Bignum));

	memset(&s0, 0, sizeof(struct Bignum));
	memset(&s1, 0, sizeof(struct Bignum));
	memset(&s, 0, sizeof(struct Bignum));

	memset(&t0, 0, sizeof(struct Bignum));
	memset(&t1, 0, sizeof(struct Bignum));
	memset(&t, 0, sizeof(struct Bignum));

	/*
	 반복문 종료 조건
	 해당 조건과 일치 시 변수들의 변화 과정을 보여주는 반복문이 종료되고 
	 최종결과가 나오게 됨.
	 */
	char END_COND[BITLEN+1];
	memset(END_COND, '0', BITLEN);
	END_COND[BITLEN] = '\0';

	/*
	 역원이 존재하는 지를 확인하기 위해
	 최대공약수를 1과 비교할 때 이용됨.
	 */
	char Is_Coprime[BITLEN+1];
	memset(Is_Coprime, '0', BITLEN);
	Is_Coprime[0] = '1';
	Is_Coprime[BITLEN] = '\0';

	/*
	a와 b 입력
	*/
	printf("[The input must be 16bytes: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 a1]\n");
	printf("1st Input: ");
	CreateBignum(&a);
	printf("2nd Input: ");
	CreateBignum(&b);
	printf("\n");

	/*
	 입력받은 a,b출력
	 */
	printf("a: ");
	PrintHex(&a);
	printf("b: ");
	PrintHex(&b);
	printf("\n");

	/*
	 round1 시작전 선언된 Bignum 변수 초기화
	 */
	memset(q.bitnum, '0', BITLEN);

	memcpy(&r0, &a, sizeof(struct Bignum));
	memcpy(&r1, &b, sizeof(struct Bignum));
	memset(r.bitnum, '0', BITLEN);

	memset(s0.bitnum, '0', BITLEN);
	memset(s1.bitnum, '0', BITLEN);
	memset(s.bitnum, '0', BITLEN);
	IncBinary(s0.bitnum);

	memset(t0.bitnum, '0', BITLEN);
	memset(t1.bitnum, '0', BITLEN);
	memset(t.bitnum, '0', BITLEN);
	IncBinary(t1.bitnum);

	/*
	q, r0, r1, r, s0, s1, s, t0, t1, t의 값이 변하는 과정을
	보여주는 반복문.
	r1.bitnum이 모두 '0'일 때 종료
	*/
	int i = 1;
	while (strncmp(END_COND,r1.bitnum,BITLEN) != 0) {

		printf("[round %d]\n", i);
		/*
		 q와 r 계산
		 */
		ModDivision(&r0, &r1, &q, &r);

		/*
		 s,t 계산
		 */
		Cal_s_t(&q, &s0, &s1, &s);
		Cal_s_t(&q, &t0, &t1, &t);

		/*
		 변수들의 상태를 출력
		 */
		PrintRound(&q, &r0, &r1, &r, &s0, &s1, &s, &t0, &t1, &t);

		/*
		 다음 round 전 변숫값 update
		 */
		memset(q.bitnum, '0', BITLEN);
		memcpy(&r0, &r1, sizeof(struct Bignum));
		memcpy(&r1, &r, sizeof(struct Bignum));
		memcpy(&s0, &s1, sizeof(struct Bignum));
		memcpy(&s1, &s, sizeof(struct Bignum));
		memcpy(&t0, &t1, sizeof(struct Bignum));
		memcpy(&t1, &t, sizeof(struct Bignum));

		i++;

	}

	/*
	 최종 round 출력
	 */
	printf("[round %d]\n", i);
	PrintLastRound(&q, &r0, &r1, &r, &s0, &s1, &s, &t0, &t1, &t);
	printf("[End of rounds]\n\n");

	printf("[Result]\n");
	printf("GCD(a,b): ");
	PrintHex(&r0);
	printf("s: ");
	PrintHex(&s0);
	printf("t: ");
	PrintHex(&t0);
	printf("Inverse of b   : ");
	/*
	 a와 b가 서로소이면 역원이 존재하므로 역원 출력
	 그렇지 않으면 "b has no multiplicative inverse on (mod a)"
	 */
	if (!strncmp(Is_Coprime, r0.bitnum, BITLEN)) {
		PrintHex(&t0);
		/*
		역원이 음수라면 +a한 값을 다시 출력
		*/
		if (t0.hexnum[RHEXLEN - 1] == 0xff) {
			struct Bignum pos_inv;//양수 역원을 저장하는 구조체
			memset(&pos_inv, 0, sizeof(struct Bignum));
			AddBinary(t0.bitnum, a.bitnum, pos_inv.bitnum);//t0+a를 통해 t0와 congruent한 양수 역원을 pos_inv에 저장 
			Bin2Hex(&pos_inv);
			printf("Congruent with : ");
			PrintHex(&pos_inv);
		}
	}
	else {
		printf("b has no multiplicative inverse on (mod a)\n");
	}


}