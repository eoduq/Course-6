//
//  main.c
//  Course6_Term_Project
//
//  Created by DD MPR on 2022/05/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 A1

/*
 16바이트 이하 큰 수를 1바이트씩 int 배열 hexnum에 각각 저장하고
 이를 이진수 형태로 변환한 값을 bitnum에 저장.
 이때 이진수는 bit를 각각 char 배열 담음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
 
 */
struct Bignum{
    unsigned int hexnum[16];
    char bitnum[128];
   
    
};

/*
 Bignum구조체에 있는 16진수 정수 배열을 2진수 형태로 char 배열에 담음.
 이때,
 X->hexnum[15]:01 02 03 04 05 06 07 08 09 A0 A1 A2 A3 A4 A5 A6:X->hexnum[0]
 Binarrary[127]:00000001
 00000010
 00000011
 00000100
 00000101
 00000110
 00000111
 00001000
 00001001
 10100000
 10100001
 10100010
 10100011
 10100100
 10100101
 10100110:Binarray[0]
 과 같이 상위 비트가 Binarray배열의 상위 주소에 저장됨.
 */
void Hex2Bin(struct Bignum *X,char *Binarray){
    
    int tmp=0;
    int result=0;
    
    for(int i=15;i>=0;i--){
        tmp=X->hexnum[i];
        for(int j=7;j>=0;j--){
            result=tmp >> j & 1;
            Binarray[8*i+j]=result+'0';
            
        }
        
        
    }
    
}
/*
 전달받은 Binarray(2진수를 char형으로 저장한 배열)를 Bignum의 X->hexnum에 16진수로 저장하는 함수
 Hex2Bin()의 반대기능을 수행
 
 */
void Bin2Hex(char *Binarray,struct Bignum *X){
    int result=0;
    int tmp=0;
    for(int i=15;i>=0;i--){
        result=0;
        for(int j=7;j>=0;j--){
            tmp=(Binarray[8*i+j]-'0');
            result+=tmp<<j;
            
        }
        X->hexnum[i]=result;
    }
}


/*
 입력받은 큰 수를 매개변수로 전달받은 Bignum 포인터가 가르키는 구조체에 저장하는 함수
 이때 0x00112233445566778899AABBCCDDEEFF를 입력 받았다면
 hexnum[0]에 FF가 저장되고 hexnum[15]에 00이 저장됨
 즉 상위 바이트가 배열의 상위 번지에 저장
 */
void CreateBignum(struct Bignum *X){
    
    /*
     16진수 입력받음
     */
    for(int i=15;i>=0;i--){
        scanf("%x",&(X->hexnum[i]));
    }
    /*
     입력받은 16진수를 이진수로 변환해 bitnum에 저장
     */
    Hex2Bin(X, X->bitnum);

}


/*
 전달받은 구조체 포인터가 가르키는 구조체의 hexnum을 출력하는 함수
 */
void PrintHex(struct Bignum *X){
    for(int i=15;i>=0;i--){
        printf("%.2X ",X->hexnum[i]);
    }
    printf("\n");
}

/*
 전달받은 구조체 포인터가 가르키는 구조체의 bitnum을 출력하는 함수
 */
