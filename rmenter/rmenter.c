#include <stdio.h>
#include <string.h>

#define FIXTXT "./tongniandemimi.txt"
#define RES "./res.txt"

const unsigned char elli[4] = {0xe2, 0x80, 0xa6, 0x0};
const unsigned char winenter[] = {0x0d, 0x0a};

void hex_print(char * str, int len)
{
    if (!str)
        return;
    int i = 0;
    for (i = 0; i < strlen(str) && i < len; i++) {
        printf("%02x ", str[i] & 0xff);
    }
}

int procline(unsigned char* src, unsigned char* dst)
{
    int i = 0, j = 0;
    if (strlen(src) < 2) {
        strncpy(dst, src, 2);
        return 0;
    }
    while(i < strlen(src)) {
        if (src[i] == elli[0] && i + 2 < strlen(src)) {
            if (src[i+1] == elli[1] && src[i+2] == elli[2]) {
                dst[j] = 0x0a;
                break;
            }
        }
        if (src[i] == 0x0d && i + 1 < strlen(src)) {
            if (src[i+1] == 0x0a) {
                if (src[i - 2] == 0x80 && src[i - 1] == 0x82) {
                    dst[j] = 0x0a;
                }
                break;
            }
        }
       dst[j] = src[i];
       i++;
       j++;
    }

    return 0;
        
}

int main()
{
    int line = 0;
    char buff[1024] = {0};
    char res[1024] = {0};
    FILE* fp = fopen(FIXTXT, "r");
    if (!fp) {
        perror("Can not open");
        return -1;
    }
    FILE* wp = fopen(RES, "w");
    if (!wp) {
        perror("Can not open w");
        return -2;
    }
    while (!feof(fp)) {
        if(fgets(buff, 1024, fp) != NULL) {
            procline(buff, res);
            //hex_print(res, 1024);
            fwrite(res, strlen(res),1,wp);
        }
        memset(buff, 0, sizeof(buff));
        memset(res, 0, sizeof(res));
        line++;

        if (line > 5000)
            break;
    }
    
    fclose(fp);
    fclose(wp);
    return 0;
}
