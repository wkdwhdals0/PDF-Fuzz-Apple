//clang++ -o PDF_Fuzz_Beta PDF_Fuzz_Beta.cpp -framework CoreGraphics -framework CoreFoundation
//VERSION BETA!
#include <iostream>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CFTree.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CoreFoundation.h>
#include <time.h>
#include <stdlib.h>

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
    mutate_data(data, filesize);
    fp = fopen("test.pdf", "wb");
    fwrite(data, 1, filesize, fp);
    fclose(fp);
    while(1) 
    {
        char filename[20] = "test.pdf";
        CFStringRef path;
        CFURLRef url;
        CGPDFDocumentRef document;
        size_t count;
        path = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
        url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, 0);//1 Create CFURL object
        CFRelease(path);
        document = CGPDFDocumentCreateWithURL(url);//2 Create a CFPDFDocument object
        CFRelease(url);
        count = CGPDFDocumentGetNumberOfPages(document);//3 Get the number of document pages
        if (count == 0) {
            printf("`%s' needs at least onepage!", filename);
            return 0;
        }
        printf("%d\n", count);
        if (!count)
        {
            break;
        }
    }
    return 1;
}