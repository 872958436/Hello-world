#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "md5.h"  
#include "aes.h"

int main(int argc, char *argv[])  
{  
    if(argc < 4)
    {
        printf("usage:./test passwd intput_file output_file 0_or_1(0 is cipher,  1 is invcipher)\n");
        return -1;
    }
    int i;  
    unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3  
    unsigned char decrypt[16];      
    MD5_CTX md5;  
    MD5Init(&md5);                

    MD5Update(&md5,argv[1],strlen(argv[1]));
    MD5Final(&md5,decrypt); //计算MD5值作为密码

    init_environment();
    key_expansion(decrypt, w);

    char buff1[16] = {0};
    char buff2[16] = {0};
    int fd1 =open(argv[2], O_RDONLY, 0644);
    if(fd1 < 0)
    {
        printf("file %s not exist\n",argv[2]);
        exit(0);
    }

    struct stat stat_buf;
    fstat(fd1, &stat_buf);
    uint32_t file_len = stat_buf.st_size;

    int fd2 =open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, stat_buf.st_mode);

    memset(buff1, 0 ,sizeof(buff1));
    memset(buff2, 0 ,sizeof(buff2));

    int ret;
    uint8_t flag = 1;//  p判断是否刚好为16个字节数的整数倍
    while(ret = read(fd1, buff1, sizeof(buff1)))
    {
        memset(buff2, 0 ,sizeof(buff2));
        if(argc == 4 || atoi(argv[4]) == 0)
        {
            if(ret < sizeof(buff1))// 填充方式PKCS+5           
            {
                int i;
                for(i = ret; i < 16;i++)
                    buff1[i]=16-ret;
                flag = 0;
            }
            cipher(buff1/* in */, buff2/* out */, w /* expanded key */);
        }
        else
        {
            inv_cipher(buff1/* in */, buff2/* out */, w /* expanded key */);
        }
        write(fd2, buff2, sizeof(buff2));
        memset(buff1, 0 ,sizeof(buff1));
    }
    if(argc == 4 || atoi(argv[4]) == 0)
    {
        if(flag)
        {
            memset(buff1, 16,sizeof(buff1));
            cipher(buff1/* in */, buff2/* out */, w /* expanded key */);
            write(fd2, buff2, sizeof(buff2));
        }
    }
    else
    {
        ftruncate(fd2, file_len-buff2[15]);
    }
    close(fd1);
    close(fd2);
    exit(0);
}
