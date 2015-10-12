#include "zigbee_common.h"

#define TRUE 0
#define FALSE -1

static
int speed_arr[] = {B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300,B115200,
          B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300};

static
int name_arr[] = {115200,57600,38400, 19200, 9600, 4800,  2400,  1200,  300,  115200,
          57600, 38400,19200,  9600, 4800, 2400, 1200,  300};

int OpenSerial( int fd, const char* port)
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

	//�����Ƿ�Ϊ�ն��豸	  
	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}

	return fd;
}

static
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
	case 5    :
	        options.c_cflag |= CS5;
	        break;
	case 6    :
	         options.c_cflag |= CS6;
	         break;
	case 7    :    
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
}

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

		  

