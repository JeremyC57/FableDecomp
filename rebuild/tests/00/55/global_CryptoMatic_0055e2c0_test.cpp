// Behaviour test for CryptoMatic (0x0055E2C0, anonymous namespace of the scoreboard crypto TU): a self-contained copy of the row
// text and its two same-TU XTEA helpers (an anonymous-namespace function cannot be bound from another TU; the row's text also runs
// for real, linked, in test/00560bb0.cpp through GetDecryptedDocument's mode-1 call). Checks: mode 0 writes the length word and one
// 8-byte block per started 8 bytes and returns that size; the first block is XTEA of the text under the key-derived schedule; mode 1
// restores the text (zero padded) and returns the stored length; a stored length past the buffer is clamped to the whole blocks.
#include <stdio.h>
#include <string.h>
static void EncipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1];
    unsigned long sum=0,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v0+=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
        sum+=delta;
        v1+=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
    }
    w[0]=v0;w[1]=v1;
}
static void DecipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1];
    unsigned long sum=0xc6ef3720,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v1-=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
        sum-=delta;
        v0-=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
    }
    w[0]=v0;w[1]=v1;
}
static int CryptoMatic(const char* data,char* enc,int dlen,const char* key,char mode)
{
    unsigned long keylength=strlen(key);
    char* keycopy=new char[keylength+1];
    unsigned long cryptuse[4]={0xaa9c,0xccc3,0xffcf,0xbbbb};
    long t=4;
    int retsize=0;
    memset(keycopy,0,keylength+1);
    for(int c=0;c<keylength;++c)
        keycopy[c]=(char)(key[keylength-c]*(c+1));
    cryptuse[2]=keycopy[1];
    cryptuse[3]=keycopy[0];
    cryptuse[1]=keycopy[2];
    cryptuse[0]=keycopy[3];
    char* oldenc=enc;
    if(mode==0) {
        memcpy(enc,&dlen,4);enc+=4;
    } else {
        memcpy(&retsize,data,4);
        if(retsize>dlen+10) {
            int mod=dlen%8;
            dlen-=mod;
            retsize=dlen;
        }
        data+=4;dlen-=4;
    }
    for(;dlen>=8;dlen-=8) {
        if(mode==0)EncipherMorsel((unsigned long*)data,(unsigned long*)enc,cryptuse);
        else DecipherMorsel((unsigned long*)data,(unsigned long*)enc,cryptuse);
        data+=8;enc+=8;
        if(t>=keylength)t=0;
        cryptuse[0]+=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[1]-=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[2]+=keycopy[t++];
        if(t>=keylength)t=0;
        cryptuse[3]-=keycopy[t++];
    }
    if(dlen>0) {
        unsigned long indata[2]={0,0};
        memcpy(indata,data,dlen);
        if(mode==0)EncipherMorsel(indata,(unsigned long*)enc,cryptuse);
        else DecipherMorsel(indata,(unsigned long*)enc,cryptuse);
        enc+=8;
    }
    if(mode==0)retsize=enc-oldenc;
    delete[] keycopy;
    return retsize;
}

static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    const char* key = "THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM";
    const char* text = "SID:OK\r\nalpha\r\nbeta\r\n";          // 21 bytes -> 3 blocks
    char cipher[64]; memset(cipher, 0x5A, sizeof cipher);
    int n = CryptoMatic(text, cipher, (int)strlen(text), key, 0);
    Check(n == 4 + 24, "mode 0 size");
    int stored; memcpy(&stored, cipher, 4); Check(stored == 21, "length word");
    Check((unsigned char)cipher[28] == 0x5A, "nothing written past the last block");
    // first block: XTEA with the initial schedule (keycopy[3], [2], [1], [0])
    unsigned long kl = strlen(key); char kc[64]; memset(kc, 0, sizeof kc);
    for (unsigned long c = 0; c < kl; ++c) kc[c] = (char)(key[kl - c] * (c + 1));
    unsigned long k[4] = { (unsigned long)(long)kc[3], (unsigned long)(long)kc[2], (unsigned long)(long)kc[1], (unsigned long)(long)kc[0] };
    unsigned long in[2], out[2]; memcpy(in, text, 8); EncipherMorsel(in, out, k);
    Check(memcmp(out, cipher + 4, 8) == 0, "first block");
    char plain[64]; memset(plain, 0x33, sizeof plain);
    int r = CryptoMatic(cipher, plain, n, key, 1);
    Check(r == 21, "mode 1 returns the stored length");
    Check(memcmp(plain, text, 21) == 0 && plain[21] == 0 && plain[23] == 0 && (unsigned char)plain[24] == 0x33, "decrypted text, zero padded");
    char other[64]; memset(other, 0, sizeof other);
    CryptoMatic(cipher, other, n, "ANOTHER-KEY", 1);
    Check(memcmp(other, text, 21) != 0, "another key does not decrypt");
    int big = 1000; memcpy(cipher, &big, 4);
    Check(CryptoMatic(cipher, plain, n, key, 1) == 24, "clamped stored length");
    if (g_fail) { printf("0055e2c0_TEST FAIL\n"); return 1; }
    printf("0055e2c0_TEST PASS\n");
    return 0;
}