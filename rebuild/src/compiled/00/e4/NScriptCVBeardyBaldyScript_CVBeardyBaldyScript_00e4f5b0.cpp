// CV_BeardyBaldyScript::CV_BeardyBaldyScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E5790, the three required-hair strings, the registered random-speech timer and ten empty speech strings
// Leaf: ??0CV_BeardyBaldyScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4F5B0, length 101 bytes (padded to 112), `ret 8`.
// Factory 0x00E54260 (`push 0A4h`, this ctor) is registered at 0x00CD82D1 under "S_VBB" / "V_BeardyBaldy".
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (3 + an inline counted
// loop of 10), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8.
// Layout (retail = Ego_r.pdb CV_BeardyBaldyScript - 0x14): RequiredHairdo +0x58, RequiredBeard +0x5c,
// RequiredTash +0x60, RandomSpeechTimer +0x74, RandomSpeech[10] +0x78, BeardyBaldyPhase +0xa0; size 0xA4.
// Shape: the landed quest-script ctor family (novtable intermediate base carrying the two arguments).
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
class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
template <class T> class CCountedPointer { public:                     // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
class CV_BeardyBaldyScript : public CQuestScriptObject {
public:   // vtable 0x012E5790
    bool QuestFinished;                            // +0x48
    bool QuestComplete;                            // +0x49
    bool QuestActivated;                           // +0x4a
    long QuestPhase;                               // +0x4c
    bool InitialisePhase;                          // +0x50
    bool IntroComplete;                            // +0x51
    bool Phase1RequirementsComplete;               // +0x52
    bool Phase2RequirementsComplete;               // +0x53
    bool Phase3RequirementsComplete;               // +0x54
    CCharString RequiredHairdo;                    // +0x58
    CCharString RequiredBeard;                     // +0x5c
    CCharString RequiredTash;                      // +0x60
    bool AcquiredNewHairdo;                        // +0x64
    bool AcquiredNewBeard;                         // +0x65
    bool AcquiredNewTash;                          // +0x66
    bool AllHairChanged;                           // +0x67
    bool BeardyBaldyKilled;                        // +0x68
    bool BarberLadyKilled;                         // +0x69
    bool BeardyBaldyLeft;                          // +0x6a
    long IncorrectHairComboCount;                  // +0x6c
    bool AttackedByHero;                           // +0x70
    CTimer RandomSpeechTimer;                      // +0x74
    CCharString RandomSpeech[10];                  // +0x78
    long BeardyBaldyPhase;                         // +0xa0
    virtual void v00();
    CV_BeardyBaldyScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;
NScript::CV_BeardyBaldyScript::CV_BeardyBaldyScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}