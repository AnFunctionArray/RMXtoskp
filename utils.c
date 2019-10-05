#include <stdint.h>
printfrawbytes(pBuffer, szStream)
unsigned char* pBuffer;
size_t szStream;
{
	for (unsigned int i = 0; i < szStream; ++i)
		if (!(i % 0x10)) printf("\n");
		else printf("%x ", i[pBuffer]);
}