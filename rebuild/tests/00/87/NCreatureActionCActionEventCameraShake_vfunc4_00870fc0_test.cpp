#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
// BEGIN exact source snapshot SHA256 602bb7df736f9c6de90cad30eff8ac78b685b4465239ab1632b23230223ba711
#include <string.h>
#include <stddef.h>
#define CCharString CCharStringPdb
#define CCharStringData CCharStringDataPdb
#include "fable_string.h"
#undef CCharStringData
#undef CCharString
#pragma pack(push,1)
// NCreatureAction::CActionEventCameraShake::SetDataFromString, 0x00870FC0.
// PDB identity: egor 19895 / debug 0x00811990. The three parsed values are
// Strength(+0x14), Length(+0x18), and Global(+0x1c), as recovered from PDB.
template<class T> struct CBasicString;
template<> struct CBasicString<char> {
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend;
    bool operator==(const char*) const;
};
// The 004115A0 equality body compares signed chars without modifying storage.
#pragma auto_inline(off)
bool CBasicString<char>::operator==(const char* text) const
{
    const signed char* first = (const signed char*)PData;
    const signed char* second = (const signed char*)text;
    int result = 0;
    while (*first || *second) {
        if (*first < *second) { result = -1; break; }
        if (*first > *second) { result = 1; break; }
        ++first; ++second;
    }
    return result == 0;
}
#pragma auto_inline(on)
struct CCharStringData { CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
typedef char StringDataMatchesDonor[(sizeof(CCharStringData)==sizeof(CCharStringDataPdb))?1:-1];
struct CCharString {
    CCharStringData* PStringData;
    CCharString();
    ~CCharString();
    __forceinline bool operator==(const char* text) const {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0) return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
namespace { bool __fastcall GetDataFromEventString(const CCharString& data, long index,
                                       CCharString& out); } // 0x00870C90

struct CCreatureActionBase;
struct CThingCreatureBase;
struct CGameCameraManager;
struct CBaseClass { virtual ~CBaseClass(); };
// Original enum types; ordinal labels below are local descriptive labels.
enum EQuakeStrength { QuakeStrengthWeak=0,QuakeStrengthMedium=1,QuakeStrengthStrong=2,QuakeStrengthMadness=3 };
enum EQuakeLength { QuakeLengthShort=0,QuakeLengthMedium=1,QuakeLengthLong=2,QuakeLengthContinuous=3 };
namespace NCreatureAction {
struct CActionEventBase : CBaseClass {
    bool Finished;
    long ActivationFrame;
    CThingCreatureBase* PThing;
    CGameCameraManager* PCameraManager;
    virtual void Dispatch(CCreatureActionBase&)=0;
    virtual CCharString GetHook() const=0;
    virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&) const=0;
    virtual bool SetDataFromString(const CCharString&)=0;
};
struct CActionEventCameraShake : CActionEventBase {
    EQuakeStrength Strength;
    EQuakeLength Length;
    bool Global;
    virtual void Dispatch(CCreatureActionBase&);
    virtual CCharString GetHook() const;
    virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&) const;
    virtual bool SetDataFromString(const CCharString&);
};

bool CActionEventCameraShake::SetDataFromString(const CCharString& data)
{
    {
        CCharString value;
        if (!GetDataFromEventString(data, 0, value)) return false;
        if (value == "STRENGTH_WEAK") Strength = QuakeStrengthWeak;
        else if (value == "STRENGTH_MEDIUM") Strength = QuakeStrengthMedium;
        else if (value == "STRENGTH_STRONG") Strength = QuakeStrengthStrong;
        else if (value == "STRENGTH_MADNESS") Strength = QuakeStrengthMadness;
        else return false;
    }
    {
        CCharString length;
        if (!GetDataFromEventString(data, 1, length)) return false;
        if (length == "LENGTH_SHORT") Length = QuakeLengthShort;
        else if (length == "LENGTH_MEDIUM") Length = QuakeLengthMedium;
        else if (length == "LENGTH_LONG") Length = QuakeLengthLong;
        else if (length == "LENGTH_CONTINUOUS") Length = QuakeLengthContinuous;
        else return false;
    }
    {
        CCharString global;
        if (!GetDataFromEventString(data, 2, global)) return false;
        if (global == "GLOBAL") Global = true;
    }
    return true;
}
}

