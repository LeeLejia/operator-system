#include <sys/types.h>
#include <stdio.h>
#include <cstdlib>
#define BUF_SIZE 2

int main(void)
{
    char buf[BUF_SIZE]={0};
    FILE *source, *backup;
    if(!(source=fopen("/home/cjwddz/桌面/实验/oS/hw1/source.dat", "r"))) {
        printf("Error in opening file.\n");
        exit(1);
    }
    if(!(backup=fopen("/home/cjwddz/桌面/实验/oS/hw1/backup.dat", "w"))) {
        printf("Error in creating file.\n");
        exit(1);
    }
    int r;
    do{
        r = fread(&buf, sizeof(buf), 1, source);
        printf("readed %s\n",buf);
        if(r==1 && fwrite(buf, sizeof(buf), 1,backup)!=1) {
            printf("Error in wrinting file.\n");
            exit(1);
        }
    }while(r==1);
    fclose(source);
    fclose(backup);
    exit(0);
}