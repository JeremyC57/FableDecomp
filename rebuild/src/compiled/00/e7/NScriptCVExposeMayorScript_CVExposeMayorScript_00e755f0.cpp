// CV_ExposeMayorScript::CV_ExposeMayorScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E96CC, four entity handles (Suspicious, LadyGrey, Sheriff, Rhodri) and the empty Evidence string
// Leaf: ??0CV_ExposeMayorScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E755F0, length 142 bytes (padded to 144), `ret 8`.
// Factory 0x00E7B290 (`push 0A0h`, this ctor) is registered at 0x00CD866B under "S_VEM" / "V_ExposeMayor".
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x4),
// CCharString::CCharString() @ 0x0099E4B0.
// Layout (retail = Ego_r.pdb - 0x14): Path +0x48, StageNumber +0x4c, six bools +0x50..+0x55 (not initialised),
// seh_Suspicious +0x58, seh_LadyGrey +0x68, seh_Sheriff +0x78, seh_Rhodri +0x88 (vtable 0x0127094C), Evidence +0x98,
// MaxFlashDistFromStables +0x9c; size 0xA0.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
class CTimer { public:                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
};
namespace NScript {
class CV_ExposeMayorScript : public CQuestScriptObject {
public:   // vtable 0x012E96CC
    long Path;                                     // +0x48 (PATH_ID)
    long StageNumber;                              // +0x4c (STAGE_IN_PATH)
    bool HeroFlashedLightsCorrectly;               // +0x50
    bool PrisonerTalkedAboutLamp;                  // +0x51
    bool ResetLamp;                                // +0x52
    bool SaidYesToBecomeMayor;                     // +0x53
    bool QuestCardGiven;                           // +0x54
    bool MayorExposeGhostCutscenePlayed;           // +0x55
    CScriptGameResourceObjectScriptedThingBase seh_Suspicious;   // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_LadyGrey;     // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;      // +0x78
    CScriptGameResourceObjectScriptedThingBase seh_Rhodri;       // +0x88
    CCharString Evidence;                          // +0x98
    float MaxFlashDistFromStables;                 // +0x9c
    virtual void v00();
    CV_ExposeMayorScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;
NScript::CV_ExposeMayorScript::CV_ExposeMayorScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}