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
    double data[8];
    double sort[8];
    double data1[11];
    fileout = fopen("stat-diff-2-gas-8-7.txt", "wb");
    if (file = fopen("gas_reading.txt", "r"))
    {
        while (!feof(file))
        {
            double mode;
            double t=0;
            double max=0,c=0;
            unsigned int IFZ=0;
            double flag=-1;
            for(int i=0;i<8;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<8;j++)
            {
                if(fscanf(file, "%lf", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 100) / 100;
                sort[j]=data[j];
                sort[j]=roundf(sort[j] * 100) / 100;
                //printf("%d ",sort[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    if(sort[i]<sort[j])
                    {
                        t=sort[i];
                        sort[i]=sort[j];
                        sort[j]=t;
                    }
                }
            }   
            for(int i=0; i<8; i++)
            {
                t=sort[i];
                c=0;   
                for(int j=0; j<8; j++) 
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
            if(max>=5)
            {
                data1[0]=1;
                data1[1]=mode;
                for(int i=0;i<8;i++)
                {
                    data1[i+3]=data[i]-mode;
                    data1[i]=roundf(data1[i] * 100) / 100;
                    //printf("%d ",data1[i+2]);
                }
                for(int i=0;i<8;i++)
                {
                    if(data1[i+3]!=0)
                    {
                        IFZ=IFZ|(1<<(7-i));
                        //printf("%u ",IFZ);
                    }
                
                }
                data1[2]=IFZ;
                
                fprintf(fileout,"%d\t",1);
                fprintf(fileout,"%.2f\t",data1[1]);
                fprintf(fileout,"%u\t",IFZ);
            
                for(int k=0;k<8;k++)
                {
                    
                    if(data1[k+3]!=0)
                        fprintf(fileout,"%.2f\t",data1[k+3]);
                }
            }
            else
            {
                data1[0]=0;
                data1[1]=data[0];
                
                for(int i=7;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] * 100) / 100;
                    data1[i+1]=data[i];
                    data1[i]=roundf(data1[i] * 100) / 100;
                }
                fprintf(fileout,"%d\t",data1[0]);
                for(int k=1;k<=8;k++)
                {
                    fprintf(fileout,"%.2f\t",data1[k]);
                }
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
