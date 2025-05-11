#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"
//NOTE:你可以自行定义辅助函数来帮助你实现
typedef struct myCommand {
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]);
}myCommand; 


int func_cmd(int argc, char (*argv)[8]){
	myPrintk(0x07,"cmd\n\0");
    myPrintk(0x07,"help\n\0");
} 

myCommand cmd={"cmd\0","List all command\n\0",func_cmd};

int func_help(int argc, char (*argv)[8]){
    if(argv[1][0]=='h'&&argv[1][1]=='e'&&argv[1][2]=='l'&&argv[1][3]=='p'&&argv[1][4]==0){
        myPrintk(0x07,"show the usage of a command\n\0");
    }
    else if(argv[1][0]=='c'&&argv[1][1]=='m'&&argv[1][2]=='d'&&argv[1][3]==0){
        myPrintk(0x07,"list all commands\n\0");
    }
    else{
        myPrintk(0x07,"Unknown Command ");
        myPrintk(0x07,argv[1]);
        myPrintk(0x07,"\n");
    }
}

myCommand help={"help\0","Usage: help [command]\n\0Display info about [command]\n\0",func_help};


void startShell(void){
//我们通过串口来实现数据的输入
char BUF[256]; //输入缓存区
int BUF_len=0;	//输入缓存区的长度
    
	int argc;
    char argv[8][8];

    do{
        BUF_len=0; 
        myPrintk(0x07,"Student>>\0");
        while((BUF[BUF_len]=uart_get_char())!='\r'){
            uart_put_char(BUF[BUF_len]);//将串口输入的数存入BUF数组中
            BUF_len++;  //BUF数组的长度加
        }
        uart_put_chars(" -pseudo_terminal\0");
        uart_put_char('\n');
        argc=0;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                argv[i][j]=0;
            }
        }
        int cnt=0;
        for(int i=0;i<BUF_len;i++){
            char c=BUF[i];
            if(c==' '){
                argc++;
                cnt=0;
            }
            else{
                argv[argc][cnt]=c;
                cnt++;
            }
        }
        if(argv[0][0]=='h'&&argv[0][1]=='e'&&argv[0][2]=='l'&&argv[0][3]=='p'&&argv[0][4]==0){
            help.func(argc+1,argv);
        }
            
        else if(argv[0][0]=='c'&&argv[0][1]=='m'&&argv[0][2]=='d'&&argv[0][3]==0){
            cmd.func(argc+1,argv);
        }
        else{
            myPrintk(0x07,"Unknown Command\n\0");
        }
    }while(1);

}

