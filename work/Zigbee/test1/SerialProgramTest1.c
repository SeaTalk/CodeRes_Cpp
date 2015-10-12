#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#endif
#include<stdio.h>      /*��׼�����������*/   
#include<stdlib.h>     /*��׼�����ⶨ��*/   
#include<unistd.h>     /*Unix ��׼��������*/   
#include<sys/types.h>    
#include<sys/stat.h>      
#include<fcntl.h>      /*�ļ����ƶ���*/   
#include<termios.h>    /*PPSIX �ն˿��ƶ���*/   
#include<errno.h>      /*����Ŷ���*/   
#include<string.h>  

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
//#define BAUDRATE B115200            
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB0"
//#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE -1
#define TRUE 0

/* Start-of-frame delimiter for UART transport */
#define MT_UART_SOF                     0xFE

/* UART frame overhead for SOF and FCS bytes */
#define MT_UART_FRAME_OVHD              2
#define MT_UART_FRAME_COMMON              5

#define MT_RPC_POS_LEN    1
#define MT_RPC_POS_CMD0    2
#define MT_RPC_POS_CMD1    3


//volatile int STOP=FALSE; 
int speed_arr[] = {B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300,B115200,
          B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300};
int name_arr[] = {115200,57600,38400, 19200, 9600, 4800,  2400,  1200,  300,  115200,
          57600, 38400,19200,  9600, 4800, 2400, 1200,  300};

typedef unsigned char   uint8;
typedef void (*pRespCallFunc) (int);
#if 0
void set_speed(int fd, int speed)
{
	int   i; 
	int   status; 
	struct termios  Opt;
	tcgetattr(fd, &Opt); 
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) 
	{ 
		if  (speed == name_arr[i]) 
		{     
			tcflush(fd, TCIOFLUSH);     
			cfsetispeed(&Opt, speed_arr[i]);  
			cfsetospeed(&Opt, speed_arr[i]);   
			status = tcsetattr(fd, TCSANOW, &Opt);  
			if  (status != 0) 
			{        
				 perror("tcsetattr fd");  
				 return;     
			}    
			tcflush(fd,TCIOFLUSH);   
		}  
	}
}
#endif 

/*****************************************************************
* ���ƣ�                    OpenSerial
* ���ܣ�                    �򿪴��ڲ����ش����豸�ļ�����
* ��ڲ�����       fd    :�ļ�������     port :���ں�(ttyS0,ttyS1,ttyS2)
* ���ڲ�����       ��ȷ����Ϊ1�����󷵻�Ϊ0
*****************************************************************/
int OpenSerial(int fd,char* port)
{
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);

	if (FALSE == fd)
	{
		perror("Can't Open Serial Port\n");
		return (FALSE);
	}

	//�жϴ��ڵ�״̬�Ƿ�Ϊ����״̬                            
	if(fcntl(fd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		return(FALSE);
	}     
	else
	{
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	}

	//�����Ƿ�Ϊ�ն��豸    
	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}
	else
	{
		printf("isatty success!\n");
	}       
	printf("fd->open=%d\n",fd);
	
	return fd;
}


