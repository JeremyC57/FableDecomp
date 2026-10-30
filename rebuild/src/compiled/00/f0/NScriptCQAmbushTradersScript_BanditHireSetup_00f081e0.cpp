// CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject) -- a living bandit confirms the hire to the hero and finishes speaking; then the hire count rises, WatchForBanditSpeechMarkers is spawned for the bandit and its health bar is added
// Leaf: ?BanditHireSetup@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F081E0, 440 bytes, `ret 8`; Ego_r.pdb
//   ?BanditHireSetup@CQ_AmbushTradersScript@NScript@@QAEJABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@@Z.
// Callees: the object's slots 12 GetScriptThing (hidden result), 13 Speak(const CScriptThing&, const char*, method, bool x3),
//   26 IsPerformingScriptTask; interface slots 264 GetHealth (> 0.0f 0x0122DEDC), 70 (+0x118, returns the hero; manifest
//   "GetHero"), 7 NewScriptFrame; IsActiveThreadTerminating 0x00CB7940; operator new 0x00BFEA1A (0x48 bytes),
//   CSpawnedFuncBase(name, 0) 0x00CDD450, CSpawnedFunc1 vtable 0x012F7EBC, WatchForBanditSpeechMarkers 0x00F07B80,
//   CScriptThing copy 0x004ABE90 / out-of-line dtor 0x004AA840 (the by-value parameter inside the inlined ctor),
//   AddSpawnedFunction 0x00CB7E50 ("" region), AddBanditHealthBar 0x00F07A80 (its result is returned).
// Layout (retail = Ego_r.pdb - 0x14 for the script; quest data +0xD0 counter, unnamed).
// Shape: SpawnFunction takes the region as `const CCharString& region = CCharString("")`: the default-argument temporary is
//   built at the call site and gets its own frame slot (frame+4) as in retail; a `const char*` default converted inside
//   the inline helper lets VC put the string in the dead `bandit` argument slot (0x10 frame).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
};
struct CScriptThingParam : CBaseClass {                     // the spawned function's by-value parameter: same object, out-of-line dtor
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingParam(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThingParam();                                   // 0x004AA840
};
struct CScriptThingStored : CBaseClass {
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingStored(const CScriptThingParam& other);     // 0x004ABE90
};

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_2 = 2 };
class CScriptGameResourceObjectScriptedThingBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;            // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual bool IsPerformingScriptTask() const;            // slot 26 (+0x68)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class C, class P> struct CSpawnedFunc1 : CSpawnedFuncBase {
    typedef void (C::*TFunc)(P);
    TFunc               Func;                              // +0x34
    C*                  PClass;                            // +0x38
    CScriptThingStored  Par1;                              // +0x3c
    virtual void v00();
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TFunc func, C* owner, CScriptThingParam par)
        : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par) { }
};

struct CQuestData {
    unsigned char _pad00[0xd0];
    long          BanditsHired;                            // +0xd0
};
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
    void Add(CSpawnedFuncBase* func, const CCharString& name);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& region = CCharString("")) { Add(func, region); }
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    CQuestData*           Data;                             // +0x44
};
namespace NScript {
class CQ_AmbushTradersScript : public CQuestScriptBase {
public:
    void WatchForBanditSpeechMarkers(CScriptThingParam bandit);   // 0x00F07B80
    long AddBanditHealthBar(const CScriptThing& bandit);    // 0x00F07A80
    long BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject);
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

long NScript::CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject)
{
    if (Game->GetHealth(banditObject.GetScriptThing()) > 0.0f) {
        banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_HIRE_CONFIRM", TEXT_GROUP_SELECTION_2, false, true, false);
        while (banditObject.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return 0;
        }
        if (IsActiveThreadTerminating())
            return 0;
    }
    ++Data->BanditsHired;
    SpawnFunction(new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThingParam>("WatchForBanditSpeechMarkers", 0,
        &NScript::CQ_AmbushTradersScript::WatchForBanditSpeechMarkers, this, bandit));
    return AddBanditHealthBar(bandit);
}