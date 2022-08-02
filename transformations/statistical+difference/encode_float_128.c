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
    double data[128];
    double sort[128];
    double data1[134];
    fileout = fopen("stat-diff-2-BVP-128-10.txt", "wb");
    if (file = fopen("BVP.txt", "r"))
    {
        while (!feof(file))
        {
            double mode;
            double t=0;
            double max=0,c=0;
            unsigned int IFZ1=0, IFZ2=0,IFZ3=0,IFZ4=0;
            double flag=-1;
            for(int i=0;i<128;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<128;j++)
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
            for(int i=0;i<128;i++)
            {
                for(int j=0;j<128;j++)
                {
                    if(sort[i]<sort[j])
                    {
                        t=sort[i];
                        sort[i]=sort[j];
                        sort[j]=t;
                    }
                }
            }   
            for(int i=0; i<128; i++)
            {
                t=sort[i];
                c=0;   
                for(int j=0; j<128; j++) 
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
            if(max>=10)
            {
                data1[0]=1;
                data1[1]=mode;
                for(int i=0;i<128;i++)
                {
                    data1[i+6]=data[i]-mode;
                    data1[i]=roundf(data1[i] * 100) / 100;
                    //printf("%.4f ",data1[i+4]);
                }
                for(int i=0;i<32;i++)
                {
                    if(data1[i+6]!=0)
                    {
                        IFZ1=IFZ1|(1<<(31-i));
                        //printf("%u ",IFZ1);
                    }
                
                }
                for(int i=32;i<64;i++)
                {
                    if(data1[i+6]!=0)
                    {
                        IFZ2=IFZ2|(1<<(31-i));
                        //printf("%u ",IFZ2);
                    }
                
                }
                for(int i=64;i<96;i++)
                {
                    if(data1[i+6]!=0)
                    {
                        IFZ3=IFZ3|(1<<(31-i));
                        //printf("%u ",IFZ2);
                    }
                
                }
                for(int i=96;i<128;i++)
                {
                    if(data1[i+6]!=0)
                    {
                        IFZ3=IFZ3|(1<<(31-i));
                        //printf("%u ",IFZ2);
                    }
                
                }

                data1[2]=IFZ1;
                data1[3]=IFZ2;
                data1[4]=IFZ3;
                data1[5]=IFZ4;

                fprintf(fileout,"%d\t",1);
                fprintf(fileout,"%.2f\t",data1[1]);
                fprintf(fileout,"%u\t",IFZ1);
                fprintf(fileout,"%u\t",IFZ2);
                fprintf(fileout,"%u\t",IFZ3);
                fprintf(fileout,"%u\t",IFZ4);
            
                for(int k=0;k<128;k++)
                {
                    
                    if(data1[k+6]!=0)
                        fprintf(fileout,"%.2f\t",data1[k+6]);
                }
            }
            else
            {
                data1[0]=0;
                data1[1]=data[0];
                
                for(int i=127;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] * 100) / 100;
                    data1[i+1]=data[i];
                    data1[i]=roundf(data1[i] * 100) / 100;
                }
                fprintf(fileout,"%d\t",data1[0]);
                for(int k=1;k<=128;k++)
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
