#include <stdio.h>
#include <string.h>
#include<math.h>

void main()
{
    FILE *file1,*file2,*fout;
    unsigned long int File_Size;
    float data;
    file1 = fopen("BVP-seperated.txt", "wb");
    file2 = fopen("EDA-seperated.txt", "wb");
    if(fout = fopen("BVP+EDA.txt", "r"))
    {
        int count1=0,count2=0;
        fscanf(fout, "%d", &File_Size);
        printf("%d ",File_Size);
        while(count1<File_Size)
        {
            fscanf(fout, "%f", &data);
            fprintf(file1,"%f\n",data);
            count1++;
        }
        fscanf(fout, "%d", &File_Size);
        while(count2<File_Size)
        {
            fscanf(fout, "%f", &data);
            fprintf(file2,"%f\n",data);
            count2++;
        }
    }
}