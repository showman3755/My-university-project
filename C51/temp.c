#include <reg51.h>
#include "intrins.h" 
#define LCD1602_DATAPINS P0
sbit beep=P3^7;
sbit led = P1^0;
sbit k1=P1^5;  //°´¼ü1
sbit k2=P1^6; //°´¼ü2
sbit k3=P1^7;//°´¼ü3
sbit LCD1602_RS=P2^0;
sbit LCD1602_RW=P2^1;
sbit LCD1602_E=P2^2;
sbit DSPORT=P2^3;
void LcdDisplay(int temp);
void UI_Init(void);
void LcdWriteCom(unsigned char com);
void LcdWriteData(unsigned char dat);
void LcdInit(void);
int KeyScan(void);
void Delay1ms(unsigned int );
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte(void);
void  Ds18b20ChangTemp(void);
void  Ds18b20ReadTempCom(void);
int Ds18b20ReadTemp(void);
void Lcd1602_Delay1ms(unsigned int c);   //Îó²î 0us

static int choose = 0;
int a,temphigh = 3000,templow = 1000;

void main()
{
	int Key_Value;
	LcdInit();			 //³õÊ¼»¯LCD1602	
	UI_Init();
	while(1)
	{
		a=Ds18b20ReadTemp();
		LcdDisplay(a);
		a=a*0.0625*100+0.5;
		Key_Value = KeyScan();
		if ((a >= temphigh) || (a <= templow))
		{
			beep=1;
			led = 0;
		} 
		else 
		{
			beep=0;
			led = 1;
		}
		LcdWriteCom(0xc0 + 10);		 
		LcdWriteData('0'+ (templow / 1000));
		LcdWriteCom(0xc0 + 11);		 
		LcdWriteData('0'+ (templow % 1000 / 100));
	
		LcdWriteCom(0xc0 + 14);		 
		LcdWriteData('0'+ (temphigh / 1000));
		LcdWriteCom(0xc0 + 15);		 
		LcdWriteData('0'+ (temphigh % 1000 / 100));
	}
}

