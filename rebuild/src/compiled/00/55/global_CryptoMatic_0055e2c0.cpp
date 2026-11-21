// _global::CryptoMatic(const char* data, char* enc, int dlen, const char* key, char mode) -- encrypts (mode 0: length word + XTEA blocks) or decrypts (mode 1) a buffer with a key schedule derived from the key text; returns the output size (mode 0) or the stored length
// Flags: /Gr
// Leaf: ?CryptoMatic@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E2C0, 549 bytes, __fastcall (ecx = data, edx = enc, stack dlen / key / mode), `ret 0Ch`.
//   Grade: RELOCATION_MATCH. NEW row (in rebuild/manifest/functions.tsv, not yet in the catalogue).
// Identity: Ego_r.pdb / FableWin.pdb ?CryptoMatic@?A0xf010c0d0@@YIHPBDPADH0D@Z (anonymous namespace). Ego_d.pdb locals data, enc, dlen,
//   key, mode, t, retsize, cryptuse, keylength, keycopy, oldenc, { c }, { mod }, { indata }.
// Callees: operator new[] 0x00BFEB22, operator delete[] 0x00BFEB1C, EncipherMorsel 0x0055E040, DecipherMorsel 0x0055E180 (same-TU
//   bodies below: declaration-only they make the row DIFFER(584v549)).
// TU: the scoreboard crypto TU (Ego_d.pdb line-table file id 2880; retail order EncipherMorsel 0x0055E040, DecipherMorsel 0x0055E180,
//   CryptoMatic 0x0055E2C0, Explode 0x00560920, GetDecryptedDocument 0x00560BB0). All five rows share this define block and /Gr: the
//   retail TU is a __fastcall-default one (the anonymous-namespace helpers and the STLport instances it calls -- search 0x00567120,
//   __uninitialized_copy 0x00568E70 -- are __fastcall without a spelling; Explode grades DIFFER(472v653) without /Gr).
// Text: the landed 0x00563480 file's same-TU text with Ego_d.pdb's names; the morsels' declarations split over two lines as Ego_d's
//   line table has them (lines 83 / 84, 95 / 96). See the batch README.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_SEARCH
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_EXPLODE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_LHHTTP2_DOCUMENT
#include "engine/CTCHeroOnlineScoreboard.h"
#include "stlport/fable_string_bodies.h"

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*) throw();

namespace {
// Fidelity: same-TU strong definition of 0x0055E040 (retail TU-mate)
void FABLE_FASTCALL EncipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
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
// Fidelity: same-TU strong definition of 0x0055E180 (retail TU-mate)
void FABLE_FASTCALL DecipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
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
int FABLE_FASTCALL CryptoMatic(const char* data,char* enc,int dlen,const char* key,char mode)
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
}