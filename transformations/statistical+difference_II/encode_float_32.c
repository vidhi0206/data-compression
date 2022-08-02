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
    double data[32];
    double sort[32];
    double data1[34];
    fileout = fopen("stat-diff-II-2-BVP-32-5.txt", "wb");
    if (file = fopen("BVP.txt", "r"))
    {
        while (!feof(fileout))
        {
            double sum=0;
            double mean;
            double median;
            double mode;
            double t=0;
            double max=0,c=0;
            unsigned int IFZ=0;
            int flag=-1;
            for(int i=0;i<32;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<32;j++)
            {
                if(fscanf(file, "%lf", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 100) / 100;
                sort[j]=data[j];
                //printf("%d ",sort[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<32;i++)
            {
                for(int j=0;j<32;j++)
                {
                    if(sort[i]<sort[j])
                    {
                        t=sort[i];
                        sort[i]=sort[j];
                        sort[j]=t;
                    }
                }
            }
            for(int i=0; i<32; i++)
            {
                t=sort[i];
                c=0;   
                for(int j=0; j<32; j++) 
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
            mode=roundf(mode * 100) / 100;
            if(max>=5)
            {
                data1[0]=mode;
                for(int i=0;i<32;i++)
                {
                    data1[i+2]=data[i]-mode;
                    //printf("%d ",data1[i+2]);
                }
            }
            else
            {
                data1[0]=data[0];
                data1[2]=data[0];
                 for(int i=31;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] * 100) / 100;
                    data1[i+2]=data[i];
                    data1[i]=roundf(data1[i] * 100) / 100;
                }
            }
            for(int i=0;i<32;i++)
            {
                if(data1[i+2]!=0)
                {
                    IFZ=IFZ|(1<<(31-i));
                    //printf("%u ",IFZ);
                }
                
            }
            data1[1]=IFZ;

            
            fprintf(fileout,"%.2f\n",data1[0]);
            fprintf(fileout,"%u\n",IFZ);
            
            for(int k=0;k<32;k++)
            {
                if(data1[k+2]!=0)
                    fprintf(fileout,"%.2f\t",data1[k+2]);
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
