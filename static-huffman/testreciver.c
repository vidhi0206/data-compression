
#include "include/commons.h"
#include "include/compressor.h"
#include "include/decompressor.h"
#include "include/compressor.c"
#include "include/compressor_functions.c"
#include "include/decompressor.c"
#include "include/decompressor_functions.c"
#include <time.h>
#include <windows.h>
#include "psapi.h"
int main(void)
{
    
    char *in="encode-stat-diff-2-BVP-64-50.txt";
	char *out="decode.txt";

    clock_t t;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	t= clock();

    
	decompressor(in, out);

    t = clock() -t;
    double timetaken = ((double)t)/CLOCKS_PER_SEC;
    fprintf(stderr, "Seconds of execution %f\n", timetaken);
	 GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	printf("virtual memory used by program:%lu bytes\n RAM used by program:%lu bytes",virtualMemUsedByMe, physMemUsedByMe);
    return 0;
}
