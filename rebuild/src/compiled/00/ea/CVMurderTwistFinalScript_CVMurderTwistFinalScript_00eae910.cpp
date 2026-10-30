// CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012EF978, the empty Sheriff script thing, the empty Guards vector, the Sheriff entity handle and the empty seh_Guards vector
// Leaf: ??0CV_MurderTwistFinalScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EAE910, length 107 bytes (padded to 112), `ret 8`.
// Factory 0x00EB6A60 (`push 88h`, this ctor) is registered at 0x00CD9094 under "S_VMTF" / "V_MurderTwistFinal".
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseClass::CBaseClass @ 0x0099A2D0 (CScriptThing root, vtable 0x01238C8C),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380.
// Layout (retail = Ego_r.pdb - 0x14, then -0x18 after the 12-byte Guards vector): SheriffState +0x48,
// AssassinState +0x4c, TaskDeclined +0x50, PreemptiveAttackInProgress +0x51, Sheriff +0x54, Guards +0x60,
// seh_Sheriff +0x6c (vtable 0x0127094C), seh_Guards +0x7c; size 0x88.
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CCPPointerInfo;
struct CCountedPointer_CScriptThing {              // CScriptThing::PImp
    CScriptThingImp* Data;                         // +0
    CCPPointerInfo*  Info;                         // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    CCountedPointer_CScriptThing PImp;             // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};
struct CV_MurderTwistFinalScript : CQuestScriptObject {   // vtable 0x012EF978
    long SheriffState;                             // +0x48 (ESheriffState)
    long AssassinState;                            // +0x4c (EAssassinState)
    bool TaskDeclined;                             // +0x50
    bool PreemptiveAttackInProgress;               // +0x51
    CScriptThing Sheriff;                          // +0x54
    CRetailVector<CScriptThing> Guards;            // +0x60
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    CRetailVector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;   // +0x7c
    virtual void v00();
    CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game);
};
CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}