/*******************************************************************
* ���ƣ�                SetSerial
* ���ܣ�                ���ô�������λ��ֹͣλ��Ч��λ
* ��ڲ�����   fd         �����ļ�������
*                                    speed      �����ٶ�
*                                    flow_ctrl  ����������
*                                    databits   ����λ   ȡֵΪ 7 ����8
*                                    stopbits   ֹͣλ   ȡֵΪ 1 ����2
*                                    parity     Ч������ ȡֵΪN,E,O,,S
*���ڲ�����    ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int SetSerial(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)

{
	int   i;
	int   status;
	struct termios options;

	/*tcgetattr(fd,&options)�õ���fdָ��������ز�����
	�������Ǳ�����options,�ú���,�����Բ�������
	�Ƿ���ȷ���ô����Ƿ���õȡ������óɹ���
	��������ֵΪ0��������ʧ�ܣ���������ֵΪ1.
	*/

	if  ( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial error");    
		return (FALSE); 
	}

	//���ô������벨���ʺ����������
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
       {
	      if  (speed == name_arr[i])
	      {       
	              cfsetispeed(&options, speed_arr[i]); 
	              cfsetospeed(&options, speed_arr[i]);  
	      }
	}     

	//�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���
	options.c_cflag |= CLOCAL;

	//�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������
	options.c_cflag |= CREAD;

	//��������������
	switch(flow_ctrl)
	{
	case 0 ://��ʹ��������
	        options.c_cflag &= ~CRTSCTS;
	        break;   
	case 1 ://ʹ��Ӳ��������
	        options.c_cflag |= CRTSCTS;
	        break;
	case 2 ://ʹ�����������
	        options.c_cflag |= IXON | IXOFF | IXANY;
	        break;
	}

	//��������λ
	options.c_cflag &= ~CSIZE; //����������־λ

	switch (databits)
	{  
	case 5:
	        options.c_cflag |= CS5;
	        break;
	case 6:
	         options.c_cflag |= CS6;
	         break;
	case 7:    
	         options.c_cflag |= CS7;
	         break;
	case 8:    
	         options.c_cflag |= CS8;
	         break;  
	default:   
	         fprintf(stderr,"Unsupported data size\n");
	         return (FALSE); 
	}

	//����У��λ
	switch (parity)
	{  
	case 'n':
	case 'N': //����żУ��λ��
	         options.c_cflag &= ~PARENB; 
	         //options.c_iflag &= ~INPCK;    
	         options.c_iflag =  IGNPAR;    
	         break; 
	case 'o':  
	case 'O'://����Ϊ��У��    
	         options.c_cflag |= (PARODD | PARENB); 
	         options.c_iflag |= INPCK;             
	         break; 
	case 'e': 
	case 'E'://����ΪżУ��  
	         options.c_cflag |= PARENB;       
	         options.c_cflag &= ~PARODD;       
	         options.c_iflag |= INPCK;       
	         break;
	case 's':
	case 'S': //����Ϊ�ո� 
	         options.c_cflag &= ~PARENB;
	         options.c_cflag &= ~CSTOPB;
	         break; 
	default:  
	         fprintf(stderr,"Unsupported parity\n");   
	         return (FALSE); 
	} 

	// ����ֹͣλ 

	switch (stopbits)
	{  
	case 1:   
	         options.c_cflag &= ~CSTOPB; 
	         break; 
	case 2:   
	         options.c_cflag |= CSTOPB; 
	         break;
	default:   
	         fprintf(stderr,"Unsupported stop bits\n"); 
	         return (FALSE);
	}

	//�޸����ģʽ��ԭʼ�������
	//options.c_oflag &= ~OPOST;
	//options.c_oflag = 0;
	//options.c_lflag = 0;
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/

	//���õȴ�ʱ�����С�����ַ�
	options.c_cc[VTIME] = 0; /* ��ȡһ���ַ��ȴ�1*(1/10)s */  
	options.c_cc[VMIN] = 1; /* ��ȡ�ַ������ٸ���Ϊ1 */

	//�����������������������ݣ����ǲ��ٶ�ȡ
	tcflush(fd,TCIFLUSH);

	//�������� (���޸ĺ��termios�������õ������У�
	if (tcsetattr(fd,TCSANOW,&options) != 0)  
	{
	        perror("com set error!\n");  
		return (FALSE); 
	}

	return (TRUE); 

	#if 0
	struct termios newtio;

	if (fd < 0 )
		return FALSE;

//	tcgetattr( fd, &oldtio ); /* save current port settings */

	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag = speed | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME]	  = 0;
	newtio.c_cc[VMIN]	  = 1; //!! cannot be set to 0, otherwise, read will return immediately with retval 0

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	return TRUE;
	#endif
}

