#include <stdio.h>
#include <string.h>
#include <limits.h>
#include<time.h>
#include <memory.h>
#include "windows.h"
#include "psapi.h"
int main(void)
{
    clock_t t;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	t= clock();
    FILE *file,*fileout;
    int data[16];
    int sort[16];
    int data1[18];
    fileout = fopen("output.txt", "wb");
    if (file = fopen("ACM.txt", "r"))
    {
        while (!feof(file))
        {
            int sum=0;
            int mean;
            int median;
            int mode;
            int t=0;
            int max=0,c=0;
            unsigned int IFZ=0;
            int flag=-1;
            for(int i=0;i<16;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%d", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                sort[j]=data[j];
                sum=sum+data[j];
                //printf("%d ",sort[j]);
            }
            mean=(int)sum/16;
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
            
            fprintf(fileout,"%d ",data1[0]);
            fprintf(fileout,"%u ",IFZ);
            
            for(int k=0;k<16;k++)
            {
                if(data1[k+2]!=0)
                    fprintf(fileout,"%d\t",data1[k+2]);
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
