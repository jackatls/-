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
int visit[256] = {0};
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
    strcat(temp2, argv[1]);
    argv[1][strlen(argv[1])-5] = '\0';
    strcat(temp1, argv[1]);
    const char*output_filename=temp2;
    const char *recover_filename = temp1;
    FILE *fp2 = fopen(output_filename, "rb");
    FILE *fp3 = fopen(recover_filename, "wb");
    int SUM = 0;
    unsigned char temp;
    fread(&temp, 1,1,fp2);
    int n = temp+1;

    for (int i = 0; i < n;i++)
    {
        unsigned char pos;
        long long cnt;
        fread(&pos, 1, 1,fp2);
        fread(&cnt, sizeof(long long), 1, fp2);
        visit[pos] = 1;
        heap[pos].val = cnt;
        heap[pos].ch = pos;
    }
        for (int i = 0; i < 256; i++)
            SUM += heap[i].val;
    int k = 0;
    for (int i = 0; i < 256; i++)
    {
        if(visit[i])
            heap[k++] = heap[i];
    }
        int m = 256;
    int t = n-1;
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
    
        for (int i = 0; i < 256; i++)
        {
            if (visit[i])
            {
                int j = i;
                while (node[j].par != -1)
                {
                    if (node[node[j].par].lc == j)
                        node[i].route += "0";
                    else
                        node[i].route += "1";
                    j = node[j].par;
                }
                int l = 0, r = (int)node[i].route.size() - 1;
                while (l < r)
                    swap(node[i].route[l++], node[i].route[r--]);
            }
        }
        string buf = "";
    int root = 254 + n;
    int cur = root;
    int CNT = 0;
    int st = 0;
        while (1)
        {
            unsigned char c;
            fread(&c, 1, 1, fp2);
            if (feof(fp2))
                break;
            string hex(8,'0');
            int ed = 7;
            int w = c;
            while(w)
            {
                hex[ed--]=w%2+'0';
                w>>=1;
            }
            buf += hex;

            while(st<(int)buf.size())
            {
                if(buf[st]=='0')
                    cur = node[cur].lc;
                else
                    cur = node[cur].rc;
                if(node[cur].lc==-1)
                {
                    unsigned char d = cur;
                    fwrite(&d, 1, 1, fp3);
                    buf = buf.substr(st+1, (int)buf.size() - st-1);
                    st = -1;
                    cur = root;
                    CNT++;
                    if(CNT==SUM)
                        break;
                }
                st++;
            }
            if(CNT==SUM)
                break;
        }
    fclose(fp2);
    fclose(fp3);
    system("pause");
}