#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_STLP_STRING_SCOREBOARD
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#include "stlport/fable_string.h"
#include "stlport/fable_string_bodies.h"
#include "boost/scoped_ptr.h"
struct CWideString;
struct CCharString {std::string Text;CCharString(){}CCharString(const char*s,long=-1):Text(s){}const char*GetAsCharArray()const{return Text.c_str();}CWideString ToWideString()const;};
struct CWideString {std::string Text;CWideString(){}CWideString(const char*s):Text(s){}CCharString ToCharString()const{return CCharString(Text.c_str());}};
CWideString CCharString::ToWideString()const{return CWideString(Text.c_str());}
CCharString FABLE_FASTCALL operator+(const CCharString&a,const CCharString&b){std::string s=a.Text;s+=b.Text;return CCharString(s.c_str());}
CCharString FABLE_FASTCALL operator+(const CCharString&a,const char*b){std::string s=a.Text;s+=b;return CCharString(s.c_str());}
long FABLE_FASTCALL GFCharStringToInt(const CCharString&s){char*end=0;long n=strtol(s.GetAsCharArray(),&end,10);assert(*end==0);return n;}
float FABLE_FASTCALL GFCharStringToFloat(const CCharString&s){char*end=0;double n=strtod(s.GetAsCharArray(),&end);assert(*end==0);return static_cast<float>(n);}
static long PrepareResult,TransactionResult,PrepareCalls,UpdateCalls,DocumentSizeCalls,DocumentCalls,CryptoCalls,HttpLive,HttpDeleted;
static std::string Reply,ExpectedRequest;
namespace LHNetworkLib {
enum LH_HTTP_STATUS {FixtureHttpOK=0};
class LHHttp2 {public:LHHttp2(){++HttpLive;}~LHHttp2(){--HttpLive;++HttpDeleted;}unsigned long GetDocumentSize(){++DocumentSizeCalls;return static_cast<unsigned long>(Reply.size());}LH_HTTP_STATUS GetDocument(char*p){++DocumentCalls;memcpy(p,Reply.data(),Reply.size());return FixtureHttpOK;}};
}
// External map-index contract. Production map::operator[] is declared out of line;
// all valid protocol keys are 0..38. Actual shared vectors and strings remain unchanged.
namespace std {template<class K,class V>class map {public:V Values[39];long Calls;map():Calls(0){}V&operator[](K k){assert(k>=0&&k<39);++Calls;return Values[k];}};}
class CTCHeroOnlineScoreboard {
public:
 enum EOnlineScoreboardActivity {OSA_NUM_ACTIVITIES=39};
 enum ENetworkSuccess {ENS_SUCCESS=1000,ENS_FAILURE=1001,ENS_PENDING=1002};
 struct CActivity {bool LowerIsBetter;};
 boost::scoped_ptr<LHNetworkLib::LHHttp2>PHttp;std::vector<CActivity>ActivityInfo;CCharString SessionID;CWideString SelectedClan;std::map<EOnlineScoreboardActivity,std::vector<std::pair<float,CWideString> > >DownloadedClanScoreboard;long TaskState;
 CCharString GetGameIdent(){return CCharString("Fable1.5\r\nVersion 1\r\n");}CCharString GetCDKey(){return CCharString("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM");}CCharString GetSignedInUserName(){return CCharString("fixture_user");}
 ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const std::string&s,const CCharString&key,bool b){++PrepareCalls;assert(!b&&strcmp(key.GetAsCharArray(),"THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM")==0&&strcmp(s.c_str(),ExpectedRequest.c_str())==0);return static_cast<ENetworkSuccess>(PrepareResult);}
 ENetworkSuccess UpdateTransaction(){++UpdateCalls;return static_cast<ENetworkSuccess>(TransactionResult);}
 int GetDecryptedDocument(std::vector<std::string>&,const CCharString&);
 ENetworkSuccess DownloadScoreboardForSelectedClan();
};

namespace std {
// STLport4.6.2 _algo.h original declaration; native score-pair instance005671C0.
template<class I,class J> inline void iter_swap(I a,J b) { swap(*a,*b); }
template<class R> inline void FABLE_FASTCALL __reverse(R first,R last,const random_access_iterator_tag&) {
    for(;first<last;++first) iter_swap(first,--last);
}
template<class R> inline void reverse(R first,R last) { __reverse(first,last,random_access_iterator_tag()); }
}