/*******************************************************************
* ���ƣ�                InitSerial()
* ���ܣ�                ���ڳ�ʼ��
* ��ڲ�����   fd           �ļ�������   
*                                    speed     �����ٶ�
*                                    flow_ctrl   ����������
*                                    databits    ����λ   ȡֵΪ 7 ����8
*                                    stopbits   ֹͣλ   ȡֵΪ 1 ����2
*                                    parity     Ч������ ȡֵΪN,E,O,,S                    
* ���ڲ�����   ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int InitSerial(int fd,  int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
	int err;

	//���ô�������֡��ʽ
	if (SetSerial(fd, speed, flow_ctrl, databits, stopbits, parity) == FALSE)
	{                                                     
		return FALSE;
	}
	else
	{
	       return  TRUE;
	}
}

/*******************************************************************
* ���ƣ�                  RecvFromSerial
* ���ܣ�                  ���մ�������
* ��ڲ�����     fd                  :�ļ�������    
*                                      rcv_buf     :���մ��������ݴ���rcv_buf��������
*                                      data_len    :���ݵĳ���
* ���ڲ�����     
*******************************************************************/
int RecvFromSerial(int fd, char *rcv_buf,int data_len)
{
	int len,fs_sel;
	fd_set fs_read;
	struct timeval time;

	FD_ZERO(&fs_read);
	FD_SET(fd,&fs_read);
	time.tv_sec = 10;
	time.tv_usec = 0;

	//ʹ��selectʵ�ִ��ڵĶ�·ͨ��
	fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);

	if(fs_sel)
	{
		len = read(fd,rcv_buf,data_len);
		return len;
	}
	else
	{
		return FALSE;
	}     

}

int ReadData(int fd, char *send_buf, int data_len)
{
	int dataReadLen = 0;
	int dataLeftLen = data_len;

	while(dataLeftLen > 0)
	{
		if((dataReadLen = RecvFromSerial(fd, send_buf, dataLeftLen)) < 0)
		{
			if(dataLeftLen == data_len)
			{
				return FALSE;
			}
			else
			{
				break;
			}
		}
		else if(0 == dataReadLen)
		{
			break;
		}
		dataLeftLen -= dataReadLen;
		send_buf += dataReadLen;
	}

	return (data_len -dataLeftLen);
}

/*******************************************************************
* ���ƣ�                SendToSerial
* ���ܣ�                ��������
* ��ڲ�����   fd                  :�ļ�������    
*                                    send_buf    :��Ŵ��ڷ�������
*                                    data_len    :һ֡���ݵĸ���
* ���ڲ�����   
*******************************************************************/
int SendToSerial(int fd, char *send_buf,int data_len)
{
	int writeLen = 0;
	int dataLeftLen = data_len;
	

	while(dataLeftLen > 0)
	{
		if((writeLen = write(fd,send_buf,dataLeftLen)) < 0)
		{
			if(dataLeftLen == data_len)
			{
				tcflush(fd,TCOFLUSH);
				return FALSE;
			}
			else
			{
				break;
			}
		}
		else if(0 == writeLen)
		{
			break;
		}
		dataLeftLen -= writeLen;
		send_buf += writeLen;
	}
	return (data_len -dataLeftLen);

}

/******************************************************
* ���ƣ�                CloseSerial
* ���ܣ�                �رմ��ڲ����ش����豸�ļ�����
* ��ڲ�����   fd    :�ļ�������   
* ���ڲ�����   void
*******************************************************************/
void CloseSerial(int fd)
{
	close(fd);
}

uint8 CalcFCS( uint8 *msg_ptr, uint8 len )
{
	uint8 x;
	uint8 xorResult = 0;

	for ( x = 0; x < len; x++, msg_ptr++ )
	{
		xorResult = xorResult ^ *msg_ptr;
	}

	return ( xorResult );
}

