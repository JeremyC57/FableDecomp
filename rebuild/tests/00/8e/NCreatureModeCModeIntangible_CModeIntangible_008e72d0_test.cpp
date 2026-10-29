#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#include <string.h>
#include <new>
extern "C" int puts(const char*);
int main();
// 008e72d0 NCreatureMode::CModeIntangible::CModeIntangible -- initialize a timed intangible mode
// Leaf: ??0CModeIntangible@NCreatureMode@@QAE@AAVCThing@@MPAVCCreatureActionBase@@@Z
// Flags:
#define CCharString CCharStringLayout
#define CCharStringData CCharStringDataLayout
#include "fable_string.h"
#undef CCharStringData
#undef CCharString
#define CModeDragonBase CModeDragonBaseLayout
#include "engine/CModeDragonBase.h"
#undef CModeDragonBase
class CThing;class CCreatureActionBase;class CCPPointerInfo;
#pragma pack(push,1)
template<class T>class CBasicString {
friend int ::main();protected:T*PData;unsigned long StringLength;
    // Layout: original allocation-length and extension flag bytes are unread.
    unsigned char unobservedStringFlags[5];
public:unsigned long GetLength()const{return StringLength;}const T*PeekData()const{return PData;}
};
class CCharStringData {friend class CCharString;
friend int ::main();public:CBasicString<char>Data;protected:long NoRefs;};
#pragma pack(pop)
extern const char CUnprovenGlobal_0129aaf4[1];
class CCharString {protected:CCharStringData*PStringData;public:
    CCharString();CCharString(const char*,long=-1);~CCharString();
    const CCharString&operator=(const CCharString&);
    long GetLength()const{return PStringData==0?0:(long)PStringData->Data.GetLength();}
    const char*GetAsCharArray()const{return PStringData==0?CUnprovenGlobal_0129aaf4:PStringData->Data.PeekData();}
};
class CCRC {public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
class CAnimFilterBase {public:virtual ~CAnimFilterBase();};
class CAnimFilterOfGroup:public CAnimFilterBase {
friend int ::main();
private:CCharString Group;unsigned long GroupCRC;
public:
    CAnimFilterOfGroup(const CCharString&group):Group() {
        Group=group;GroupCRC=CCRC::Calc(0,group.GetAsCharArray(),group.GetLength());
    }
};
#pragma pack(push,1)
class CBaseIntelligentPointer {
public:
    // Layout: the original PData pointer is never read by this constructor.
    private:unsigned char unobservedPointerState[4];public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:CIntelligentPointer() {}};
#pragma pack(pop)
template<class T>class CCountedPointer {
public:T*Data;CCPPointerInfo*Info;
    CCountedPointer():Data(0),Info(0){}~CCountedPointer();void Reset(T*);
};
class CTCCreatureModeManager {
public:class CModeBase {
public:virtual ~CModeBase();
    // Layout: ParentThing, WeaponBasedFilter, removal countdown and active state are unread.
    unsigned char unobservedModeState[24];CModeBase(CThing&);
};};
// Layout: static-only declaration; no CMainGameComponent instance or full extent claimed.
class CMainGameComponent{
friend int ::main();protected:static long ConstantFPS;public:static __forceinline long __fastcall GetConstantFPS(){return ConstantFPS;}};
namespace NCreatureMode {
class CModeDragonBase:public CTCCreatureModeManager::CModeBase {
friend int ::main();
protected:CAnimFilterOfGroup Filter;CIntelligentPointer<CThing>PTarget;
public:
    CModeDragonBase(CThing&thing,const CCharString&group):CModeBase(thing),Filter(group){}
};
class CModeIntangible:public CModeDragonBase {
friend int ::main();
private:long Timer,BossPhase;bool WasDamageable;CCountedPointer<CCreatureActionBase>PActionOnDeactivate;
public:
    CModeIntangible(CThing&,float,CCreatureActionBase*);
};
CModeIntangible::CModeIntangible(CThing&thing,float seconds,CCreatureActionBase*action):CModeDragonBase(thing,"") {
    Timer=(long)(seconds*CMainGameComponent::GetConstantFPS());PActionOnDeactivate.Reset(action);
}
}
FABLE_STATIC_ASSERT(sizeof(CCharStringData)==sizeof(CCharStringDataLayout));
FABLE_STATIC_ASSERT(sizeof(CCharString)==sizeof(CCharStringLayout));
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeDragonBase)==sizeof(CModeDragonBaseLayout));
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeIntangible)==68);

