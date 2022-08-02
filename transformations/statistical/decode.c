#include<stdio.h>
#include<string.h>
#include<time.h>
#include <memory.h>
#include "windows.h"
#include "psapi.h"
int main(void)
{
    clock_t ti;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	ti= clock();
    FILE *file,*fileout;
    int t[16];
    int IFZ1[16];

    fileout = fopen("output.txt", "r");
    if (file = fopen("ECG-decoded.txt", "w"))
    {
        while (!feof(fileout))
        {
            int added=0;
            int IFZ=0;
            int flag=-1;

            if(fscanf(fileout, "%d", &added)==EOF)
                break;
            if(fscanf(fileout, "%d", &IFZ)==EOF)
                break;
            //printf("%d ",IFZ);
            for(int i=0;i<16;i++)
            {
                IFZ1[i]=IFZ&(1<<(15-i));
                //printf("%d\n",1<<(15-i));
            }

            for(int i=0;i<16;i++)
            {
                t[i]=-1;
            }

            for(int j=0;j<16;j++)
            {
                if(IFZ1[j]!=0)
                {
                    if(fscanf(fileout, "%d", &t[j])==EOF)
                    {
                        flag=j;
                        break;
                    }
                }
                else
                {
                    t[j]=0;
                }
                //printf("%d ",t[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<16;i++)
            {
                t[i]=t[i]+added;
            }
            for(int k=0;k<16;k++)
            {
                fprintf(file,"%d\n",t[k]);
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