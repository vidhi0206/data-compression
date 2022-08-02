#include <stdio.h>
#include <string.h>
#include<math.h>
void main()
{
    FILE *file,*fileout;
    float data[16];
    fileout = fopen("diff-output-EDA-opt.txt", "wb");
    if (file = fopen("EDA.txt", "r"))
    {
        int flag=-1;
        while (!feof(file))
        {
            for(int i=0;i<16;i++)
            {
                data[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%f", &data[j])==EOF)
                {
                    flag=j;
                    break;
                }
                data[j]=roundf(data[j] * 1000) / 1000;
                //printf("%d ",data[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=15;i>0;i--)
            {
                data[i]=data[i]-data[i-1];
                data[i]=roundf(data[i] * 1000) / 1000;
            }
            for(int k=0;k<16;k++)
            {
                
                fprintf(fileout,"%.3f ",data[k]);
            }
        }
        fclose(fileout);
        fclose(file);
        
    }
}