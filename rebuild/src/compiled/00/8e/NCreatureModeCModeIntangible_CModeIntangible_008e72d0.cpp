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
template<class T>class CBasicString {protected:T*PData;unsigned long StringLength;
    // Layout: original allocation-length and extension flag bytes are unread.
    unsigned char unobservedStringFlags[5];
public:unsigned long GetLength()const{return StringLength;}const T*PeekData()const{return PData;}
};
class CCharStringData {public:CBasicString<char>Data;protected:long NoRefs;};
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
class CMainGameComponent{protected:static long ConstantFPS;public:static __forceinline long __fastcall GetConstantFPS(){return ConstantFPS;}};
namespace NCreatureMode {
class CModeDragonBase:public CTCCreatureModeManager::CModeBase {
protected:CAnimFilterOfGroup Filter;CIntelligentPointer<CThing>PTarget;
public:
    CModeDragonBase(CThing&thing,const CCharString&group):CModeBase(thing),Filter(group){}
};
class CModeIntangible:public CModeDragonBase {
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