void PrintBin(struct Bignum *X){
    for(int i=15;i>=0;i--){
        for(int j=7;j>=0;j--){
            printf("%c",X->bitnum[i*8+j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 전달받은 2진수 배열인 op1과 op2 덧셈 결과를 result에 저장하는 함수
 상위비트의 합이 마찬가지로 result배열의 상위 주소번지에 저장됨
 */
void AddBinary(char *op1, char *op2, char *result){
    
    int carry=0;
    int temp=0;
    for(int i=0;i<128;i++){
        temp=(op1[i]-'0')+(op2[i]-'0')+carry;
        if(temp<2){
            carry=0;
        }
        else{
            carry=1;
        }
        result[i]=(temp%2)+'0';
    }
}
/*
 전달받은 char형 2진수 배열을 1만큼 증가시키는 함수
 아래에 NegBinary()와 ModDivision()함수에 사용됨
 */
void IncBinary(char *op){
    int tmp1=0;
    int carry=1;
    for(int i=0;i<128;i++){
        tmp1=(op[i]-'0')+carry;
        if(tmp1==2){
            carry=1;
        }
        else{
            carry=0;
        }
        op[i]=(tmp1%2)+'0';
    }
}

/*
 SubBinary()함수에서 이용되는 함수로
 전달받은 2진수 char 배열 op를 2의 보수를 취한 결과로
 바꿈
 */
void NegBinary(char *op){
    
    //1의 보수를 취한 형태로 바꿈
    for(int i=127;i>=0;i--){
        if(op[i]=='0'){
            op[i]='1';
        }
        else{
            op[i]='0';
        }
        
    }
    //위의 결과에 1을 더한 형태로 바꿈
    IncBinary(op);
    
    
    
}
/*
 전달 받은 배열 op1과 op2를 뺀 결과를 result에 저장하는 함수
 op1-op2 결과를 op1+(-op2)를 이용해 구함
 즉, NegBinary(op2)를 결과를 op1과 AddBinary 함수를 통해 더함
 */
void SubBinary(char *op1, char *op2, char *result){
    char tmp[128]={0,};
    strncpy(tmp,op2,128);
    NegBinary(tmp);
    AddBinary(op1, tmp, result);
    
}
/*
 전달받은 char형 2진수 배열 op1과 op2를 곱하는 함수
 계산값은 result에 저장
 이진수의 특성을 이용하여
 1101*101=1101+110100과 같이 연산
 
 */
void MulBinary(char *op1, char *op2, char *result){
    char tmp[256];
    memset(result,'0',128);
    
    for(int i=0;i<128;i++){
        memset(tmp,'0',128);
        
        if(op2[i]=='1'){
            memcpy(tmp+i,op1,128);
            AddBinary(result, tmp, result);
        }
    }
    
}
/*
 전달받은 r0/r1의 값은 q에 저장하고 r0%r1의 값은 r에 저장
 먼저 r0를 tmp에 복사한 후
 모듈러의 특성을 이용하여 tmp가 음수가 될 때까지 tmp-r1을 진행
 음수가 되기 직전 tmp값이 나머지이고 연산을 진행한 횟수가 몫이 됨
 */
void ModDivision(struct Bignum *r0,struct Bignum *r1,struct Bignum *q, struct Bignum *r){
    struct Bignum *tmp=(struct Bignum*)malloc(sizeof(struct Bignum));
    memcpy(tmp, r0,sizeof(struct Bignum));
    SubBinary(tmp->bitnum, r1->bitnum, tmp->bitnum);
    while(tmp->bitnum[127]!='1'){
        
        IncBinary(q->bitnum);
        SubBinary(tmp->bitnum, r1->bitnum, tmp->bitnum);
        
        
    }
    
    AddBinary(tmp->bitnum, r1->bitnum, r->bitnum);
    
}
/*
 s나 t를 계산하는 함수로, 계산에 필요한 q, x0, x1을 통해 x를 계산(x는 s or t)
 temp=x1*q
 x=x0-temp
 위와 같은 방식으로 계산
 */
void Cal_s_t(struct Bignum *q, struct Bignum *x0, struct Bignum *x1, struct Bignum *x){
    
    struct Bignum *temp=(struct Bignum*)malloc(sizeof(struct Bignum));
    memset(temp->bitnum,'0',128);
    MulBinary(x1->bitnum, q->bitnum, temp->bitnum);

    SubBinary(x0->bitnum, temp->bitnum, x->bitnum);

    
    
}
/*
 각 round의 상태를 출력하는 함수
 */
void PrintRound(struct Bignum *q, struct Bignum *r0, struct Bignum *r1, struct Bignum *r, struct Bignum *s0, struct Bignum *s1, struct Bignum *s, struct Bignum *t0, struct Bignum *t1, struct Bignum *t){
    Bin2Hex(q->bitnum, q);
    Bin2Hex(r0->bitnum, r0);
    Bin2Hex(r1->bitnum, r1);
    Bin2Hex(r->bitnum, r);
    Bin2Hex(s0->bitnum, s0);
    Bin2Hex(s1->bitnum, s1);
    Bin2Hex(s->bitnum, s);
    Bin2Hex(t0->bitnum, t0);
    Bin2Hex(t1->bitnum, t1);
    Bin2Hex(t->bitnum, t);
    
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
void PrintLastRound(struct Bignum *q, struct Bignum *r0, struct Bignum *r1, struct Bignum *r, struct Bignum *s0, struct Bignum *s1, struct Bignum *s, struct Bignum *t0, struct Bignum *t1, struct Bignum *t){
    Bin2Hex(q->bitnum, q);
    Bin2Hex(r0->bitnum, r0);
    Bin2Hex(r1->bitnum, r1);
    Bin2Hex(r->bitnum, r);
    Bin2Hex(s0->bitnum, s0);
    Bin2Hex(s1->bitnum, s1);
    Bin2Hex(s->bitnum, s);
    Bin2Hex(t0->bitnum, t0);
    Bin2Hex(t1->bitnum, t1);
    Bin2Hex(t->bitnum, t);
    
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

int main(int argc, const char * argv[]) {
    // insert code here...
    /*
     계산에 사용될 Bignum 포인터 변수들 선언
     각 변수들은 예시에서 해당 변수명을 가진 변수들과 동일한 기능을 함
     */
    struct Bignum *a=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *b=(struct Bignum*)malloc(sizeof(struct Bignum));
    
    struct Bignum *q=(struct Bignum*)malloc(sizeof(struct Bignum));
    
    struct Bignum *r0=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *r1=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *r=(struct Bignum*)malloc(sizeof(struct Bignum));
    
    struct Bignum *s0=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *s1=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *s=(struct Bignum*)malloc(sizeof(struct Bignum));
    
    struct Bignum *t0=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *t1=(struct Bignum*)malloc(sizeof(struct Bignum));
    struct Bignum *t=(struct Bignum*)malloc(sizeof(struct Bignum));
    
    /*
     반복문 종료 조건
     해당 조건과 일치 시 변수들의 변화 과정을 보여주는
     반복문이 종료되고 최종결과가 나오게 됨.
     */
    char END_COND[128];
    memset(END_COND,'0',128);
    /*
     역원이 존재하는 지를 확인하기 위해
     최종 결과로 나온 최대공약수를 1과 비교할 때 이용됨.
     */
    char Is_Coprime[128];
    memset(Is_Coprime,'0',128);
    Is_Coprime[127]='1';
    
    /*
     a와 b입력
     */
    printf("1st Input: ");
    CreateBignum(a);
    printf("2nd Input: ");
    CreateBignum(b);
    printf("\n");
    
    /*
     입력받은 a,b출력
     */
    printf("a: ");
    PrintHex(a);
    printf("b: ");
    PrintHex(b);
    
    printf("\n");
    
    /*
     round1 시작전 선언된 Bignum 구조체 포인터 변수 초기화
     */
    memset(q,'0',sizeof(struct Bignum));
    memcpy(r0,a,sizeof(struct Bignum));
    memcpy(r1,b,sizeof(struct Bignum));
    memset(r,'0',sizeof(r->bitnum));
    memset(s0->bitnum,'0',sizeof(s0->bitnum));
    IncBinary(s0->bitnum);
    memset(s1->bitnum,'0',sizeof(s1->bitnum));
    memset(s->bitnum,'0',sizeof(s1->bitnum));
    memset(t1->bitnum,'0',sizeof(t1->bitnum));
    IncBinary(t1->bitnum);
    memset(t0->bitnum,'0',sizeof(s0->bitnum));
    memset(t->bitnum,'0',sizeof(s0->bitnum));
    
    /*
     q, r0, r1, r, s0, s1, s, t0, t1, t의 값이 변하는 과정을
     보여주는 반복문.
     r1->bitnum이 모두 '0'일 때 종료
     */
    int i=1;
    while(strncmp(r1->bitnum, END_COND,128)!=0){
        
        printf("[round %d]\n",i);
        /*
         q와 r 계산
         */
        ModDivision(r0, r1, q ,r);
        
        /*
         s,t 계산
         */
        Cal_s_t(q, s0, s1, s);
        Cal_s_t(q, t0, t1, t);
        
        /*
         변수들의 상태를 출력
         */
        PrintRound(q, r0, r1, r, s0, s1, s, t0, t1, t);
        
        /*
         다음 round 전 변숫값 update
         */
        memset(q->bitnum,'0',sizeof(struct Bignum));
        memcpy(r0,r1,sizeof(struct Bignum));
        memcpy(r1,r,sizeof(struct Bignum));
        memcpy(s0,s1,sizeof(struct Bignum));
        memcpy(s1,s,sizeof(struct Bignum));
        memcpy(t0,t1,sizeof(struct Bignum));
        memcpy(t1,t,sizeof(struct Bignum));
        
        
        i++;

    }
    
    /*
     최종 round 출력
     */
    printf("[round %d]\n",i);
    PrintLastRound(q, r0, r1, r, s0, s1, s, t0, t1, t);
    
    
    printf("[end of rounds]\n");
    printf("[Result]\n");
    printf("GCD(a,b): ");
    PrintHex(r0);
    printf("s: ");
    PrintHex(s0);
    printf("t: ");
    PrintHex(t0);
    printf("Inverse of b: ");
    /*
     a와 b가 서로소이면 역원이 존재하므로 역원 출력
     그렇지 않으면 "a mod b has no multiplicative inverse"
     */
    if(!strncpy(Is_Coprime,r0->bitnum,128)){
        PrintHex(t0);
    }
    else{
        printf("a mod b has no multiplicative inverse\n");
    }
    
    
    
    
    
    free(a);
    free(b);
    free(q);
    free(r0);
    free(r1);
    free(r);
    free(s0);
    free(s1);
    free(s);
    free(t0);
    free(t1);
    free(t);
    
    return 0;
}
