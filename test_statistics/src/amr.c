#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"amr.h"

extern int length_amr;

double get_bitrate(int row, int ft) {                     //��ñ�����
    double bitrate_table[2][9] = {
        {4.75,5.15,5.90,6.70,7.40,7.95,10.20,12.20,1.80},
        {6.60,8.85,12.65,14.25,15.85,18.25,19.85,23.05,23.85}
    };
    return bitrate_table[row - 1][ft];
}

int  get_frame_size(int row, int ft) {                    //���֡�ֽڴ�С
    int frame_size_table[2][9] = {
        {13,14,16,18,20,21,27,32,6},
        {18,24,33,37,41,47,51,59,61}
    };
    return frame_size_table[row - 1][ft];
}

int get_head_size(FILE *fp){                              //����ļ�ͷ�ֽڴ�С
    fread(file_head, 19, 1, fp);
    int i;
    for (i = 0;i < 15;i++) {                               //�жϸ�ʽ����
        if (file_head[i] == '\n') {
            i++;
            break;
        }
    }
    if (i == 6) {
        if(!strncasecmp(file_head,"#!AMR\n",6) || !strncasecmp(file_head,"#!amr\n",6)){
            printf("This is AMR-NB Mono\n");
        }
        else{
            printf("The word of header error");
            exit(-1);
        }
    }
    else if (i == 9) {
        if(!strncasecmp(file_head,"#!AMR-WB\n",6) || !strncasecmp(file_head,"#!amr-wb\n",6)){
            printf("This is AMR-WB Mono\n");
        }
        else{
            printf("The word of header error");
            exit(-1);
        }
    }
    else if (i == 12) {
        channel_number = file_head[15] & 0xF;
        if(!strncasecmp(file_head,"#!AMR_MC1.0\n",6) || !strncasecmp(file_head,"#!amr_mc1.0\n",6)){
            printf("This is AMR-NB with %d channels", channel_number);
            i = i + 4;
        }
        else{
            printf("The word of header error");
            exit(-1);
        }
    }
    else if (i == 15) {
        channel_number = file_head[18] & 0xF;
        if(!strncasecmp(file_head,"#!AMR-WB_MC1.0\n",6) || !strncasecmp(file_head,"#!amr-wb_mc1.0\n",6)){
            printf("This is AMR-WB with %d channels", channel_number);
            i = i + 4;
        }
        else{
            printf("The word of header error");
            exit(-1);
        }
    }
    else {
        printf("ERROR:This is not a complete arm file");
        exit(-1);
    }
    return i;
}


