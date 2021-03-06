#include <cstdio>
#include <cstdlib>
#include <Windows.h>

static HANDLE hCom;
static DCB dcb;

int main()
{
	unsigned char buf[256] = {0xFE, 0x0, 0x21, 0x02, 0x23};
	DWORD count;
	BOOL ret;

	//freopen( "out.txt", "w", stdout );
	//setvbuf( stdout, NULL, _IONBF, 0 );

	// open
	hCom=CreateFile( "COM1",//COM1口  
		GENERIC_READ|GENERIC_WRITE, //允许读和写  
		0, //独占方式  
		NULL,  
		OPEN_EXISTING, //打开而不是创建  
		0, //同步方式  
		NULL );  
	if(hCom==(HANDLE)-1)  
	{  
		printf( "Open serial port fail!\n" );
		exit(-1); 
	}

	// set attribute
	GetCommState(hCom, &dcb);
	//printf( "%u\n", dcb.BaudRate );
	dcb.BaudRate = CBR_115200;
	dcb.fBinary = TRUE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	SetCommState(hCom, &dcb);

	// clear buffer
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);

	// write
	ret = WriteFile(hCom, buf, 5, &count, NULL );
	if( !ret )
		printf( "write error!\n" );
	printf( "%u bytes written.\n", count );

	//unsigned char *ptr = buf;
	unsigned char tmp;
	while( ReadFile(hCom, &tmp, 1, &count, NULL) ) {
		printf( "%x\n", tmp );
	}
	//while( ReadFile(hCom, ptr, 256, &count, NULL ) ) {
	//	ptr += count;
	//}
	//printf( "%d bytes read.\n", ptr-buf );

	CloseHandle(hCom);
	return 0;
}