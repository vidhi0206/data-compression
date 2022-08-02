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
    double data[16];
    double sort[16];
    double data1[18];
    fileout = fopen("stat-2-BVP-5.txt", "wb");
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
            for(int i=0;i<16;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%lf", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 100) / 100;
                sort[j]=data[j];
                sum=sum+data[j];
                

                //printf("%d ",sort[j]);
            }
            mean=(double)sum/16.0;
            //printf("%f ",mean);
            mean=roundf(mean * 100) / 100;
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<16;i++)
            {
                for(int j=0;j<16;j++)
                {
                    if(sort[i]<sort[j])
                    {
                        t=sort[i];
                        sort[i]=sort[j];
                        sort[j]=t;
                    }
                }
            }   
            median=( (sort[7] + sort[8] ) / 2.0);
            median=roundf(median * 100) / 100;
            for(int i=0; i<16; i++)
            {
                t=sort[i];
                c=0;   
                for(int j=0; j<16; j++) 
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
                for(int i=0;i<16;i++)
                {
                    data1[i+2]=data[i]-mode;
                    //printf("%d ",data1[i+2]);
                }
            }
            else
            {
                data1[0]=mean;
                for(int i=0;i<16;i++)
                {
                    data1[i+2]=data[i]-mean;
                    //printf("%d ",data1[i+2]);
                }
            }
            for(int i=0;i<16;i++)
            {
                if(data1[i+2]!=0)
                {
                    IFZ=IFZ|(1<<(15-i));
                    //printf("%u ",IFZ);
                }
                
            }
            data1[1]=IFZ;

            
            fprintf(fileout,"%.2f\n",data1[0]);
            fprintf(fileout,"%u\n",IFZ);
            
            for(int k=0;k<16;k++)
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
