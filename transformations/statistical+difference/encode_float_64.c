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
    double data1[68];
    fileout = fopen("stat-diff-0-BVP-64-40.txt", "wb");
    if (file = fopen("BVP.txt", "r"))
    {
        while (!feof(file))
        {
            double mode;
            double t=0;
            double max=0,c=0;
            unsigned int IFZ1=0, IFZ2=0;
            double flag=-1;
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
                data[j]=roundf(data[j] * 10 )/10 ;
                sort[j]=data[j];
                sort[j]=roundf(sort[j] *10)/10 ;
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
            if(max>=40)
            {
                data1[0]=1;
                data1[1]=mode;
                for(int i=0;i<64;i++)
                {
                    data1[i+4]=data[i]-mode;
                    data1[i]=roundf(data1[i] *10) /10;
                    //printf("%.4f ",data1[i+4]);
                }
                for(int i=0;i<32;i++)
                {
                    if(data1[i+4]!=0)
                    {
                        IFZ1=IFZ1|(1<<(31-i));
                        //printf("%u ",IFZ1);
                    }
                
                }
                for(int i=32;i<64;i++)
                {
                    if(data1[i+4]!=0)
                    {
                        IFZ2=IFZ2|(1<<(31-i));
                        //printf("%u ",IFZ2);
                    }
                
                }
            

                data1[2]=IFZ1;
                data1[3]=IFZ2;
                
                fprintf(fileout,"%d\t",1);
                fprintf(fileout,"%.1f\t",data1[1]);
                fprintf(fileout,"%u\t",IFZ1);
                fprintf(fileout,"%u\t",IFZ2);
            
                for(int k=0;k<64;k++)
                {
                    
                    if(data1[k+4]!=0)
                        fprintf(fileout,"%.1f\t",data1[k+4]);
                }
            }
            else
            {
                data1[0]=0;
                data1[1]=data[0];
                
                for(int i=63;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] *10)/10 ;
                    data1[i+1]=data[i];
                    data1[i]=roundf(data1[i]*10 )/10 ;
                }
                fprintf(fileout,"%d\t",data1[0]);
                for(int k=1;k<=64;k++)
                {
                    fprintf(fileout,"%.1f\t",data1[k]);
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
