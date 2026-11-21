// CTCHeroOnlineScoreboard::GetDecryptedDocument(std::vector<std::string>& r_decrypted_lines, const CCharString& key) -- reads the HTTP reply document, decrypts it with the key and splits it into CRLF-separated lines; -1 when there is no document
// Flags: /Gr
// Leaf: ?GetDecryptedDocument@CTCHeroOnlineScoreboard@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00560BB0, 321 bytes, __fastcall `ret 8`. Grade: RELOCATION_MATCH. NEW row.
// Identity: Ego_r.pdb / FableWin.pdb ?GetDecryptedDocument@CTCHeroOnlineScoreboard@@AAEHAAV?$vector@..@std@@ABVCCharString@@@Z (private).
//   Ego_d.pdb locals this, r_decrypted_lines, key, docsize, { encrypted_doc, decrypted_doc, doc_str, decrypted_size, count }.
// Callees: LHHttp2::GetDocumentSize 0x0083EA70 / GetDocument 0x0083EAA0, vector<char>(n, val) 0x00567560, vector<char>::resize 0x004122D0,
//   CryptoMatic 0x0055E2C0, basic_string::_M_range_initialize<char*> 0x00567D00, Explode 0x00560920, malloc / memmove / free.
// Same-TU bodies: Explode is load-bearing (declaration-only: DIFFER(336v321); retail keeps both buffer starts and the separator start in
//   registers across the calls, which VC7.1 does only when it sees the callee keep no local's address). EncipherMorsel, DecipherMorsel and
//   CryptoMatic are byte-neutral here; their bodies are the retail TU-mates, present so the behaviour test links the real object (an
//   anonymous-namespace declaration cannot be stood in from another TU: its ?A0x hash follows the compile path).
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
// Fidelity: same-TU strong definition of 0x0055E2C0 (retail TU-mate)
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
// Fidelity: same-TU strong definition of 0x00560920 (retail TU-mate)
int FABLE_FASTCALL Explode(std::string& input,const std::string& token,std::vector<std::string>& ret)
{
    unsigned int startpos=0;
    unsigned int findpos=0;
    do {
        findpos=input.find(token,startpos);
        if(findpos!=std::string::npos) {
            std::string newstr=input.substr(startpos,findpos-startpos);
            ret.push_back(newstr);
            startpos=findpos+token.length();
        }
    } while(findpos!=std::string::npos);
    return ret.size();
}
}

int CTCHeroOnlineScoreboard::GetDecryptedDocument(std::vector<std::string>& r_decrypted_lines,const CCharString& key)
{
    unsigned long docsize=PHttp->GetDocumentSize();
    if(docsize!=0) {
        std::vector<char> encrypted_doc(docsize+1,0);
        PHttp->GetDocument(&encrypted_doc[0]);
        std::vector<char> decrypted_doc;
        decrypted_doc.resize(docsize+1);
        int decrypted_size=CryptoMatic(&encrypted_doc[0],&decrypted_doc[0],docsize,key.GetAsCharArray(),1);
        std::string doc_str(decrypted_doc.begin(),decrypted_doc.end());
        int count=Explode(doc_str,std::string("\r\n"),r_decrypted_lines);
        return count;
    }
    return -1;
}
