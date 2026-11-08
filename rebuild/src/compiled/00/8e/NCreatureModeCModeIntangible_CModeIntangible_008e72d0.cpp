// 008e72d0 NCreatureMode::CModeIntangible::CModeIntangible -- initialize a timed intangible mode
// Leaf: ??0CModeIntangible@NCreatureMode@@QAE@AAVCThing@@MPAVCCreatureActionBase@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Flags:
#include "fable_compiler.h"
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
#include "engine/CCountedPointer.h"
class CTCCreatureModeManager {
public:class CModeBase {
public:virtual ~CModeBase();
    // Layout: ParentThing, WeaponBasedFilter, removal countdown and active state are unread.
    unsigned char unobservedModeState[24];CModeBase(CThing&);
};};
// Layout: static-only declaration; no CMainGameComponent instance or full extent claimed.
class CMainGameComponent{protected:static long ConstantFPS;public:static FABLE_FORCEINLINE long __fastcall GetConstantFPS(){return ConstantFPS;}};
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