// END exact source snapshot
static int allocations,liveStrings;
static void Release(CCharString& s){
 if(s.PStringData && --s.PStringData->NoRefs==0){free(s.PStringData->Data.PData);free(s.PStringData);--allocations;}
 s.PStringData=0;
}
static void Assign(CCharString& out,const CCharString& input){
 CCharStringData* p=input.PStringData;if(p)++p->NoRefs;Release(out);out.PStringData=p;
}
static void SetRange(CCharString& out,const char* text,unsigned long n){
 CCharStringData* p=0;
 if(n){p=(CCharStringData*)malloc(sizeof(CCharStringData));p->Data.PData=(char*)malloc(n+1);memcpy(p->Data.PData,text,n);p->Data.PData[n]=0;p->Data.StringLength=n;p->Data.DataLength=n;p->Data.UseFastExtend=false;p->NoRefs=1;++allocations;}
 Release(out);out.PStringData=p;
}
static const char* Text(const CCharString& s){return s.PStringData?s.PStringData->Data.PData:"";}
CCharString::CCharString():PStringData(0){++liveStrings;}
CCharString::~CCharString(){Release(*this);--liveStrings;}
CBaseClass::~CBaseClass(){}
void NCreatureAction::CActionEventCameraShake::Dispatch(CCreatureActionBase&){}
CCharString NCreatureAction::CActionEventCameraShake::GetHook()const{return CCharString();}
NCreatureAction::CActionEventBase* NCreatureAction::CActionEventCameraShake::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{return 0;}
namespace {
bool __fastcall GetDataFromEventString(const CCharString& input,long index,CCharString& out){
 Assign(out,input);
 for(long token=0;token<=index;++token){
  const char* start=Text(out);const char* space=strchr(start,' ');
  if(!space)return false;
  SetRange(out,space+1,(unsigned long)strlen(space+1));
 }
 const char* start=Text(out);const char* space=strchr(start,' ');
 SetRange(out,start,(unsigned long)(space?space-start:strlen(start)));
 return true;
}
}
#define CHECK(x) do{if(!(x))return printf("00870fc0_TEST FAIL line %d\n",__LINE__),1;}while(0)
int main(){
 CHECK(sizeof(CBasicString<char>)==13 && sizeof(CCharStringData)==17 && offsetof(CCharStringData,NoRefs)==13);
 CHECK(sizeof(NCreatureAction::CActionEventBase)==20 && offsetof(NCreatureAction::CActionEventCameraShake,Strength)==0x14 && offsetof(NCreatureAction::CActionEventCameraShake,Length)==0x18 && offsetof(NCreatureAction::CActionEventCameraShake,Global)==0x1c);
 {
  NCreatureAction::CActionEventCameraShake action;CCharString input;
  action.Strength=QuakeStrengthWeak;action.Length=QuakeLengthShort;action.Global=false;
  const char* strengths[]={"STRENGTH_WEAK","STRENGTH_MEDIUM","STRENGTH_STRONG","STRENGTH_MADNESS"};
  const char* lengths[]={"LENGTH_SHORT","LENGTH_MEDIUM","LENGTH_LONG","LENGTH_CONTINUOUS"};
  for(int i=0;i<4;++i)for(int j=0;j<4;++j){char buf[128];sprintf(buf,"evt %s %s GLOBAL",strengths[i],lengths[j]);SetRange(input,buf,(unsigned long)strlen(buf));CHECK(action.SetDataFromString(input));CHECK(action.Strength==i && action.Length==j && action.Global && allocations==1 && input.PStringData->NoRefs==1 && liveStrings==1);}
  const char* cases[]={"evt STRENGTH_WEAK LENGTH_SHORT LOCAL","evt UNKNOWN LENGTH_SHORT GLOBAL","evt STRENGTH_MEDIUM UNKNOWN GLOBAL","evt STRENGTH_MADNESS LENGTH_CONTINUOUS","evt STRENGTH_WEAK LENGTH_SHORT ","evt  LENGTH_LONG GLOBAL","evt STRENGTH_STRONG  GLOBAL","evt",""};
  bool expect[]={true,false,false,false,true,false,false,false,false};
  for(int k=0;k<9;++k){SetRange(input,cases[k],(unsigned long)strlen(cases[k]));CHECK(action.SetDataFromString(input)==expect[k]);CHECK(action.Global && allocations==(input.PStringData?1:0) && liveStrings==1);if(k==2)CHECK(action.Strength==1 && action.Length==0);if(k==3)CHECK(action.Strength==3 && action.Length==3);}
  CCharString out;SetRange(input,"evt A B",7);CHECK(!GetDataFromEventString(input,2,out) && strcmp(Text(out),"B")==0);
  SetRange(input,"evt A B ",8);CHECK(GetDataFromEventString(input,2,out) && !out.PStringData);
  SetRange(input,"evt  A",6);CHECK(GetDataFromEventString(input,0,out) && !out.PStringData);CHECK(GetDataFromEventString(input,1,out) && strcmp(Text(out),"A")==0);
  SetRange(input," A",2);CHECK(GetDataFromEventString(input,0,out) && strcmp(Text(out),"A")==0);
 }
 CHECK(allocations==0 && liveStrings==0);puts("00870fc0_TEST PASS");return 0;
}