uint8 UbecCalcFCS( uint8 *msg_ptr, uint8 len )
{
	uint8 x;
	uint8 xorResult = 0;

	for ( x = 0; x < len; x++, msg_ptr++ )
	{
		xorResult = xorResult + *msg_ptr;
	}

	return ( 0xff-xorResult );
}

void SendData(int fd, uint8 *packet, uint8 dataLen, pRespCallFunc aRespCallFunc)
{
	int len;                          
	int j;

	len = SendToSerial(fd,packet, dataLen - 1);  
	if(len >= 0)  
	{
		printf("send data successful:\n");
		for(j = 0; j < dataLen - 1; j++)
			printf("%02x\n",*packet++);  
		printf("wait for resp:\n");
		aRespCallFunc(fd);
	}
	else  
		printf("send data failed!\n");
	
}

void SysVersionResp(int fd)
{
	int len = 0;    
	uint8 rcv_buf[100];    
	int i =0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 10 ); 
		if ( len >= 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < rcv_buf[1] + MT_UART_FRAME_COMMON; i++ )
				printf("%2x\n",rcv_buf[i]); 
			printf("the majorRel=%2x,the minorRel=%2x,the HwRev=%2X",
				 rcv_buf[MT_RPC_POS_CMD1 + 3], 
				 rcv_buf[MT_RPC_POS_CMD1 + 4],
				 rcv_buf[MT_RPC_POS_CMD1 + 5]);
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}
}

void GetSysVersion(int fd)
{
	uint8 send_buf[6] = {0xfe,0x00,0x21,0x02};
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size,SysVersionResp);
}

void ChannellistResp(int fd)
{
	int len = 0;
	uint8 rcv_buf[100]; 
	int i = 0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 12 ); 
		if ( len > 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < rcv_buf[1] + MT_UART_FRAME_COMMON; i++ )
				printf("%2x\n",rcv_buf[i]); 
			printf("the channellist=%d",
				 rcv_buf[MT_RPC_POS_CMD1 + 4]);
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}
}

pRespCallFunc gChannellistResp = NULL;

void InstallChannellistRespCallBack(pRespCallFunc aRespCallFunc)
{
	gChannellistResp = aRespCallFunc;
}
void GetChannellist(int fd)
{
	uint8 send_buf[7] = {0xfe,0x01,0x26,0x04,0x84};
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size, ChannellistResp);
}

/*
void GetLogicType(int fd)
{
	uint8 send_buf[7] = {0xfe,0x01,0x26,0x04,0x87};//logic type ZC\ZR\ZED
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size);
}

void AfRegiste(int fd)
{
	uint8 send_buf[17] = {0xfe,0x0b,0x24,0x00,0x01,0x04,0x01,0x00,
	                                      0x01,0x00,0x00,0x00,0x01,0x06,0x00};//registe
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size); 	     
}

void ZdoStartupFromApp(int fd)
{
	uint8 send_buf[7] = {0xfe,0x01,0x25,0x40,0x00};//start up
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size); 	     
}

void GetPanId(int fd)
{
	uint8 send_buf[7] = {0xfe,0x01,0x26,0x06,0x06};//pan id
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	SendData(fd, send_buf, send_buf_size);
}
*/

///*************start UEBC*********************///
void MeasuredValueResp(int fd)
{
	int len = 0;
	uint8 rcv_buf[100]; 
	int i = 0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 23 ); 
		if ( len >= 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < rcv_buf[1] + MT_UART_FRAME_COMMON; i++ )
				printf("%2x\n",rcv_buf[i]); 
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}	
}
void GetMeasuredValue(int fd)
{
	uint8 send_buf[19] = {0x02,0x00,0x0e,0x85,0x5B,0xC4,0x00,0x00,0x01,0x04,
		                           0x04,0x02,0x00,0x01,0x00,0x00,0x00,0x41,0x03};

	SendData(fd, send_buf, 19, MeasuredValueResp);
}

