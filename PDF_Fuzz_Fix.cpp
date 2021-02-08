//clang++ -o PDF_Fuzz_Fix PDF_Fuzz_Fix.cpp -framework CoreGraphics -framework CoreFoundation
//VERSION Public 1.2
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

int SHM_init() //공유 메모리 연결
{
    void *shmaddr;
    if((shmid=shmget((key_t)KEY, 0, 0))== -1)
    {
        printf("ShareMemory Connection Error\n");
        return 1;
    }
}

int main(int argc, char *argv[])
{
    SHM_init();
    size_t filesize, count;
    void *shmaddr;
    int Data_Size;
    char *data;
    if((shmaddr=shmat(shmid,(void *)0, 0))==(void*)-1) //공유 메모리 get
    {
        printf("SHM MAT Error\n");
        return 0;
    }
    //memcpy(&Data_Size, (char *)shmaddr, sizeof(int));
    FILE *fp;
    //printf("Data Size : %d\n", *(int*)shmaddr);
    Data_Size = *(int*)shmaddr;
    CFStringRef path;
    CFURLRef url;
    CGPDFDocumentRef document;
    data = (char*)(int*)shmaddr+1;
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, Data_Size, NULL); //provider Create
    document = CGPDFDocumentCreateWithProvider(provider);//provider -> PDF로 Read
    count = CGPDFDocumentGetNumberOfPages(document);//PDF Page Count
    CGDataProviderRelease(provider); //FREE
    if (count != 0){
        printf("Count : [%d]\n", count);
    }
    return 1;
}