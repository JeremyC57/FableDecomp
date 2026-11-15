// CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan -- download per-activity clan records.
// Grade: MASKED_MATCH_PENDING_AUDIT(1178/1178); all364 instructions match.
// Original ENetworkSuccess values: success1000, failure1001, pending1002. Original private leaf, class/member names and enum fields.
// Full byte/ABI/helper-debt evidence and independent behavior fixture are retained alongside this source.
// Flags: /Gr
// Leaf: ?DownloadScoreboardForSelectedClan@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_STLP_STRING_SCOREBOARD
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_SEARCH
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_EXPLODE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_PASS22_SCOREBOARD_COPY_BACKWARD_ORIGINAL
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_ORIGINAL_ALIASES_PASS22
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_LHHTTP2_DOCUMENT
#include "engine/CTCHeroOnlineScoreboard.h"
#include "stlport/fable_string_bodies.h"

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*) throw();

long FABLE_FASTCALL GFCharStringToInt(const CCharString&);
float FABLE_FASTCALL GFCharStringToFloat(const CCharString&);
// Inline-budget: native00563480 calls only the score-pair append. Keep this
// actual STLport body visible without suppressing the string appends in Explode.
#pragma auto_inline(off)
namespace std {
template<> void vector<pair<float,CWideString> >::push_back(const pair<float,CWideString>& x)
{
    if(this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish,x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish,x,_IsPODType(),1UL,true);
}
}
#pragma auto_inline(on)

namespace std {
// STLport4.6.2 _algo.h original declaration; native score-pair instance005671C0.
template<class I,class J> inline void iter_swap(I a,J b) { swap(*a,*b); }
template<class R> inline void FABLE_FASTCALL __reverse(R first,R last,const random_access_iterator_tag&) {
    for(;first<last;++first) iter_swap(first,--last);
}
template<class R> inline void reverse(R first,R last) { __reverse(first,last,random_access_iterator_tag()); }
}

namespace {
// Complete original anonymous helpers, recovered from both typed PDB records and donor/native instructions.
void FABLE_FASTCALL EncipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1],sum=0,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v0+=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
        sum+=delta;
        v1+=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
    }
    w[0]=v0;w[1]=v1;
}
void FABLE_FASTCALL DecipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1],sum=0xc6ef3720,delta=0x9e3779b9;
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

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan()
{
    ENetworkSuccess result;
    if (TaskState == 100 || TaskState == 101) {
        CCharString req = GetGameIdent() + CCharString("DOWNLOAD_SCOREBOARD\r\n") + GetSignedInUserName() + "\r\n" + SessionID + "\r\n" + SelectedClan.ToCharString() + "\r\n";
        if(ResetAndOpenHTTPAndPrepareRequest(req.GetAsCharArray(),GetCDKey(),false)==ENS_FAILURE)
            return ENS_FAILURE;
    }
    result = UpdateTransaction();
    if (result == ENS_SUCCESS) {
        std::vector<std::string> lines;
        int count = GetDecryptedDocument(lines, GetCDKey());
        if (count > 0) {
            if (lines[0] != "SID:OK")
                return ENS_FAILURE;
            TOnlineTopScoresInSingleEvent event_results[OSA_NUM_ACTIVITIES];
            for (int i = 1; i < count; ) {
                EOnlineScoreboardActivity activity_enum = static_cast<EOnlineScoreboardActivity>(GFCharStringToInt(lines[i++].c_str()));
                TActivityScore score = GFCharStringToFloat(lines[i++].c_str());
                std::string scoring_player = lines[i++];
                std::string empty_line = lines[i++];
                TOnlineSingleScore single_score;
                single_score.first = score;
                single_score.second = CCharString(scoring_player.c_str()).ToWideString();
                event_results[activity_enum].push_back(single_score);
            }
            for (int act = 0; act < OSA_NUM_ACTIVITIES; ++act) {
                if (!event_results[act].empty() && ActivityInfo[act].LowerIsBetter)
                    std::reverse(event_results[act].begin(),event_results[act].end());
                TOnlineTopScoresInSingleEvent& top_scores_in_this_activity=event_results[act];
                EOnlineScoreboardActivity act_enum=static_cast<EOnlineScoreboardActivity>(act);
                DownloadedClanScoreboard[act_enum] = top_scores_in_this_activity;
            }
        }
        PHttp.reset();
    }
    return result;
}