/*
 * 本文件实现vga的相关功能，清屏和屏幕输出
 * clear_screen和append2screen必须按照如下借口实现
 * 可以增加其他函数供clear_screen和append2screen调用
 */
extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);
extern void uart_put_char(unsigned char c);
//VGA字符界面规格：25行80列
//VGA显存初始地址为0xB8000

short cur_line=0;
short cur_column=0;//当前光标位置
char * vga_init_p=(char *)0xB8000;

/*

*不必局限于以下框架，可以自定义接口来实现，只需要保证append2screen接口能够
被正确调用即可  

*/
void update_cursor(void){//通过当前行值cur_cline与列值cur_column回写光标
	outb(0x3d4,0xe);
	outb(0x3d5,cur_line);
	outb(0x3d4,0xf);
	outb(0x3d5,cur_column);
}

void get_cursor_position(void){//获得当前光标，计算出cur_line和cur_column的值
	outb(0x3d4,0xe);
	cur_line=inb(0x3d5);
	outb(0x3d4,0xf);
	cur_column=inb(0x3d5);
}

char* pos(short line,short column){
	return vga_init_p+line*80*2+column*2;
}
void clear_screen(void) {//清屏
	for(short line=0;line<25;line++){
		for(short column=0;column<80;column++){
			*pos(line,column)=0;
			*(pos(line,column)+1)=0;
		}
	}
	cur_line=cur_column=0;
	update_cursor();
}
void roll_up(){
	for(short line=1;line<25;line++){
		for(short column=0;column<80;column++){
			*pos(line-1,column)=*pos(line,column);
			*(pos(line-1,column)+1)=*(pos(line,column)+1);
		}
	}
	for(short column=0;column<80;column++){
		*pos(24,column)=0;
		*(pos(24,column)+1)=0;
	}
}
void append2screen(char *str,int color){ //将str输出到vga屏幕
	get_cursor_position();
	char color_8=color;
	while(*str){
		if(*str=='\n'){
			cur_column=0;
			if(cur_line<24)
				cur_line++;
			else
				roll_up();
			str++;
			update_cursor();
			continue;
		}
		else
			*pos(cur_line,cur_column)=*str;
		*(pos(cur_line,cur_column)+1)=color_8;
		if(cur_column<79)
			cur_column++;
		else{
			cur_column=0;
			if(cur_line<24)
				cur_line++;
			else
				roll_up();
		}
		str++;
		update_cursor();
	}
}