namespace {
int FABLE_FASTCALL CryptoMatic(const char* in,char* out,int length,const char* key,char mode);
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
int FABLE_FASTCALL Explode(std::string& text,const std::string& separator,std::vector<std::string>& lines)
{
    std::string::size_type pos=0,found;
    while((found=text.find(separator.c_str(),pos,separator.size()))!=std::string::npos) {
        std::string piece=text.substr(pos,found-pos);
        lines.push_back(piece);
        pos=found+separator.size();
    }
    return lines.size();
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
}

#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
int CTCHeroOnlineScoreboard::GetDecryptedDocument(std::vector<std::string>& lines,const CCharString& key)
{
    unsigned long size=PHttp->GetDocumentSize();
    if(size!=0) {
        std::vector<char> document(size+1,0);
        PHttp->GetDocument(&document[0]);
        std::vector<char> decrypted;
        decrypted.resize(size+1);
        CryptoMatic(&document[0],&decrypted[0],size,key.GetAsCharArray(),1);
        std::string text(decrypted.begin(),decrypted.end());
        return Explode(text,std::string("\r\n"),lines);
    }
    return -1;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

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
            std::vector<std::pair<float, CWideString> > event_results[OSA_NUM_ACTIVITIES];
            for (int i = 1; i < count; ) {
                EOnlineScoreboardActivity activity_enum = static_cast<EOnlineScoreboardActivity>(GFCharStringToInt(lines[i++].c_str()));
                float score = GFCharStringToFloat(lines[i++].c_str());
                std::string scoring_player = lines[i++];
                std::string empty_line = lines[i++];
                std::pair<float, CWideString> single_score;
                single_score.first = score;
                single_score.second = CCharString(scoring_player.c_str()).ToWideString();
                event_results[activity_enum].push_back(single_score);
            }
            for (int act = 0; act < OSA_NUM_ACTIVITIES; ++act) {
                std::vector<std::pair<float, CWideString> >& top_scores_in_this_activity=event_results[act];
                if (!top_scores_in_this_activity.empty() && ActivityInfo[act].LowerIsBetter)
                    std::reverse(top_scores_in_this_activity.begin(),top_scores_in_this_activity.end());
                EOnlineScoreboardActivity act_enum=static_cast<EOnlineScoreboardActivity>(act);
                DownloadedClanScoreboard[act_enum] = top_scores_in_this_activity;
            }
        }
        PHttp.reset();
    }
    return result;
}

namespace std {
// External search contract used by the actual shared string::find body.
template<class I,class J,class P>I FABLE_FASTCALL search(I f,I l,J a,J b,P pred){
    if(a==b)return f;
    for(;f!=l;++f){I i=f;J j=a;while(i!=l&&j!=b&&pred(*i,*j)){++i;++j;}if(j==b)return f;}
    return l;
}
}
namespace {
int FABLE_FASTCALL CryptoMatic(const char*in,char*out,int length,const char*key,char mode){assert(mode==1&&strcmp(key,"THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM")==0);++CryptoCalls;memcpy(out,in,length);return length;}
}
static void AddLine(const char*s){Reply+=s;Reply+="\r\n";}
int main(){
 for(long c=0;c<4096;++c){
  PrepareResult=1000+(c%3);TransactionResult=1000+((c/3)%3);PrepareCalls=UpdateCalls=DocumentSizeCalls=DocumentCalls=CryptoCalls=HttpDeleted=0;Reply="";
  int scenario=(int)((c/9)%4),records=(int)((c/36)%24);std::vector<std::pair<float,CWideString> > expected[39];
  if(scenario!=0){AddLine(scenario==1?"SID:FAIL":"SID:OK");for(int k=0;k<records;++k){char line[80];long a=(c+(k%7)*5)%39;float value=static_cast<float>(k*4-(c%41));sprintf(line,"%ld",a);AddLine(line);sprintf(line,"%ld",(long)value);AddLine(line);sprintf(line,"player_%ld_%d",c,k);AddLine(line);std::pair<float,CWideString>p;p.first=value;p.second=CWideString(line);expected[a].push_back(p);AddLine(k%2?"ignored_extra":"");}}
  CTCHeroOnlineScoreboard m;m.TaskState=(c%4==0?100:c%4==1?101:102);m.PHttp.reset(new LHNetworkLib::LHHttp2);m.SessionID=CCharString("fixture_session");m.SelectedClan=CWideString("fixture_clan");
  for(int a=0;a<39;++a){CTCHeroOnlineScoreboard::CActivity info;info.LowerIsBetter=((a+c)&1)!=0;m.ActivityInfo.push_back(info);}
  ExpectedRequest="Fable1.5\r\nVersion 1\r\nDOWNLOAD_SCOREBOARD\r\nfixture_user\r\nfixture_session\r\nfixture_clan\r\n";
  bool preparing=m.TaskState==100||m.TaskState==101,early=preparing&&PrepareResult==1001,success=!early&&TransactionResult==1000,bad=success&&scenario==1;
  long result=m.DownloadScoreboardForSelectedClan();assert(result==(early||bad?1001:TransactionResult));assert(PrepareCalls==(preparing?1:0)&&UpdateCalls==(early?0:1));assert(DocumentSizeCalls==(success?1:0));assert(DocumentCalls==(success&&scenario!=0?1:0)&&CryptoCalls==DocumentCalls);
  bool copied=success&&scenario>=2;assert(m.DownloadedClanScoreboard.Calls==(copied?39:0));
  for(int a=0;a<39;++a){std::vector<std::pair<float,CWideString> >&v=m.DownloadedClanScoreboard.Values[a];assert(v.size()==(copied?expected[a].size():0));for(unsigned long j=0;j<v.size();++j){unsigned long ix=m.ActivityInfo[a].LowerIsBetter?static_cast<unsigned long>(expected[a].size()-1-j):j;assert(v[j].first==expected[a][ix].first&&strcmp(v[j].second.Text.c_str(),expected[a][ix].second.Text.c_str())==0);}}
  bool closed=success&&!bad;assert((m.PHttp.get()==0)==closed&&HttpDeleted==(closed?1:0));m.PHttp.reset();assert(HttpDeleted==1&&HttpLive==0);
 }
 puts("00563480_TEST PASS");return 0;
}