const char CUnprovenGlobal_0129aaf4[1]={0};
long CMainGameComponent::ConstantFPS=0;
static CCharStringData fixtureData;static char fixtureEmpty[1]={0};static bool fixtureNullString=false;
static unsigned int fixtureText=0,fixtureDefault=0,fixtureAssign=0,fixtureStringDtor=0,fixtureBaseCtor=0,fixtureBaseDtor=0,fixtureIntellCtor=0,fixtureIntellDtor=0,fixtureCRC=0,fixtureReset=0,fixtureCountDtor=0,fixtureErrors=0;
static const CThing*fixtureThing=0;static CCreatureActionBase*fixtureAction=0;
CCharString::CCharString():PStringData(0){++fixtureDefault;}
CCharString::CCharString(const char*p,long n):PStringData(fixtureNullString?0:&fixtureData){++fixtureText;if(!p||*p||n!=-1)++fixtureErrors;if(PStringData)PStringData->NoRefs=1;}
CCharString::~CCharString(){++fixtureStringDtor;if(PStringData)--PStringData->NoRefs;}
const CCharString&CCharString::operator=(const CCharString&s){++fixtureAssign;PStringData=s.PStringData;if(PStringData)++PStringData->NoRefs;return *this;}
unsigned long __fastcall CCRC::Calc(unsigned long seed,const void*p,unsigned long length){++fixtureCRC;if(seed||length||!p||*(const char*)p)++fixtureErrors;return 0;}
CAnimFilterBase::~CAnimFilterBase(){}
CBaseIntelligentPointer::CBaseIntelligentPointer(){++fixtureIntellCtor;memset(unobservedPointerState,0,sizeof(unobservedPointerState));}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++fixtureIntellDtor;}
CTCCreatureModeManager::CModeBase::CModeBase(CThing&thing){++fixtureBaseCtor;if(&thing!=fixtureThing)++fixtureErrors;memset(unobservedModeState,0xaa,sizeof(unobservedModeState));}
CTCCreatureModeManager::CModeBase::~CModeBase(){++fixtureBaseDtor;}
template<class T>void CCountedPointer<T>::Reset(T*p){++fixtureReset;if(p!=fixtureAction)++fixtureErrors;Data=p;Info=reinterpret_cast<CCPPointerInfo*>(p?0x1234U:0U);}
template<class T>CCountedPointer<T>::~CCountedPointer(){++fixtureCountDtor;}
struct FixtureModeStorage {unsigned int before;unsigned char bytes[68];unsigned int after;};
int main(){
 unsigned int cases=0;const long rates[7]={-1,0,1,25,30,60,120};
 fixtureData.Data.PData=fixtureEmpty;fixtureData.Data.StringLength=0;
 for(int rate=0;rate<7;++rate)for(int t=-32;t<=32;++t)for(int empty=0;empty<2;++empty)for(int action=0;action<4;++action){
  FixtureModeStorage storage;storage.before=0x12345678U;storage.after=0xfedcba98U;memset(storage.bytes,0xa5,sizeof(storage.bytes));
  unsigned char originalThing[8];memset(originalThing,0x7b,sizeof(originalThing));fixtureThing=reinterpret_cast<const CThing*>(originalThing);fixtureAction=reinterpret_cast<CCreatureActionBase*>(action?0x1000U+32U*action:0U);CMainGameComponent::ConstantFPS=rates[rate];fixtureNullString=empty!=0;fixtureData.NoRefs=0;
  fixtureText=fixtureDefault=fixtureAssign=fixtureStringDtor=fixtureBaseCtor=fixtureBaseDtor=fixtureIntellCtor=fixtureIntellDtor=fixtureCRC=fixtureReset=fixtureCountDtor=fixtureErrors=0;
  float seconds=(float)t/8.0f;NCreatureMode::CModeIntangible*mode=new(storage.bytes)NCreatureMode::CModeIntangible(*const_cast<CThing*>(fixtureThing),seconds,fixtureAction);
  if(fixtureErrors||fixtureText!=1||fixtureDefault!=1||fixtureAssign!=1||fixtureStringDtor!=1||fixtureBaseCtor!=1||fixtureIntellCtor!=1||fixtureCRC!=1||fixtureReset!=1)return 1;
  if(mode->Timer!=(long)((double)seconds*rates[rate])||mode->PActionOnDeactivate.Data!=fixtureAction||mode->PActionOnDeactivate.Info!=reinterpret_cast<CCPPointerInfo*>(action?0x1234U:0U)||mode->Filter.GroupCRC!=0)return 2;
  unsigned char fill[4];memset(fill,0xa5,sizeof(fill));if(memcmp(&mode->BossPhase,fill,4)||memcmp(&mode->WasDamageable,fill,1))return 3;
  if(!fixtureNullString&&fixtureData.NoRefs!=1)return 4;
  mode->~CModeIntangible();if(fixtureStringDtor!=2||fixtureBaseDtor!=1||fixtureIntellDtor!=1||fixtureCountDtor!=1||(!fixtureNullString&&fixtureData.NoRefs))return 5;
  if(storage.before!=0x12345678U||storage.after!=0xfedcba98U||CMainGameComponent::ConstantFPS!=rates[rate])return 6;
  for(unsigned int k=0;k<sizeof(originalThing);++k)if(originalThing[k]!=0x7b)return 7;
  ++cases;
 }
 if(cases!=3640)return 8;puts("008e72d0_TEST PASS");return 0;
}