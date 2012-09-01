#include <io.h>
#include <fcntl.h>

void InitConsole()
{
    int nRet= 0;
    FILE* fp;
    AllocConsole();
    nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nRet, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
	printf("MapDeluxe Debugging Console.\n");
}
