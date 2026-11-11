#include <stdio.h>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <math.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
namespace std{template<class T>const T& max(const T&a,const T&b){return a<b?b:a;}}
#endif
#ifndef _MSC_VER
#define FABLE_CDECL
#endif
static bool dependency_ok;
static int wide_live,wide_created,wide_destroyed,lookup_count,construct_one,construct_two,map_updates,ceil_calls;
static std::string number(long n){std::ostringstream o;o<<n;return o.str();}
class CCharString { public:std::string text;CCharString(const char*s=""):text(s){}bool operator<(const CCharString&o)const{return text<o.text;}};
class COpinionDeedEffectsDef { public:mutable long refs;long KnowledgeLifeInSeconds,CrimeFineAmount;};
static COpinionDeedEffectsDef* all_defs;
class CWideString { public:
 std::string text;bool alive;
 CWideString():alive(true){++wide_live;++wide_created;}
 CWideString(const std::string&s):text(s),alive(true){++wide_live;++wide_created;}
 CWideString(const CWideString&o):text(o.text),alive(true){dependency_ok=dependency_ok&&o.alive;++wide_live;++wide_created;}
 ~CWideString(){dependency_ok=dependency_ok&&alive;alive=false;--wide_live;++wide_destroyed;}
 const CWideString&operator=(const CWideString&o){dependency_ok=dependency_ok&&alive&&o.alive;text=o.text;return *this;}
 const CWideString&operator+=(const CWideString&o){dependency_ok=dependency_ok&&alive&&o.alive;for(int n=0;n<10;++n)dependency_ok=dependency_ok&&all_defs[n].refs==1;text+=o.text;return *this;}
};
CWideString operator+(const CWideString&a,const wchar_t*b){dependency_ok=dependency_ok&&a.alive&&b[0]=='\n'&&b[1]==0;return CWideString(a.text+"\n");}
namespace NGameText {
class CDataBank { public:const CWideString GetTextBySymbol(const CCharString&s)const{++lookup_count;return CWideString(s.text);}};
bool ConstructString(CWideString&t,long a){++construct_one;dependency_ok=dependency_ok&&t.alive;t.text+="|"+number(a);return true;}
bool ConstructString(CWideString&t,long a,long b){++construct_two;dependency_ok=dependency_ok&&t.alive;long sum=0;for(int n=0;n<10;++n)sum+=all_defs[n].refs;dependency_ok=dependency_ok&&sum==11;t.text+="|"+number(a)+"|"+number(b);return true;}
}
class CMainGameComponent{ public:NGameText::CDataBank bank;static CMainGameComponent*instance;static CMainGameComponent*Get(){return instance;}const NGameText::CDataBank&GetTextBank()const{return bank;}};CMainGameComponent*CMainGameComponent::instance;
namespace {const CCharString CrimeText[10]={"C0","C1","C2","C3","C4","C5","C6","C7","C8","C9"};CCharString CrimeTotalText("FINE"),CrimeTimeLeftText("TIME");}
template<class T>class CDefPointer { public:T*Object;CDefPointer(T*p):Object(p){++Object->refs;}CDefPointer(const CDefPointer&o):Object(o.Object){++Object->refs;}~CDefPointer(){--Object->refs;}T*operator->()const{return Object;}};
enum EOpinionDeedType {DEED_FIRST};
class CTCHeroOpinionDeedLog { public:COpinionDeedEffectsDef*defs;mutable long requests;CDefPointer<const COpinionDeedEffectsDef>PeekPDeedEffectsDef(EOpinionDeedType n)const{++requests;return CDefPointer<const COpinionDeedEffectsDef>(&defs[(int)n]);}};
class CThing { public:CTCHeroOpinionDeedLog*log;template<class T>bool GetTC(T**p){*p=log;return true;}};
class CWorld { public:long frame;long GetFrame()const{return frame;}};
class CTCHeroStats { public:
 struct MirroredCriminalStatus {long Crimes[10],LastTimeSeenByGuard;MirroredCriminalStatus():LastTimeSeenByGuard(0){for(int i=0;i<10;++i)Crimes[i]=0;}};
 std::map<CCharString,MirroredCriminalStatus>OutstandingCrimes;CThing*parent;CWorld*world;bool erase_on_summary;
 CThing&GetParentThing(){return *parent;}CWorld&GetWorld(){return *world;}
 void UpdateOutstandingCrimes(const CCharString&region){++map_updates;if(map_updates==2&&erase_on_summary)for(int n=0;n<10;++n)if(n%2==0)OutstandingCrimes[region].Crimes[n]=0;}
 void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
 void GetCrimeReport(const CCharString&,CWideString&,bool);
};
void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString& region,long& fine,long& frames_left)
{
 UpdateOutstandingCrimes(region);
 frames_left=0;fine=0;
 CTCHeroOpinionDeedLog* tc_opinion_deed_log=0;
 GetParentThing().GetTC(&tc_opinion_deed_log);
 for(long n=0;n<10;++n) {
  if(OutstandingCrimes[region].Crimes[n]!=0) {
   fine+=tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->CrimeFineAmount*OutstandingCrimes[region].Crimes[n];
   long expires_on=tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->KnowledgeLifeInSeconds*15+OutstandingCrimes[region].LastTimeSeenByGuard;
   frames_left=std::max(frames_left,expires_on-GetWorld().GetFrame());
  }
 }
}
void CTCHeroStats::GetCrimeReport(const CCharString& region,CWideString& report,bool include_summary)
{
 const NGameText::CDataBank& rTextBank=CMainGameComponent::Get()->GetTextBank();
 UpdateOutstandingCrimes(region);
 CTCHeroOpinionDeedLog* tc_opinion_deed_log=0;
 GetParentThing().GetTC(&tc_opinion_deed_log);
 CWideString crime_text;
 for(long n=0;n<10;++n) {
  if(OutstandingCrimes[region].Crimes[n]!=0) {
   crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeText[n]);
   NGameText::ConstructString(crime_text,OutstandingCrimes[region].Crimes[n],tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->CrimeFineAmount);
   report+=crime_text+L"\n";
  }
 }
 if(include_summary) {
  long fine,frames_left;
  GetOutstandingFineAndFramesLeft(region,fine,frames_left);
  crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeTotalText);
  NGameText::ConstructString(crime_text,fine);
  report+=crime_text+L"\n";
  if(fine) {
   crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeTimeLeftText);
   // Native0057ddd3 calls the original float overload; preserve overload identity.
   float (FABLE_CDECL* round_up)(float)=ceil;
   NGameText::ConstructString(crime_text,(long)round_up(frames_left*(1.0f/900.0f)));
   report+=crime_text+L"\n";
  }
 }
}

