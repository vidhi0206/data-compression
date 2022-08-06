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
    float t[64];
    int IFZ[64];

    fileout = fopen("stat-diff-II-2-watch-acc-64-40.txt", "r");
    if (file = fopen("decoded.txt", "w"))
    {
        while (!feof(fileout))
        {
            float added=0;
            int IFZ1=0,IFZ2=0;
            int flag=-1;

            if(fscanf(fileout, "%f", &added)==EOF)
                break;
            added=roundf(added * 100) / 100;
            if(fscanf(fileout, "%d", &IFZ1)==EOF)
                break;
            if(fscanf(fileout, "%d", &IFZ2)==EOF)
                break;
            for(int i=0;i<32;i++)
            {
                IFZ[i]=IFZ1&(1<<(31-i));
                //printf("%d\n",1<<(31-i));
            }
            for(int i=0;i<32;i++)
            {
                IFZ[i+32]=IFZ2&(1<<(31-i));
                //printf("%d\n",1<<(31-i));
            }

            for(int i=0;i<64;i++)
            {
                t[i]=-1;
            }

            for(int j=0;j<64;j++)
            {
                if(IFZ[j]!=0)
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
                for(int i=1;i<64;i++)
                {
                    //printf("%lf \t",t[i]);
                    t[i]=t[i-1]+t[i];
                    //printf("%lf \n",t[i]);
                    t[i]=roundf(t[i] * 100) / 100;
                }
                for(int k=0;k<64;k++)
                {
                    fprintf(file,"%.2f\n",t[k]);
                }
            }
            else
            {
                for(int i=0;i<64;i++)
                {
                    t[i]=t[i]+added;
                    t[i]=roundf(t[i] * 100) / 100;
                }
                for(int k=0;k<64;k++)
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