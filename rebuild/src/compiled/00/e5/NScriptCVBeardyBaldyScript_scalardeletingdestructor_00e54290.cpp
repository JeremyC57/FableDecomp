// NScript::CV_BeardyBaldyScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_BeardyBaldyScript@NScript@@UAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54290, 30 bytes, thiscall, `ret 4`; calls 00e542b0=NScript::CV_BeardyBaldyScript::~CV_BeardyBaldyScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012E5790 (no RTTI), the vtable the landed constructor 0x00E4F5B0 stores last
//   (??0CV_BeardyBaldyScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, Ego_r.pdb / FableWin.pdb); Ego_r.pdb ??_ECV_BeardyBaldyScript@NScript@@UAEPAXI@Z; FableWin.pdb ??_ECV_BeardyBaldyScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??1CV_BeardyBaldyScript@NScript@@UAE@XZ; FableWin.pdb ??1CV_BeardyBaldyScript@NScript@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012e5790::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00E4F5B0 (rebuild/src/compiled/00/e4/NScriptCVBeardyBaldyScript_CVBeardyBaldyScript_00e4f5b0.cpp), its own source unchanged. The constructor body is not part of this graded body.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
#include "engine/CCountedPointer.h"
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