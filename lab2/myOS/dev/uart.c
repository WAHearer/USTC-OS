/* 
 * 与 UART 相关的输出
 * 调用inb和outb函数，实现下面的uart的三个函数
 */
extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);

#define uart_base 0x3F8

void uart_put_char(unsigned char c){
	outb(0x3f8,c);
}

unsigned char uart_get_char(void){
	return inb(0x3f8);
}

void uart_put_chars(char *str){ 
	while(*str){
		if(*str=='\n')
			uart_put_char('\r');
		else
			uart_put_char(*str);
		str++;
	}
}