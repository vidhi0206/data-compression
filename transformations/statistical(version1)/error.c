#include <stdio.h>
#include <string.h>
#include<math.h>
#include<limits.h>
void main()
{
    FILE *file,*fileout,*file1;
    float data1[16];
    float data2[16];
    float error[16];
    fileout = fopen("error.txt", "wb");
    file1= fopen("decoded.txt","r");
    if (file = fopen("gas_reading.txt", "r"))
    {
        int flag=-1;
        while (!feof(file) && !feof(file1))
        {
            for(int i=0;i<16;i++)
            {
                data1[i]=-1;
                data2[i]=-1;
                error[i]=INT_MIN;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(file, "%f", &data1[j])==EOF)
                {
                    flag=j;
                    break;
                }
                if(fscanf(file1, "%f", &data2[j])==EOF)
                {
                    flag=j;
                    break;
                }
                //data[j]=roundf(data[j] * 100) / 100;
                //printf("%d ",data[j]);
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=0;i<16;i++)
            {
                error[i]=data1[i]-data2[i];
                //data[i]=roundf(data[i] * 100) / 100;
            }
            for(int k=0;k<16;k++)
            {
                
                fprintf(fileout,"%f\n",error[k]);
            }
        }
        fclose(fileout);
        fclose(file);
        
    }
}