void amr(FILE *fp) {
    char   re_frame_head[1];                            //�����֡�û���֡ͷ
    double  bitrate;                                    //������
    char   frame_head[1];                               //֡ͷ�����֡�û���
    char   frames[1024];                                //֡
    int frame_size;                                     //֡�ֽڴ�С
    int head_size;                                      //�ļ�ͷ��ռ�ֽڴ�С
    int complete_frame_number;                          //��֡������
    complete_frame_number = 0;
    int row;                                            //��ά�������
    int ft;                                             //FTֵ����ά�������-1
    int q;                                              //Qֵ
    int length;                                         //���ȣ������룩
    int p;                                              //֡ͷ��һλ
    int pp;                                             //֡ͷ�����λ
    int i = 1;
    int j = 1;
    int k = 1;
    int m = 1;
    int symbol = 0;                                     //��ȷ�ļ���־

    head_size = get_head_size(fp);
    printf("FILE_HEAD_SIZE:\t%d bytes\n", head_size);
    fseek(fp, head_size, SEEK_SET);                                         //�����ļ�ͷ

FIND_COMPLETE_FRAME:                                               //�Һ�֡
    fread(frames, 1, sizeof(frame), fp);
    frame_head[0] = frames[0];
    q = (frame_head[0] & 0x4) >> 2;
    p = (frame_head[0] & 0x80) >> 7;
    pp = (frame_head[0] & 0x3);
    ft = (frame_head[0]) >> 3;
    if ((head_size == 6 || head_size == 16) && q == 1 && p == 0 && pp == 0 && \
            (ft==0 || ft==1 || ft==2 || ft==3 || ft==4 || ft==5 || ft ==6 || ft==7)) {                  //AMR-NB��֡
        row = 1;
        bitrate = get_bitrate(row, ft);
        printf("Bitrate:\t%.2f kbit/s\n", bitrate);
        frame_size = get_frame_size(row, ft);
        printf("FrameSize:\t%d bytes\n", frame_size);
        complete_frame_number++;
    }
    else if ((head_size == 9 || head_size == 19) && q == 1 && p == 0 && pp == 0 && \
            (ft==0 || ft==1 || ft==2 || ft==3 || ft==4 || ft==5 || ft ==6 || ft==7 || ft==8)) {         //AMR-WB��֡
        row = 2;
        bitrate = get_bitrate(row, ft);
        printf("Bitrate:\t%.2f kbit/s\n", bitrate);
        frame_size = get_frame_size(row, ft);
        printf("FrameSize:\t%d bytes\n", frame_size);
        complete_frame_number++;                                   //��֡����+1
    }
    else if((head_size == 6 || head_size == 16) && q == 1 && p == 0 \
            && pp == 0 && (ft==8 || ft==9 || ft==10 || ft==11 )){                   //SID
        symbol = 1;
        while(k){
            printf("There is a SID Frame\n");
            k--;
        }
        goto FIND_COMPLETE_FRAME;                                    //����һ�ֽڣ��ҵ���֡
    }
    else if((head_size == 9 || head_size == 19) && q == 1 && p == 0 \
            && pp == 0 && ft==9){                                                //Comfort Noise Frame
        symbol = 1;
        while(m){
            printf("There is a Comfort Noise Frame\n");
            m--;
        }
        goto FIND_COMPLETE_FRAME;                                    //����һ�ֽڣ��ҵ���֡
    }
    else {                                                           //��֡
        while(i){
            symbol = 1;
            printf("There is a wrong frame\n");
            i--;
        }
        goto FIND_COMPLETE_FRAME;                                    //����һ�ֽڣ��ҵ���֡
    }

    while (!feof(fp)) {
        fseek(fp, frame_size - 1, SEEK_CUR);                               //����һ֡֡ͷ��
RE_FIND_COMPLETE_FRAME:
        fread(frames, 1, sizeof(frame), fp);
        re_frame_head[0] = frames[0];
        ft = (re_frame_head[0]) >> 3;
        q = (re_frame_head[0] & 0x4) >> 2;
        p = (re_frame_head[0] & 0x80) >> 7;
        pp = (re_frame_head[0] & 0x3);
        if (re_frame_head[0] == frame_head[0]) {                         //��֡
            complete_frame_number++;                                      //��֡����+1
        }
        else if((head_size == 6 || head_size == 16) && (ft==0 || ft==1 || ft==2 || ft==3 \
                    || ft==4 || ft==5 || ft ==6 || ft==7) && q==1 && p==0 && pp==0){        //amr-nb������ģʽ�ı�
            symbol = 1;
            printf("The mode of sampling rate has changed.\n");
            bitrate = get_bitrate(row, ft);
            printf("Bitrate:\t%.2f kbit/s\n", bitrate);
            frame_size = get_frame_size(row, ft);
            printf("FrameSize:\t%d bytes\n", frame_size);
            complete_frame_number++;                                   //��֡����+1
            frame_head[0] = re_frame_head[0];
        }
        else if((head_size == 9 || head_size == 19) && (ft==0 || ft==1 || ft==2 || ft==3 \
                    || ft==4 || ft==5 || ft ==6 || ft==7 || ft ==8) && q==1 && p==0 && pp==0){        //amr-wb������ģʽ�ı�
            symbol = 1;
            printf("The mode of sampling rate has changed.\n");
            bitrate = get_bitrate(row, ft);
            printf("Bitrate:\t%.2f kbit/s\n", bitrate);
            frame_size = get_frame_size(row, ft);
            printf("FrameSize:\t%d bytes\n", frame_size);
            complete_frame_number++;                                   //��֡����+1
            frame_head[0] = re_frame_head[0];
        }
        else if((head_size == 6 || head_size == 16) && q == 1 && p == 0 \
                && pp == 0 && (ft==8 || ft==9 || ft==10 || ft==11 )){                   //SID
            symbol = 1;
            while(k){
                printf("There is a SID Frame\n");
                k--;
            }
            goto RE_FIND_COMPLETE_FRAME;                                    //����һ�ֽڣ��ҵ���֡
        }
        else if((head_size == 9 || head_size == 19) && q == 1 && p == 0 \
                && pp == 0 && ft==9){                                                //Comfort Noise Frame
            symbol = 1;
            while(m){
                printf("There is a Comfort Noise Frame\n");
                m--;
            }
            goto RE_FIND_COMPLETE_FRAME;                                    //����һ�ֽڣ��ҵ���֡
        }
        else{                                                                 //��֡
            symbol = 1;
            while(j){
                printf("There is a bad frame or the mode of sampling rate error\n");
                j--;
            }
            goto RE_FIND_COMPLETE_FRAME;                             //����һ�ֽڣ��ҵ���֡
        }
    }
    printf("CompleteFrameNumber:%d \n", complete_frame_number);

    length = complete_frame_number * 20 / 1000;
    if(symbol == 1){
    printf("Length:\t\t%d s\n\n\n", length);
    }
    else{
    printf("Length:\t\t%d s\n", length);
    printf("It's a correct amr or awb file\n\n\n");
    }
    length_amr += length;
}
