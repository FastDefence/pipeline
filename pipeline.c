#include<stdio.h>
#include<string.h>

typedef unsigned short ins;
  
ins Mem[100]={0};//メモリ空間
ins Reg[16]={0};//レジスタ空間
int IF_ID[2]={0};//IF_IDレジスタ 0,命令,1,PC
int ID_EX[4]={0};//ID_EXレジスタ
int EX_MEM[1]={0};//EX_MEMレジスタ
int MEM_WB[1]={0};//MEM_WBレジスタ
int w1,w2,w3,w4;

enum Opecode{
  LOAD,
  STORE,
  ADD,
  SUB,
  MUL,
  DIV,
  NONE
};

void InstructionFetch();
void InstructionDecode();
void Execution();
void MemoryAccess();
void WriteBack();
void area_disp();
void resistor_disp();

int main(){
  memset(Mem,0,sizeof(Mem));
  memset(Reg,0,sizeof(Reg));
  for(int i=0;i<30;i++){//None命令で埋める
    Mem[i]=24576;
  }
  //メモリとレジスタの値はここから下で初期化する
  Mem[0]=768;
  Mem[1]=4865;
  Mem[30]=12;
  Reg[0]=0;
  Reg[1]=1;
  Reg[2]=2;
  
  for(IF_ID[1]=0;IF_ID[1]<2&&Mem[IF_ID[1]]!=-1;IF_ID[1]++){
    area_disp();
	  InstructionFetch();
	  InstructionDecode();
	  Execution();
	  MemoryAccess();
	  WriteBack();
	  resistor_disp();
    area_disp();
  }
  
  return 0;
}

//IF 命令フェッチ
void InstructionFetch(){
  IF_ID[0]=Mem[IF_ID[1]];
}

//ID 命令解読
void InstructionDecode(){
  enum Opecode ope;
  w1=IF_ID[0] >> 12;
  w2=(IF_ID[0] >> 8) & 0x000f;
  w3=(IF_ID[0] >> 4) & 0x000f;
  w4=IF_ID[0] & 0x000f;
  switch(w1){
    case LOAD:
      ID_EX[0]=w1;
      ID_EX[1]=w2;
      ID_EX[2]=Reg[w3];//rt
      ID_EX[3]=Reg[w4];//const
      break;
    case STORE:
      ID_EX[0]=w1;
      ID_EX[1]=Reg[w2];//rs 値の書き込みに使う
      ID_EX[2]=Reg[w3];//rt
      ID_EX[3]=Reg[w4];//const
      break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      ID_EX[0]=w1;
      ID_EX[1]=Reg[w2];//rs
      ID_EX[2]=Reg[w3];//rt
      ID_EX[3]=w4;
      break;
  }
  printf("10進:%d\n",Mem[IF_ID[1]]);
  printf("8進:%d %d %d %d\n",w1,w2,w3,w4);
}

//EX 処理分岐、分岐に応じた値の計算
void Execution(){
  enum Opecode ope;
  switch(ID_EX[0]){
    case LOAD:
    case STORE:
      EX_MEM[0]=ID_EX[2]+ID_EX[3];
      break;
    case ADD:
      EX_MEM[0]=ID_EX[1]+ID_EX[2];
      break;
    case SUB:
      EX_MEM[0]=ID_EX[1]-ID_EX[2];
      break;
    case MUL:
      EX_MEM[0]=ID_EX[1]*ID_EX[2];
      break;
    case DIV:
      EX_MEM[0]=ID_EX[1]/ID_EX[2];
      break;
  }
}

//Mem メモリ読み書き
void MemoryAccess(){
  enum Opecode ope;
  switch(ID_EX[0]){
    case LOAD:
      MEM_WB[0]=Mem[EX_MEM[0]+30];
      break;
    case STORE:
      Mem[EX_MEM[0]+30]=ID_EX[1];
      break;
  }
}

//WB レジスタ書き込み
void WriteBack(){
  enum Opecode ope;
  switch(ID_EX[0]){
    case LOAD:
      Reg[ID_EX[1]]=MEM_WB[0];
      break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      Reg[ID_EX[3]]=EX_MEM[0];
      break;
  }
}

void area_disp(){//メモリ、レジスタ領域の状態を表示
  printf("=====Memory Data=====\n");
  for(int i=0;i<100;i++){
	if(i==30){
	  printf("\n");
	}
	printf("Memory 0x%03x:%hd\n",i,Mem[i]);
  }
  printf("=====Resistor data=====\n");
  for(int i=0;i<16;i++){
	printf("Resistor 0x%02x:%hd\n",i,Reg[i]);
  }
  printf("\n");
}

void resistor_disp(){//処理結果の表示
  enum Opecode ope;
  switch(ID_EX[0]){
    case LOAD:
      printf("load from Memory:0x%02x=%d to Registor:0x%02x\n",EX_MEM[0]+30,Mem[EX_MEM[0]+30],ID_EX[1]);
      break;
    case STORE:
      printf("store Registor:0x%03x=%d for Memory:0x%03x\n",w2,Reg[w2],EX_MEM[0]+30);
      break;
    case ADD:
      printf("$%03x+$%03x=%x\n",w2,w3,Reg[w4]);
      break;
    case SUB:
      printf("$%03x-$%03x=%x\n",w2,w3,Reg[w4]);
      break;
    case MUL:
      printf("$%03x*$%03x=%x\n",w2,w3,Reg[w4]);
      break;
    case DIV:
      printf("$%03x/$%03x=%x\n",w2,w3,Reg[w4]);
      break;
  }
}