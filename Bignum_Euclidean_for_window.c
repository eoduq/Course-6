
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HEXLEN 32	//������� hexnum�� ����
#define RHEXLEN 16  //���� hexnum ����¿� �̿�Ǵ� ������ 16����
#define BITLEN 256  //hexnumũ�Ⱑ 32�̹Ƿ� �̸� bit�� �ٲپ��� ���� ���� 32*8=256
#define RBITLEN 128 //�����δ� 16����Ʈ������ �ʿ��ϹǷ� �̿� �ش��ϴ� bit ���� 16*8=128

/*
ū ���� �����ϴ� ����ü�� 16���� ���¿� 2���� ���·� ���尡���ϴ�.
�ִ� �Է� ũ��� 16byte�� ����������, ��� �������� �ʿ�� 32byte�� ���� �� �ְ� �Ͽ���.
2������ �����ϴ� �迭�� NULL���� ����� 256+1�� �迭ũ��� ���Ͽ���.
*/
struct Bignum {
	unsigned int hexnum[HEXLEN];
	char bitnum[BITLEN+1];
};

/*
hexnum�� ����� 16������ bitnum�� 2������ ��ȯ�� �����ϴ� �Լ�.
1BYTE�� 8bit�̹Ƿ� ����Ʈ ������ ����� 1�� &�� �����ϴ� ������� �����Ͽ���.
bitnum�� char �̹Ƿ� +'0'�� ���� char�� �����Ͽ���.
*/
void Hex2Bin(struct Bignum* x) {
	int tmp_hex2bin = 0;//1byte ����
	int result_hex2bin = 0;//tmp_hex2bin�� ����� ����Ʈ�� 1��Ʈ�� �̾Ƴ��� ����

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
binnum�� ����Ǿ� �ִ� �������� hexnum�� �����ϴ� �Լ�
1BYTE�� 8bit�� ������ �����Ͽ���.
*/
void Bin2Hex(struct Bignum* x) {
	int tmp_bin2hex = 0;	//char�� ����Ǿ� �ִ� '1','0'�� int�� �ٲپ� ����
	int result_bin2hex = 0;//tmp_bin2hex�� ����Ǿ� �ִ� bit 8���� ���� byte ó�� ����
	
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
ū���� �����ϴ� �Լ�
16������ �Է¹޾� hexnum�� ������ �� �̸� Hex2Bin�Լ��� bitnum�� �����ϵ��� �Ͽ���. 
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
hexnum ����ϴ� �Լ� 
���� ����Ʈ�� ���� ��µȴ�.
*/
void PrintHex(struct Bignum* x) {
	int i = 0;
	for (i = RHEXLEN-1; i >= 0; i--) {
		printf("%.2X ", x->hexnum[i]);
	}
	printf("\n");
}

/*
bitnum�� ����ϴ� �Լ�
���� digit�� ���� ��µȴ�.
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
�Ű����� 2������ �����ϴ� op1�� op2�� ���� result�� �����ϴ� �Լ�
*/
void AddBinary(char* op1, char* op2, char* result) {

	int carry_addbin = 0; //���� �������� �߻��ϴ� caary����
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
�Ű������� ���޵� 2������ 1��ŭ ������Ű�� �Լ�
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
�Ű������� ���޵� ���ڿ��� 2�� ������ �ٲٴ� �Լ�
1,0�� �ٲ� ���� 1 �������״�.
 */
void NegBinary(char* op) {
	int i = 0;
	//1�� ������ ���� ���·� �ٲ�
	for (i = BITLEN-1; i >= 0; i--) {
		if (op[i] == '0') {
			op[i] = '1';
		}
		else {
			op[i] = '0';
		}

	}
	//���� ����� 1�� ���� ���·� �ٲ�
	IncBinary(op);
}

/*
 �Ű����� op1�� op2�� �� ����� result�� �����ϴ� �Լ�
 op1-op2 ����� op1+(-op2)�� �̿��� ���Ͽ���.
 ��, NegBinary(op2)�� ����� op1�� AddBinary �Լ��� ���� ���Ͽ���.
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
 �Ű����� op1�� op2�� ���� ���� result�� �����ϴ� �Լ�
 �������� Ư���� �̿��Ͽ� 1101*101=1101+110100�� ���� ����

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
 �Ű����� r0/r1�� ���� q�� �����ϰ� r0%r1�� ���� r�� �����ϴ� �Լ�
 ���� r0�� tmp�� ������ ��
 ��ⷯ�� Ư���� �̿��Ͽ� tmp�� ������ �� ������ tmp_mod=tmp_mod-(r1->bitnum)�� ����
 ������ �Ǳ� ���� tmp���� �������̰� ������ ������ Ƚ���� ���� ��
 */
void ModDivision(struct Bignum* r0, struct Bignum* r1, struct Bignum* q, struct Bignum* r) {
	
	char tmp_mod[BITLEN + 1];
	memset(tmp_mod, '0', BITLEN);
	tmp_mod[BITLEN] = '\0';

	memcpy(tmp_mod, r0->bitnum, BITLEN);
	SubBinary(tmp_mod, r1->bitnum, tmp_mod);
	while (tmp_mod[RBITLEN-1] != '1') {

		IncBinary(q->bitnum);//�� ����
		SubBinary(tmp_mod, r1->bitnum, tmp_mod);//tmp_mod=tmp_mod-r1-bitnum

	}

	AddBinary(tmp_mod, r1->bitnum, r->bitnum);//������ tmp_mod�� r1->bitnum�� ���� ������ r�� ����

}

/*
 s�� t�� ����ϴ� �Լ���, ��꿡 �ʿ��� q, x0, x1�� ���� x�� ����ϴ� �Լ�(x�� s or t)
 tmp_calc=x1*q
 x=x0-tmp_calc
 ���� ���� ������� ���
 */
void Cal_s_t(struct Bignum* q, struct Bignum* x0, struct Bignum* x1, struct Bignum* x) {

	char tmp_calc[BITLEN+1];
	memset(tmp_calc, '0', BITLEN);
	tmp_calc[BITLEN] = '\0';
	
	MulBinary(x1->bitnum, q->bitnum, tmp_calc);//tmp_calc=x1*q

	SubBinary(x0->bitnum, tmp_calc, x->bitnum);//x=x0-tmp_calc

}


/*
 �� round�� ���¸� ����ϴ� �Լ�
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
 ������ round�� ������ ���
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
	���� ����� �ʱ�ȭ
	�� ������� ������ ppt�� �־��� �Ͱ� ����
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
	 �ݺ��� ���� ����
	 �ش� ���ǰ� ��ġ �� �������� ��ȭ ������ �����ִ� �ݺ����� ����ǰ� 
	 ��������� ������ ��.
	 */
	char END_COND[BITLEN+1];
	memset(END_COND, '0', BITLEN);
	END_COND[BITLEN] = '\0';

	/*
	 ������ �����ϴ� ���� Ȯ���ϱ� ����
	 �ִ������� 1�� ���� �� �̿��.
	 */
	char Is_Coprime[BITLEN+1];
	memset(Is_Coprime, '0', BITLEN);
	Is_Coprime[0] = '1';
	Is_Coprime[BITLEN] = '\0';

	/*
	a�� b �Է�
	*/
	printf("[The input must be 16bytes: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 a1]\n");
	printf("1st Input: ");
	CreateBignum(&a);
	printf("2nd Input: ");
	CreateBignum(&b);
	printf("\n");

	/*
	 �Է¹��� a,b���
	 */
	printf("a: ");
	PrintHex(&a);
	printf("b: ");
	PrintHex(&b);
	printf("\n");

	/*
	 round1 ������ ����� Bignum ���� �ʱ�ȭ
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
	q, r0, r1, r, s0, s1, s, t0, t1, t�� ���� ���ϴ� ������
	�����ִ� �ݺ���.
	r1.bitnum�� ��� '0'�� �� ����
	*/
	int i = 1;
	while (strncmp(END_COND,r1.bitnum,BITLEN) != 0) {

		printf("[round %d]\n", i);
		/*
		 q�� r ���
		 */
		ModDivision(&r0, &r1, &q, &r);

		/*
		 s,t ���
		 */
		Cal_s_t(&q, &s0, &s1, &s);
		Cal_s_t(&q, &t0, &t1, &t);

		/*
		 �������� ���¸� ���
		 */
		PrintRound(&q, &r0, &r1, &r, &s0, &s1, &s, &t0, &t1, &t);

		/*
		 ���� round �� ������ update
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
	 ���� round ���
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
	 a�� b�� ���μ��̸� ������ �����ϹǷ� ���� ���
	 �׷��� ������ "b has no multiplicative inverse on (mod a)"
	 */
	if (!strncmp(Is_Coprime, r0.bitnum, BITLEN)) {
		PrintHex(&t0);
		/*
		������ ������� +a�� ���� �ٽ� ���
		*/
		if (t0.hexnum[RHEXLEN - 1] == 0xff) {
			struct Bignum pos_inv;//��� ������ �����ϴ� ����ü
			memset(&pos_inv, 0, sizeof(struct Bignum));
			AddBinary(t0.bitnum, a.bitnum, pos_inv.bitnum);//t0+a�� ���� t0�� congruent�� ��� ������ pos_inv�� ���� 
			Bin2Hex(&pos_inv);
			printf("Congruent with : ");
			PrintHex(&pos_inv);
		}
	}
	else {
		printf("b has no multiplicative inverse on (mod a)\n");
	}


}