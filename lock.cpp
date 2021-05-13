#include<iostream>
#include<string.h>
using namespace std;
struct heap{
    long long val;
    int ch;
} heap[256];
struct node{
    int lc=-1, rc=-1, par=-1;
    string route="";
} node[512];
long long count[256]={0};
void sift(int l,int &r)
{
    int i = l, j = 2 * i + 1;
    struct heap x = heap[i];
    while(j<r)
    {
        if (j + 1 < r&&heap[j+1].val < heap[j].val)
            j++;
        if(heap[j].val>=x.val)
            break;
        heap[i] = heap[j];
        i = j;
        j = 2 * i + 1;
    }
    heap[i] = x;
}
int main(int argc,char*argv[])
{
    char temp1[100] = ".\\";
    char temp2[100] = ".\\";
    strcat(temp1, argv[1]);
    strcat(temp2, argv[1]);
    strcat(temp2, ".lock");
    const char*input_filename = temp1;
    const char*output_filename=temp2;
    FILE *fp = fopen(input_filename, "rb");
    long long total_cnt = 0;//总共的字符数量
    int visit[256] = {0};
    int n = 0;
    while(1)
    {
        unsigned char c;
        fread(&c, 1, 1, fp);
        if(feof(fp))
            break;
        total_cnt++;
        if(!visit[(int)c])
        {
            visit[(int)c] = 1;
            n++;
            heap[(int)c].ch = (int)c;
        }
        heap[(int)c].val++;
        count[(int)c]++;
    }
    int k = 0;
    int m = 256;
    int t = n-1;
    for (int i = 0; i < 256;i++)
    {
        if(visit[i])
            heap[k++] = heap[i];
    }
    for (int i = n / 2 - 1; i >= 0;i--)
        sift(i, n);
    for (int i = 0; i < n-1;i++)
    {
        int j = 1;
        if(2<=t&&heap[2].val<heap[1].val)
            j = 2;
        heap[0].val+=heap[j].val;
        node[m].lc = heap[0].ch, node[m].rc = heap[j].ch;
        node[heap[0].ch].par = m;
        node[heap[j].ch].par = m;
        heap[0].ch = m;
        swap(heap[j], heap[t]);
        sift(j, t);
        sift(0, t);
        t--;
        m++;
    }
    for (int i = 0; i < 256;i++)
    {
        if(visit[i])
        {
            int j = i;
            while(node[j].par!=-1)
            {
                if(node[node[j].par].lc==j)
                    node[i].route += "0";
                else
                    node[i].route += "1";
                j = node[j].par;
            }
            int l = 0, r = (int)node[i].route.size() - 1;
            while(l<r)
                swap(node[i].route[l++], node[i].route[r--]);
        }
    }
    fseek(fp, 0, SEEK_SET);
    string buf = "";
    FILE *fp2 = fopen(output_filename, "wb");
    unsigned char temp = n-1;
    fwrite(&temp, 1, 1, fp2);
    for (int i = 0; i < 256;i++)
    {
        if(visit[i])
        {
            unsigned char c;
            c = i;
            fwrite(&c, 1,1,fp2);
            long long d;
            d = count[i];
            fwrite(&d, sizeof(long long), 1, fp2);
        }
    }

        while (1)
        {
            unsigned char c;
            fread(&c, 1, 1, fp);
            if (feof(fp))
                break;
            buf += node[(int)c].route;
            while (buf.size() >= 8)
            {
                unsigned char res = 0;
                for (int i = 0; i < 8; i++)
                    res = res << 1 | (buf[i] - '0');
                fwrite(&res, 1, 1, fp2);
                buf = buf.substr(8, buf.size() - 8);
            }
        }
        if(buf.size())
        {
            while(buf.size()<8)
                buf += "0";
        }
        unsigned char res = 0;
        for (int i = 0; i < 8; i++)
            res = res << 1 | (buf[i] - '0');
        fwrite(&res, 1, 1, fp2);
        //补0
        fclose(fp);
        fclose(fp2);
        system("pause");
}