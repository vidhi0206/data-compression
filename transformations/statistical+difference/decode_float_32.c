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
    double t[32];
    int IFZ1[32];

    fileout = fopen("stat-diff-2-gas-5.txt", "r");
    if (file = fopen("decoded.txt", "w"))
    {
        while (!feof(fileout))
        {
            int ind;
            if(fscanf(fileout, "%d", &ind)==EOF)
                break;
            for(int i=0;i<32;i++)
            {
                t[i]=-1;
            }
            if(ind==1)
            {
                double added=0;
                unsigned int IFZ=0;
                int flag=-1;

                if(fscanf(fileout, "%lf", &added)==EOF)
                    break;
                if(fscanf(fileout, "%u", &IFZ)==EOF)
                    break;
                //printf("%d ",IFZ);
                added=roundf(added * 100) / 100;
                for(int i=0;i<32;i++)
                {
                    IFZ1[i]=IFZ&(1<<(31-i));
                    //printf("%d\n",1<<(15-i));
                }

                for(int j=0;j<32;j++)
                {
                    if(IFZ1[j]!=0)
                    {
                        if(fscanf(fileout, "%lf", &t[j])==EOF)
                        {
                            flag=j;
                            break;
                        }
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
                for(int i=0;i<32;i++)
                {
                    //printf("%lf \t",t[i]);
                    t[i]=t[i]+added;
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
                int flag=-1;
                for(int j=0;j<32;j++)
                {
                    if(fscanf(fileout, "%lf", &t[j])==EOF)
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