void AtInfoResp(int fd)
{
	int len = 0;
	uint8 rcv_buf[100]; 
	int i = 0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 50 ); 
		if ( len >= 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < 50; i++ )
				printf("%2x\n",rcv_buf[i]); 
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}	
	
}

void GetAtInfo(int fd)
{
	//uint8 send_buf[12] = {0x02,0x00,0x07,0x00,0x61,0x74,0x69,0x6e,0x66,0x6f,
		                          // 0x77,0x03};
	uint8 send_buf[12] = {0x02,0x00,0x07,0x00,'a','t','i','n','f','o',
		                           0x77,0x03};
	send_buf[10] = UbecCalcFCS(&send_buf[1], 9);

	SendData(fd, send_buf, 13, AtInfoResp);
}

void RemoteAtEpReqResp(int fd)
{
	int len = 0;
	uint8 rcv_buf[100]; 
	int i = 0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 50 ); 
		if ( len >= 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < 50; i++ )
				printf("%2x\n",rcv_buf[i]); 
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}	
	
}

void GetRemoteAtEpReq(int fd)
{
	//uint8 send_buf[12] = {0x02,0x00,0x07,0x00,0x61,0x74,0x69,0x6e,0x66,0x6f,
		                          // 0x77,0x03};
	uint8 send_buf[15] = {0x02,0x00,0x0a,0x83,0x2e,0xd7,'a','t','e','p','r','e','q',
		                           0x00,0x03};
	send_buf[13] = UbecCalcFCS(&send_buf[1], 12);

	SendData(fd, send_buf, 16, RemoteAtEpReqResp);
}

void TurnOnLightResp(int fd)
{
	int len = 0;
	uint8 rcv_buf[100]; 
	int i = 0;
        while (1) 
	{
		len = ReadData( fd, rcv_buf, 50 ); 
		if ( len >= 0 )  
		{  
		        printf("receive len=%d, data is:\n", len);
			for ( i = 0; i < 50; i++ )
				printf("%2x\n",rcv_buf[i]); 
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		} 
    	}	
}

void TurnOnLight(int fd)
{
	uint8 send_buf[17] = {0x02,0x00,0x0c,0x85,0x2e,0xd7,0x00,0x00,0x01,0x04,0x00,0x06,
		                           0x01,0x04,0x01,0x58,0x03};

	SendData(fd, send_buf, 18, TurnOnLightResp);
}


///*************end UEBC*********************///
int main(int argc, char **argv)  
{  
	int fd;                            //�ļ�������   
	int err;                           //���ص��ú�����״̬   
	int i,j;  
	//uint8 rcv_buf[100];         
	//uint8 send_buf[7] = {0xfe,0x01,0x26,0x04,0x83};//pan id
	uint8 send_buf[7] = {0xfe,0x01,0x26,0x06,0x06};//pan id
	//uint8 send_buf[7] = {0xfe,0x01,0x41,0x00,0x01};//reset
	uint8 send_buf_size = sizeof(send_buf);
        send_buf[send_buf_size -2] = CalcFCS(&send_buf[1], send_buf_size - 3);
	send_buf[send_buf_size -1] = '\0';
	
	#if 0
	if(argc != 3)  
	{  
		printf("Usage: %s /dev/ttySn 0(send data)/1 (receive data) \n",argv[0]);  
		return FALSE;  
	}  
	#endif

	if(2 == argc)
	{
		fd = OpenSerial(fd, argv[1]); //�򿪴��ڣ������ļ�������  
	}
	else
	{
		fd = OpenSerial(fd, MODEMDEVICE);
	}
	do{  
		err = InitSerial(fd,115200,0,8,1,'N'); 
		//err = InitSerial(fd,115200,0,8,1,'N');  
		printf("Set Port Exactly!err=%d.fd=%d\n",err,fd);  
	}while(FALSE == err || FALSE == fd);  

	//GetSysVersion(fd);
	//GetMeasuredValue(fd);
	GetAtInfo(fd);
	//GetRemoteAtEpReq(fd);
	//TurnOnLight(fd);
	#if 0
        while (1) 
	{
		len = RecvFromSerial(fd, rcv_buf,14); 
		//len = read(fd,rcv_buf,10);
		if(len > 0)  
		{  
		        //if(0xfe == rcv_buf[0])
		        printf("receive data is:\n");
			for(i = 0; i < rcv_buf[1] + 5; i++)
				printf("%2x\n",rcv_buf[i]);  
			printf("len = %d\n",len); 
			
		}  
		else  
		{  
			printf("cannot receive data\n");  
			break;
		}  
		//sleep(2);  
	}
	#endif
	CloseSerial(fd); 
	exit(0);
}  

