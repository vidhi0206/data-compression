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
    clock_t t;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	t= clock();
    FILE *file,*fileout;
    double data[64];
    double sort[64];
    double data1[67];
    fileout = fopen("stat-diff-II-0-gas-64-50.txt", "wb");
    if (file = fopen("gas_reading.txt", "r"))
    {
        while (!feof(fileout))
        {
            double sum=0;
            double mean;
            double median;
            double mode;
            double t=0;
            double max=0,c=0;
            unsigned int IFZ1=0,IFZ2=0;
            int flag=-1;
            for(int i=0;i<64;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<64;j++)
            {
                if(fscanf(file, "%lf", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 1) / 1;
                sort[j]=data[j];
                //printf("%d ",sort[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<64;i++)
            {
                for(int j=0;j<64;j++)
                {
                    if(sort[i]<sort[j])
                    {
                        t=sort[i];
                        sort[i]=sort[j];
                        sort[j]=t;
                    }
                }
            }
            for(int i=0; i<64; i++)
            {
                t=sort[i];
                c=0;   
                for(int j=0; j<64; j++) 
                { 
                    if(t==sort[j]) 
                        c++; 
                    if(c>max)
                    {
                        max=c;
                        mode=t;
                    }
                }
            }
            mode=roundf(mode * 1) / 1;
            if(max>=50)
            {
                data1[0]=mode;
                for(int i=0;i<64;i++)
                {
                    data1[i+3]=data[i]-mode;
                    //printf("%d ",data1[i+2]);
                }
            }
            else
            {
                data1[0]=data[0];
                data1[3]=data[0];
                 for(int i=63;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] * 1) / 1;
                    data1[i+3]=data[i];
                    data1[i]=roundf(data1[i] * 1) / 1;
                }
            }
            for(int i=0;i<32;i++)
            {
                if(data1[i+3]!=0)
                {
                    IFZ1=IFZ1|(1<<(31-i));
                    //printf("%u ",IFZ);
                }
                
            }
            for(int i=32;i<64;i++)
            {
                if(data1[i+3]!=0)
                {
                    IFZ2=IFZ2|(1<<(31-i));
                    //printf("%u ",IFZ);
                }
                
            }
            data1[1]=IFZ1;
            data1[2]=IFZ2;
            
            fprintf(fileout,"%.0f\n",data1[0]);
            fprintf(fileout,"%u\n",IFZ1);
            fprintf(fileout,"%u\n",IFZ2);

            for(int k=0;k<64;k++)
            {
                if(data1[k+3]!=0)
                    fprintf(fileout,"%.0f\t",data1[k+3]);
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
    return 0;
}
