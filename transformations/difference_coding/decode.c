#include <stdio.h>
#include <string.h>
#include <limits.h>
#include<math.h>
#include <time.h>
#include <memory.h>
#include "windows.h"
#include "psapi.h"
int main(void)
{
    clock_t ti;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	ti= clock();
    FILE *file,*fileout;
    float t[16];

    fileout = fopen("diff-2-gactive.txt", "r");
    if (file = fopen("decoded.txt", "w"))
    {
        while (!feof(file))
        {
            int flag=-1;
            for(int i=0;i<16;i++)
            {
                t[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(fileout, "%f", &t[j])==EOF)
                {
                    flag=j;
                    break;
                }
                t[j]=roundf(t[j] * 100) / 100;
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=1;i<16;i++)
            {
                t[i]=t[i-1]+t[i];
                t[i]=roundf(t[i] * 100) / 100;
            }
            for(int k=0;k<16;k++)
            {
                fprintf(file,"%.2f\n",t[k]);
            }
        }
        fclose(fileout);
        fclose(file);
    }
    ti = clock() -ti;
    double timetaken = ((double)ti)/CLOCKS_PER_SEC;
    fprintf(stderr, "Seconds of execution %f\n", timetaken);
	 GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	printf("virtual memory used by program:%lu bytes\n RAM used by program:%lu bytes",virtualMemUsedByMe, physMemUsedByMe);
    return 0;
}