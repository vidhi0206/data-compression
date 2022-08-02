#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
int main(void)
{
    FILE *file,*fileout;
    float data[16];
    float sort[16];
    float data1[19];
    fileout = fopen("stat-diff-output-opt-EDA-7.txt", "wb");
    if (file = fopen("EDA.txt", "r"))
    {
        while (!feof(file))
        {
            float mode;
            float t=0;
            float max=0,c=0;
            unsigned int IFZ=0;
            float flag=-1;
            for(int i=0;i<16;i++)
            {
                data[i]=-1;
                sort[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%f", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                sort[j]=data[j];
                //printf("%d ",sort[j]);
            }
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
            if(max>=7)
            {
                data1[0]=1;
                data1[1]=mode;
                for(int i=0;i<16;i++)
                {
                    data1[i+3]=data[i]-mode;
                    data1[i]=roundf(data1[i] * 1000) / 1000;
                    //printf("%d ",data1[i+2]);
                }
                for(int i=0;i<16;i++)
                {
                    if(data1[i+3]!=0)
                    {
                        IFZ=IFZ|(1<<(15-i));
                        //printf("%u ",IFZ);
                    }
                
                }
                data1[2]=IFZ;
                
                fprintf(fileout,"%d\t",1);
                fprintf(fileout,"%.3f\t",data1[1]);
                fprintf(fileout,"%u\t",IFZ);
            
                for(int k=0;k<16;k++)
                {
                    
                    if(data1[k+3]!=0.00)
                        fprintf(fileout,"%.3f\t",data1[k+3]);
                }
            }
            else
            {
                data1[0]=0;
                data1[1]=data[0];
                
                for(int i=15;i>0;i--)
                {
                    data[i]=data[i]-data[i-1];
                    data[i]=roundf(data[i] * 1000) / 1000;
                    data1[i+1]=data[i];
                    data1[i]=roundf(data1[i] * 1000) / 1000;
                }
                fprintf(fileout,"%d\t",data1[0]);
                for(int k=1;k<=16;k++)
                {
                    fprintf(fileout,"%.3f\t",data1[k]);
                }
            }
            
        }
        fclose(fileout);
        fclose(file);
    }

    return 0;
}
