#include<stdio.h>
#include <string.h>
#include<math.h>

void main()
{
    FILE *file1,*file2,*fout;
    unsigned long int File_Size;
    float data;
    unsigned int IFZ1,IFZ2;
    unsigned int IFZ[64];
    file1 = fopen("gys-seperated.txt", "wb");
    file2 = fopen("watch-acc-seperated.txt", "wb");
    if(fout = fopen("gys+watch-acc.txt", "r"))
    {
        int count1=0,count2=0;
        fscanf(fout, "%d", &File_Size);
        //printf("%d ",File_Size);
        while(count1<File_Size)
        {
            if(fscanf(fout, "%f", &data)!=EOF)
            {
                count1++;
                fprintf(file1,"%.2f\n",data);
            }
            if(fscanf(fout, "%d", &IFZ1)!=EOF)
            {
                count1++;
                fprintf(file1,"%u\n",IFZ1);
                //printf("%u ",IFZ1);
            }
            if(fscanf(fout, "%u", &IFZ2)!=EOF)
            {
                count1++;
                fprintf(file1,"%u\n",IFZ2);
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
                    if(fscanf(fout, "%f", &data)!=EOF)
                    {
                        data=roundf(data * 100) / 100;
                        fprintf(file1,"%.2f\n",data);
                        count1++;
                    }
                }
            }
        }
        fscanf(fout, "%d", &File_Size);
        while(count2<File_Size)
        {
            if(fscanf(fout, "%f", &data)!=EOF)
            {
                count2++;
                fprintf(file2,"%.2f\n",data);
            }
            if(fscanf(fout, "%u", &IFZ1)!=EOF)
            {
                count2++;
                fprintf(file2,"%u\n",IFZ1);
                
            }
            if(fscanf(fout, "%u", &IFZ2)!=EOF)
            {
                count2++;
                fprintf(file2,"%u\n",IFZ2);
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
                    if(fscanf(fout, "%f", &data)!=EOF)
                    {
                        data=roundf(data * 100) / 100;
                        fprintf(file2,"%.2f\n",data);
                        count2++;
                    }
                }
            }
        }
    }
}