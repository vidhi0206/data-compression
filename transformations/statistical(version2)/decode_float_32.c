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
    float t[32];
    int IFZ1[32];

    fileout = fopen("stat-diff-II-2-BVP-32-5.txt", "r");
    if (file = fopen("decoded.txt", "w"))
    {
        while (!feof(fileout))
        {
            float added=0;
            int IFZ=0;
            int flag=-1;

            if(fscanf(fileout, "%f", &added)==EOF)
                break;
            added=roundf(added * 100) / 100;
            if(fscanf(fileout, "%d", &IFZ)==EOF)
                break;
            for(int i=0;i<32;i++)
            {
                IFZ1[i]=IFZ&(1<<(31-i));
                //printf("%d\n",1<<(31-i));
            }

            for(int i=0;i<32;i++)
            {
                t[i]=-1;
            }

            for(int j=0;j<32;j++)
            {
                if(IFZ1[j]!=0)
                {
                    if(fscanf(fileout, "%f", &t[j])==EOF)
                    {
                        flag=j;
                        break;
                    }
                    //printf("%f", t[j]);
                    t[j]=roundf(t[j] * 100) / 100;
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
            if(added==t[0])
            {
                for(int i=1;i<32;i++)
                {
                    //printf("%lf \t",t[i]);
                    t[i]=t[i-1]+t[i];
                    //printf("%lf \n",t[i]);
                    t[i]=roundf(t[i] * 100) / 100;
                }
                for(int k=0;k<32;k++)
                {
                    fprintf(file,"%.2f\n",t[k]);
                }
            }
            else
            {
                for(int i=0;i<32;i++)
                {
                    t[i]=t[i]+added;
                    t[i]=roundf(t[i] * 100) / 100;
                }
                for(int k=0;k<32;k++)
                {
                    fprintf(file,"%.2f\n",t[k]);
                }
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