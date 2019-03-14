#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "wave.h"

#define WAVE 1
#define MP3 2
#define AMR 3
#define OTHER 4
#define ERROR 5

extern void wave(wave_t* wav, FILE *fp);
extern void mp3(FILE *fp);
extern void amr(FILE *fp);

int length_wave = 0;
int length_mp3 = 0;
int length_amr = 0;
int length_all = 0;
int num_soft_link = 0;

//通配符检测，用于通过文件名检测文件格式
int match(char *str1, char *str2)
{
    if(str1 == NULL || str2 == NULL)
        return 0 ;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char *tmp_str = str2 ;
    int tmp_len = len2 ;
    int location = 0 ;
    int match_flag = 0 ;
    int i = 0 ;
    while(i<tmp_len) {
        if(tmp_str[i] == '*') {
            location = i+1 ;
            break ;
        }
        i++ ;
    }
    char *tmp_str1 = str1 ;
    char *tmp_str2 = str2 ;
    if(location) {
        location -= 1;
        if(!strncmp(str1, str2, location)) {
            tmp_len = len2 - location ;
            tmp_str1 += len1 ;
            tmp_str2 += len2 ;
            while(tmp_len && (*tmp_str1 == *tmp_str2)){
                tmp_str1 -- ;
                tmp_str2 -- ;
                tmp_len -- ;
            }
            match_flag = tmp_len?0:1 ;
        }
    }
    return match_flag ;
}

//判断文件类型
int type_check(wave_t *wav, char* file_name){
    if (match(file_name,"*.wav") || match(file_name,"*.WAV")){
        if(!strncasecmp(wav->riff.wave_id, "wave", 4)){
        return WAVE;
        }
        else{
        return ERROR;
        }
    }
    else if (match(file_name,"*.mp3") || match(file_name,"*.MP3")){
        if(!strncasecmp((char *)wav, "ID3", 3) || !strncasecmp((char *)wav, "id3", 3)){
        return MP3;
        }
        else{
        return ERROR;
        }
    }
    else if (match(file_name,"*.amr") || match(file_name,"*.AMR") || match(file_name,"*.awb") || match(file_name,"*.AWB")){
        if(!strncasecmp((char *)wav, "#!AMR", 5)){
        return AMR;
        }
        else{
        return ERROR;
        }
    }
    else{
        return OTHER;
    }
}

//音频文件
void ergodic_statistics(char* file_name){
    static char buff[1024];
    FILE *fp = fopen(file_name, "rb");
    if(!fp){
        printf("can't open audio file\n");
        exit(-1);
    }
    fread(buff, 1, sizeof(buff) , fp);
    wave_t *wav = (wave_t *)&buff[0];
    fseek(fp, 0 , SEEK_SET);
    int i = type_check(wav,file_name);
    switch(i)
    {
        case WAVE:
            wave(wav,fp);
            break;
        case MP3:
            mp3(fp);
            break;
        case AMR:
            amr(fp);
            break;
        case OTHER:
            printf("This is an audio file of other types\n\n\n");
            break;
        case ERROR:
            printf("The header error\n\n\n");
    }
}

//主程序
int main(int argc, char* argv[]) {
    char* filename;
    if(argc == 1){
        printf("There is no file\n");
    }
    else{
        filename = argv[1];
        ergodic_statistics(filename);
    }
}

