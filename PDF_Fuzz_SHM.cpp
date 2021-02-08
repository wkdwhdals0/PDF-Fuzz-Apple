//clang++ -o PDF_Fuzz_SHM PDF_Fuzz_SHM.cpp -framework CoreGraphics -framework CoreFoundation
//VERSION Public 0.5
//Issue -> 뮤테이션을 2회 이상하면 데이터가 읽히지 않늗다. 뮤테이션 코드 필요
#include <iostream>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CFTree.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGDataProvider.h>
#include <CoreFoundation/CFData.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#define SIZE 10485760
#define KEY 1229

int shmid;

int SHM_init()
{
    void *shmaddr;
    if((shmid=shmget((key_t)KEY, 0, 0))== -1)
    {
        printf("ShareMemory Connectiong Error\n");
        return 1;
    }
}

int SHM_Write(char *data, int size)
{

    void *shmaddr;
    if((shmaddr=shmat(shmid, (void*)0, 0))==(void *)-1)
    {
        printf("Write Error\n");
        return 1;
    }
    printf("memcpy start %d\n", size);
    memcpy(shmaddr, &size, sizeof(int));
    memcpy((int*)shmaddr+1, (char *)data, size);
    //printf("[test]%d %d\n", (int*)shmaddr, *(int*)(shmaddr));
    //printf("[test]%d %s\n", (int*)shmaddr+1, *((int*)(shmaddr)+1));
    if(shmdt(shmaddr)== -1)
    {
        printf("Write Error\n");
        return 1;
    }
    printf("SHM Write Success\n");
    return 0;
}

size_t rand_value(size_t min, size_t max) {
    return rand() % (max+1-min) + min;
}

void mutate_data(char *data, size_t size) {
    size_t iterate_count = size*0.1; //10%
    size_t offset = 0;

    for(int i=0;i<200;i++) {
        offset = rand_value(0, size);
        if(rand_value(0,1)) {
            //random byte
            data[offset] = (uint8_t)rand_value(0,0xff);
        } else {
            data[offset] = rand_value(0,1) ? data[offset]+1 : data[offset]-1;
        }
    }
    //bit slice ?
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[2048], *data;
    size_t filesize;
    int rand_value = 0x0;
    int fd;
    if(argc!=2)
    {
        fprintf(stderr, "no args\n");
        return -1;
    }
    if(SHM_init())
    {
        return -1;
    }
    fd = open("/dev/urandom", O_RDONLY);
    read(fd, &rand_value, 4);
    close(fd);
    fp = fopen(argv[1], "rb");
    if(!fp)
    {
        fprintf(stderr, "No Such File\n");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    printf("FILE Size : %d\n", filesize);
    fseek(fp, 0, SEEK_SET);
    data = (char*)malloc(filesize+1);
    memset(data, '\x00', filesize+1);
    while(1)
    {
        size_t r = fread(data, 1, filesize, fp);
        if(r==0)
            break;
    }
    fclose(fp);
    int n=1;
    while(1) 
    {
        CFStringRef path;
        CFURLRef url;
        CGPDFDocumentRef document;
        size_t count;
        mutate_data(data, filesize);
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, filesize, NULL);
        document = CGPDFDocumentCreateWithProvider(provider);//2 Create a CFPDFDocument object
        count = CGPDFDocumentGetNumberOfPages(document);//3 Get the number of document pages
        CGDataProviderRelease(provider);
        SHM_Write(data, filesize);
        printf("N[%d]\n", n);
        if (count != 0){
            printf("Count : [%d]\n", count);
        }
        n++;
        break;
    }
    return 1;
}