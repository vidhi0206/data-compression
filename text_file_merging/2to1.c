#include <stdio.h>
#include <string.h>
#include<math.h>
void main()
{
    FILE *file1,*file2,*fout;
    float data;
    fout = fopen("BVP+EDA.txt", "wb");
    if ((file1 = fopen("stat-diff-II-2-BVP-64-50.txt", "r")) && (file2=fopen("stat-diff-II-2-EDA-64-50.txt", "r")))
    {
        int count1=0,count2=0;
        while(fscanf(file1, "%f", &data)!=EOF)
        {
            count1++;
        }
        fprintf(fout,"%d\n",count1);
        rewind(file1);
        while(fscanf(file1, "%f", &data)!=EOF)
        {
            data=roundf(data * 100) / 100;
            fprintf(fout,"%.2f\n",data);
        }
        while(fscanf(file2, "%f", &data)!=EOF)
        {
            count2++;
        }
        fprintf(fout,"%d\n",count2);
        rewind(file2);
        while(fscanf(file2, "%f", &data)!=EOF)
        {
            
            fprintf(fout,"%f\n",data);
            
        }
    }
    fclose(file1);
    fclose(file2);
    fclose(fout);
}