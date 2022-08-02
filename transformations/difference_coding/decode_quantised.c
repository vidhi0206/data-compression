#include<stdio.h>
#include<string.h>
#include<math.h>
int main(void)
{
    FILE *file,*fileout;
    float t[16];

    fileout = fopen("diff-output-EDA-opt.txt", "r");
    if (file = fopen("decoded.txt", "w"))
    {
        while (!feof(file))
        {
            int flag=-1;
            for(int i=0;i<16;i++)
            {
                t[i]=-1;
            }
            for(int j=0;j<16;j++)
            {
                if(fscanf(fileout, "%f", &t[j])==EOF)
                {
                    flag=j;
                    break;
                }
                t[j]=roundf(t[j] * 1000) / 1000;
            }
            if(flag==0 )
            {
                break;
            }
            for(int i=1;i<16;i++)
            {
                t[i]=t[i-1]+t[i];
                t[i]=roundf(t[i] * 1000) / 1000;
            }
            for(int k=0;k<16;k++)
            {
                fprintf(file,"%.3f\n",t[k]);
            }
        }
        fclose(fileout);
        fclose(file);
    }
    
    return 0;
}