void LcdDisplay(int temp) 	 //lcdÏÔÊ¾
{
    
 unsigned char datas[] = {0, 0, 0, 0, 0}; 
	float tp;  
	if(temp< 0)				//µ±ÎÂ¶ÈÎª¸ºÖµ
  	{
	  	LcdWriteCom(0x85);		//Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	    LcdWriteData('-');  		//ÏÔÊ¾¸º
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öÐ¡Êýµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊý×ª»»ÎªÕûÐÍµÄÊ±ºò°ÑÐ¡Êýµã
		//ºóÃæµÄÊý×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ð¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚÐ¡ÊýµãºóÃæ¡£
 
  	}
 	else
  	{			
	  	LcdWriteCom(0x85);		//Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	    LcdWriteData('+'); 		//ÏÔÊ¾Õý
		tp=temp;//ÒòÎªÊý¾Ý´¦ÀíÓÐÐ¡ÊýµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãÐÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕýµÄÄÇÃ´£¬ÄÇÃ´ÕýÊýµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öÐ¡Êýµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊý×ª»»ÎªÕûÐÍµÄÊ±ºò°ÑÐ¡Êýµã
		//ºóÃæµÄÊý×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ð¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚÐ¡ÊýµãºóÃæ¡£
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;
 
	LcdWriteCom(0x86);		  //Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('0'+datas[0]); //°ÙÎ»

	
	LcdWriteCom(0x87);		 //Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('0'+datas[1]); //Ê®Î»

	LcdWriteCom(0x88);		//Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('0'+datas[2]); //¸öÎ» 

	LcdWriteCom(0x89);		//Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('.'); 		//ÏÔÊ¾"."

	LcdWriteCom(0x8a);		 //Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('0'+datas[3]); //ÏÔÊ¾Ð¡Êýµã  

	LcdWriteCom(0x8b);		 //Ð´µØÖ·80±íÊ¾³õÊ¼µØÖ·
	LcdWriteData('0'+datas[4]); //ÏÔÊ¾Ð¡Êýµã 
}
void Lcd1602_Delay1ms(unsigned int c)   //Îó²î 0us
{
  unsigned char a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}
void LcdWriteCom(unsigned char com)	  //Ð´ÈëÃüÁî
{
	LCD1602_E = 0;     //Ê¹ÄÜ
	LCD1602_RS = 0;	   //Ñ¡Ôñ·¢ËÍÃüÁî
	LCD1602_RW = 0;	   //Ñ¡ÔñÐ´Èë
	
	LCD1602_DATAPINS = com;     //·ÅÈëÃüÁî
	Lcd1602_Delay1ms(1);		//µÈ´ýÊý¾ÝÎÈ¶¨

	LCD1602_E = 1;	          //Ð´ÈëÊ±Ðò
	Lcd1602_Delay1ms(1);	  //±£³ÖÊ±¼ä
	LCD1602_E = 0;
}
void LcdWriteData(unsigned char dat)			//Ð´ÈëÊý¾Ý
{
	LCD1602_E = 0;	//Ê¹ÄÜÇåÁã
	LCD1602_RS = 1;	//Ñ¡ÔñÊäÈëÊý¾Ý
	LCD1602_RW = 0;	//Ñ¡ÔñÐ´Èë

	LCD1602_DATAPINS = dat; //Ð´ÈëÊý¾Ý
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //Ð´ÈëÊ±Ðò
	Lcd1602_Delay1ms(1);   //±£³ÖÊ±¼ä
	LCD1602_E = 0;
}
void LcdInit()						  //LCD³õÊ¼»¯×Ó³ÌÐò
{
 	LcdWriteCom(0x38);  //¿ªÏÔÊ¾
	LcdWriteCom(0x0c);  //¿ªÏÔÊ¾²»ÏÔÊ¾¹â±ê
	LcdWriteCom(0x06);  //Ð´Ò»¸öÖ¸Õë¼Ó1
	LcdWriteCom(0x01);  //ÇåÆÁ
	LcdWriteCom(0x80);  //ÉèÖÃÊý¾ÝÖ¸ÕëÆðµã
}

/*******************************************************************************
* º¯ Êý Ãû         : Delay1ms
* º¯Êý¹¦ÄÜ		   : ÑÓÊ±º¯Êý
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void Delay1ms(unsigned int y)
{
	unsigned int x;
	for( ; y>0; y--)
	{
		for(x=110; x>0; x--);
	}
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20Init
* º¯Êý¹¦ÄÜ		   : ³õÊ¼»¯
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ³õÊ¼»¯³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
*******************************************************************************/

unsigned char Ds18b20Init()
{
	unsigned char i;
	DSPORT = 0;			 //½«×ÜÏßÀ­µÍ480us~960us
	i = 100;	
	while(i--);//ÑÓÊ±642us
	DSPORT = 1;			//È»ºóÀ­¸ß×ÜÏß£¬Èç¹ûDS18B20×ö³ö·´Ó¦»á½«ÔÚ15us~60usºó×ÜÏßÀ­µÍ
	i = 0;
	while(DSPORT)	//µÈ´ýDS18B20À­µÍ×ÜÏß
	{
		i++;
		if(i>5)//µÈ´ý>5MS
		{
			return 0;//³õÊ¼»¯Ê§°Ü
		}
		Delay1ms(1);	
	}
	return 1;//³õÊ¼»¯³É¹¦
}

/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20WriteByte
* º¯Êý¹¦ÄÜ		   : Ïò18B20Ð´ÈëÒ»¸ö×Ö½Ú
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i, j;

	for(j=0; j<8; j++)
	{
		DSPORT = 0;	     	  //Ã¿Ð´ÈëÒ»Î»Êý¾ÝÖ®Ç°ÏÈ°Ñ×ÜÏßÀ­µÍ1us
		i++;
		DSPORT = dat & 0x01;  //È»ºóÐ´ÈëÒ»¸öÊý¾Ý£¬´Ó×îµÍÎ»¿ªÊ¼
		i=6;
		while(i--); //ÑÓÊ±68us£¬³ÖÐøÊ±¼ä×îÉÙ60us
		DSPORT = 1;	//È»ºóÊÍ·Å×ÜÏß£¬ÖÁÉÙ1us¸ø×ÜÏß»Ö¸´Ê±¼ä²ÅÄÜ½Ó×ÅÐ´ÈëµÚ¶þ¸öÊýÖµ
		dat >>= 1;
	}
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadByte
* º¯Êý¹¦ÄÜ		   : ¶ÁÈ¡Ò»¸ö×Ö½Ú
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/


unsigned char Ds18b20ReadByte()
{
	unsigned char byte, bi;
	unsigned int i, j;	
	for(j=8; j>0; j--)
	{
		DSPORT = 0;//ÏÈ½«×ÜÏßÀ­µÍ1us
		i++;
		DSPORT = 1;//È»ºóÊÍ·Å×ÜÏß
		i++;
		i++;//ÑÓÊ±6usµÈ´ýÊý¾ÝÎÈ¶¨
		bi = DSPORT;	 //¶ÁÈ¡Êý¾Ý£¬´Ó×îµÍÎ»¿ªÊ¼¶ÁÈ¡
		/*½«byte×óÒÆÒ»Î»£¬È»ºóÓëÉÏÓÒÒÆ7Î»ºóµÄbi£¬×¢ÒâÒÆ¶¯Ö®ºóÒÆµôÄÇÎ»²¹0¡£*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;		//¶ÁÈ¡ÍêÖ®ºóµÈ´ý48usÔÙ½Ó×Å¶ÁÈ¡ÏÂÒ»¸öÊý
		while(i--);
	}				
	return byte;
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ChangTemp
* º¯Êý¹¦ÄÜ		   : ÈÃ18b20¿ªÊ¼×ª»»ÎÂ¶È
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//Ìø¹ýROM²Ù×÷ÃüÁî		 
	Ds18b20WriteByte(0x44);	    //ÎÂ¶È×ª»»ÃüÁî
	Delay1ms(100);	//µÈ´ý×ª»»³É¹¦£¬¶øÈç¹ûÄãÊÇÒ»Ö±Ë¢×ÅµÄ»°£¬¾Í²»ÓÃÕâ¸öÑÓÊ±ÁË
   
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadTempCom
* º¯Êý¹¦ÄÜ		   : ·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //Ìø¹ýROM²Ù×÷ÃüÁî
	Ds18b20WriteByte(0xbe);	 //·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadTemp
* º¯Êý¹¦ÄÜ		   : ¶ÁÈ¡ÎÂ¶È
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

int Ds18b20ReadTemp()
{
	int temp = 0;
	unsigned char tmh, tml;
	Ds18b20ChangTemp();			 	//ÏÈÐ´Èë×ª»»ÃüÁî
	Ds18b20ReadTempCom();			//È»ºóµÈ´ý×ª»»Íêºó·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
	tml = Ds18b20ReadByte();		//¶ÁÈ¡ÎÂ¶ÈÖµ¹²16Î»£¬ÏÈ¶ÁµÍ×Ö½Ú
	tmh = Ds18b20ReadByte();		//ÔÙ¶Á¸ß×Ö½Ú
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}

int KeyScan(void)
{
	int Key_Value;
		if(k3 == 0)
	   {
			Delay1ms(5);
		   if(k3 == 0)
		   {
				choose = choose + 1;
				if(choose >= 3) choose = 0;
			   while(!k3);
			 }
		 }
		if(choose == 1)
		{
			if(k1==0)
		   {
				Delay1ms(5);
			   if(k1 == 0)
			   {
					temphigh = temphigh + 100;
					Key_Value = 2;
				  while(!k1);
				}
		   }
		   if(k2==0)
		   {
			  Delay1ms(5);
			  if(k2 == 0)
			  {
					temphigh = temphigh - 100;
					Key_Value = 3;
					while(!k2);
				}
		   }
		}
		if(choose == 2)
		{
			if(k1==0)
		   {
				Delay1ms(5);
			  if(k1 == 0)
			  {
					templow = templow + 100;
					Key_Value = 4;
				  while(!k1);
				}
		   }
		   if(k2==0)
		   {
				 Delay1ms(5);
			  if(k2 == 0)
			  {
					templow = templow - 100;
					Key_Value = 5;
				  while(!k2);
				}
		  }
		}
	
	return Key_Value;
}

void UI_Init(void)
{
		LcdWriteCom(0x80);	
	LcdWriteData('t'); 
	LcdWriteData('e'); 
	LcdWriteData('m');
	LcdWriteData('p'); 	
	LcdWriteData(':');
	LcdWriteCom(0x80+12);	
	LcdWriteData('c'); 
	LcdWriteCom(0xc0);
	LcdWriteData('W');
	LcdWriteData('a');
	LcdWriteData('r');
	LcdWriteData('n');
	LcdWriteData('i');
	LcdWriteData('n');
	LcdWriteData('g');
	LcdWriteData('t');
	LcdWriteData('D');
	LcdWriteData(':');
	LcdWriteCom(0xca);
	LcdWriteData('1');
  LcdWriteData('0');
	LcdWriteCom(0xcd);
	LcdWriteData('U');
	LcdWriteData('3');
	LcdWriteData('0');
}