#if 0
main()
{
  int fd,c, res;
  struct termios oldtio,newtio;
  char buf[255];
/* 
  Open modem device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
*/
 fd = openDev(MODEMDEVICE); 
 if (-1 == fd)
 {
 	exit(-1);
 }

 tcgetattr(fd,&oldtio); /* save current serial port settings */
 bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

/* 
  BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
  CRTSCTS : output hardware flow control (only used if the cable has
            all necessary lines. See sect. 7 of Serial-HOWTO)
  CS8     : 8n1 (8bit,no parity,1 stopbit)
  CLOCAL  : local connection, no modem contol
  CREAD   : enable receiving characters
*/
 newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
 
/*
  IGNPAR  : ignore bytes with parity errors
  ICRNL   : map CR to NL (otherwise a CR input on the other computer
            will not terminate input)
  otherwise make device raw (no other input processing)
*/
 newtio.c_iflag = IGNPAR | ICRNL;
 
/*
 Raw output.
*/
 newtio.c_oflag = 0;
 
/*
  ICANON  : enable canonical input
  disable all echo functionality, and don't send signals to calling program
*/
 newtio.c_lflag = ICANON;
 
/* 
  initialize all control characters 
  default values can be found in /usr/include/termios.h, and are given
  in the comments, but we don't need them here
*/
 newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
 newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
 newtio.c_cc[VERASE]   = 0;     /* del */
 newtio.c_cc[VKILL]    = 0;     /* @ */
 newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
 newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
 newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
 newtio.c_cc[VSWTC]    = 0;     /* '\0' */
 newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
 newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
 newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
 newtio.c_cc[VEOL]     = 0;     /* '\0' */
 newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
 newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
 newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
 newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
 newtio.c_cc[VEOL2]    = 0;     /* '\0' */

/* 
  now clean the modem line and activate the settings for the port
*/
 tcflush(fd, TCIFLUSH);
 tcsetattr(fd,TCSANOW,&newtio);


int len = 0;

 
//len = write(fd,"ls -l\r\n",strlen("ls -l\r\n"));

len = write(fd,"ps\r\n",strlen("ps\r\n"));
//printf("len = %d,%d\n",strlen("ls -l\r\n"),len);

#if 0
/*
  terminal settings done, now handle input
  In this example, inputting a 'z' at the beginning of a line will 
  exit the program.
*/
 while (STOP==FALSE) {     /* loop until we have a terminating condition */
 /* read blocks program execution until a line terminating character is 
    input, even if more than 255 chars are input. If the number
    of characters read is smaller than the number of chars available,
    subsequent reads will return the remaining chars. res will be set
    to the actual number of characters actually read */
    res = read(fd,buf,255); 
    buf[res]=0;             /* set end of string, so we can printf */
    printf(":%s:%d\n", buf, res);
    if (buf[0]=='z') STOP=TRUE;
 }
 #endif
 /* restore the old port settings */
 tcsetattr(fd,TCSANOW,&oldtio);
 close(fd);
}
#endif

