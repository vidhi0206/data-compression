#include <stdio.h>
#include <string.h>
#include <limits.h>
#include<math.h>
#include <time.h>
#include <memory.h>
#include "windows.h"
#include "psapi.h"
void main()
{
    clock_t t;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	t= clock();
    FILE *file,*fileout;
    float data[16];
    fileout = fopen("diff-2-gactive.txt", "wb");
    if (file = fopen("gactive.txt", "r"))
    {
        int flag=-1;
        while (!feof(file))
        {
            for(int i=0;i<16;i++)
            {
                data[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%f", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 100) / 100;
                //printf("%d ",data[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=15;i>0;i--)
            {
                data[i]=data[i]-data[i-1];
                data[i]=roundf(data[i] * 100) / 100;
            }
            for(int k=0;k<16;k++)
            {
                
                fprintf(fileout,"%.2f ",data[k]);
            }
        }
        fclose(fileout);
        fclose(file);
        
    }
    t = clock() -t;
    double timetaken = ((double)t)/CLOCKS_PER_SEC;
    fprintf(stderr, "Seconds of execution %f\n", timetaken);
	 GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	printf("virtual memory used by program:%lu bytes\n RAM used by program:%lu bytes",virtualMemUsedByMe, physMemUsedByMe);
    
}