static int failed_line;
#define CHECK(x) do{if(!(x)){failed_line=__LINE__;return false;}}while(0)
static bool one(unsigned mask,bool summary,int timing,bool erase,int sign)
{
 dependency_ok=true;wide_live=wide_created=wide_destroyed=lookup_count=construct_one=construct_two=map_updates=ceil_calls=0;
 COpinionDeedEffectsDef defs[10];all_defs=defs;CTCHeroOpinionDeedLog log;log.defs=defs;log.requests=0;CThing thing;thing.log=&log;CMainGameComponent main;CMainGameComponent::instance=&main;CWorld world;const long frames[]={-1,0,1,899,900,901,1800};world.frame=1500-frames[timing];
 CTCHeroStats stats;stats.parent=&thing;stats.world=&world;stats.erase_on_summary=erase;CCharString region("BOWERSTONE"),other("OTHER");stats.OutstandingCrimes[other].Crimes[0]=777;long before[10],after[10];long fine=0,frames_left=0;int report_crimes=0,fine_crimes=0;
 if(mask)stats.OutstandingCrimes[region].LastTimeSeenByGuard=0;
 std::string expected="PREFIX\n";
 for(int n=0;n<10;++n){defs[n].refs=1;defs[n].KnowledgeLifeInSeconds=100;defs[n].CrimeFineAmount=n*2;before[n]=(mask&(1u<<n))?((n%3+1)*(sign?-1:1)):0;after[n]=erase&&n%2==0?0:before[n];if(mask)stats.OutstandingCrimes[region].Crimes[n]=before[n];if(before[n]){++report_crimes;expected+="C"+number(n)+"|"+number(before[n])+"|"+number(defs[n].CrimeFineAmount)+"\n";}if(after[n]){++fine_crimes;fine+=after[n]*defs[n].CrimeFineAmount;if(frames[timing]>frames_left)frames_left=frames[timing];}}
 if(summary){expected+="FINE|"+number(fine)+"\n";if(fine){long minutes=(frames_left+899)/900;expected+="TIME|"+number(minutes)+"\n";}}
 {CWideString report(std::string("PREFIX\n"));stats.GetCrimeReport(region,report,summary);CHECK(report.text==expected);CHECK(wide_live==1);}
 CHECK(dependency_ok&&wide_live==0&&wide_created==wide_destroyed);CHECK(map_updates==(summary?2:1));CHECK(lookup_count==report_crimes+(summary?1+(fine!=0):0));CHECK(construct_two==report_crimes);CHECK(construct_one==(summary?1+(fine!=0):0));CHECK(log.requests==report_crimes+(summary?fine_crimes*2:0));CHECK(stats.OutstandingCrimes[other].Crimes[0]==777);
 for(int n=0;n<10;++n){CHECK(defs[n].refs==1);CHECK(stats.OutstandingCrimes[region].Crimes[n]==(summary?after[n]:before[n]));}
 return true;
}
int main(){unsigned long cases=0;for(unsigned mask=0;mask<1024;++mask)for(int summary=0;summary<2;++summary)for(int timing=0;timing<7;++timing)for(int erase=0;erase<2;++erase)for(int sign=0;sign<2;++sign){if(!one(mask,summary!=0,timing,erase!=0,sign)){printf("0057dc22_TEST FAIL line%d case%lu\n",failed_line,cases);return 1;}++cases;}if(cases!=57344)return 2;puts("0057dc22_TEST PASS");return 0;}