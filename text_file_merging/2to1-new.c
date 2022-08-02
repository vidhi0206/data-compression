#include <stdio.h>
#include <string.h>
#include<math.h>
void main()
{
    FILE *file1,*file2,*fout;
    float data;
    float c;
    unsigned int IFZ1,IFZ2;
    unsigned int IFZ[64];
    fout = fopen("gys+watch-acc.txt", "wb");
    if ((file1 = fopen("stat-diff-II-2-gys-64-50.txt", "r")) && (file2=fopen("stat-diff-II-2-watch-acc-64-50.txt", "r")))
    {
        int count1=0,count2=0;
        while(fscanf(file1, "%f", &c)!=EOF)
        {
            count1++;
        }
        fprintf(fout,"%d\n",count1);
        //printf("P ");
        rewind(file1);
        while(count1>0)
        {
            //printf("%d ",count1);
            if(fscanf(file1, "%f", &data)!=EOF)
            {
                count1--;
                fprintf(fout,"%.2f\n",data);
                //printf("%f ",data);
            }
            if(fscanf(file1, "%u", &IFZ1)!=EOF)
            {
                count1--;
                fprintf(fout,"%u\n",IFZ1);
                //printf("%u ",IFZ1);
            }
            if(fscanf(file1, "%u", &IFZ2)!=EOF)
            {
                count1--;
                fprintf(fout,"%u\n",IFZ2);
            }
            for(int i=0;i<32;i++)
            {
                IFZ[i]=IFZ1&(1<<(31-i));
                
            }
            for(int i=0;i<32;i++)
            {
                IFZ[i+32]=IFZ2&(1<<(31-i));
                //printf("%d ",IFZ2);
            }
            for(int j=0;j<64;j++)
            {
                //printf("%d ",IFZ[j]);
                if(IFZ[j]!=0)
                {
                    if(fscanf(file1, "%f", &data)!=EOF)
                    {
                        data=roundf(data * 100) / 100;
                        fprintf(fout,"%.2f\n",data);
                        count1--;
                    }
                }
            }
        }
        //printf("%d ",count1);
        while(fscanf(file2, "%f", &c)!=EOF)
        {
            count2++;
        }
        fprintf(fout,"%d\n",count2);
        //printf("pr ");
        rewind(file2);
        while(count2>0)
        {
            //printf("%d ", count2);
            if(fscanf(file2, "%f", &data)!=EOF)
            {
                count2--;
                fprintf(fout,"%.2f\n",data);
                //printf(" h ");
            }
            if(fscanf(file2, "%u", &IFZ1)!=EOF)
            {
                count2--;
                fprintf(fout,"%u\n",IFZ1);
            }
            if(fscanf(file2, "%u", &IFZ2)!=EOF)
            {
                count2--;
                fprintf(fout,"%u\n",IFZ2);
            }
            for(int i=0;i<32;i++)
            {
                IFZ[i]=IFZ1&(1<<(31-i));
            }
            for(int i=0;i<32;i++)
            {
                IFZ[i+32]=IFZ2&(1<<(31-i));
            }
            for(int j=0;j<64;j++)
            {
                if(IFZ[j]!=0)
                {
                    if(fscanf(file2, "%f", &data)!=EOF)
                    {
                        data=roundf(data * 100) / 100;
                        fprintf(fout,"%.2f\n",data);
                        count2--;
                    }
                }
            }
        }
    }
    else{
        printf("file not open.");
    }
    fclose(file1);
    fclose(file2);
    fclose(fout);
}