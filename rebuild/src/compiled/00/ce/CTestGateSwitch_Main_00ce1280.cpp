// NScript::CPersonalScript_GlobalThingsScript::CTestGateSwitch::Main() -- test gate switch: open or close every "TestGate" thing to match the switch state (with an
// on-screen reminder of the required script name), then whenever the hero uses the switch toggle the state and
// re-apply it, once per script frame until the thread terminates
// Leaf: Main
// Identity (naming wave 11): ?Main@CTestGateSwitch@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012C33E0 slot 1 is stored by the factory 0x00CDF930 registered as "TestGateSwitch"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE1280, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30,
//   vector<CScriptThing>::~vector 0x008AC970 (out of line on the inner exits), CScriptThing slots 0 (virtual dtor in the
//   inlined ~vector) and 20 MsgIsUsedBy, free 0x00BFEA14; CGameScriptInterface slots 7 NewScriptFrame,
//   75 GetAllThingsWithScriptName, 111 ShowOnScreenMessage, 425 OpenDoor, 426 CloseDoor.
// Layout: Ego_r.pdb CTestGateSwitch (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, GatesOpen +0x1c. Model types as the landed CCOE_Victim::Main 0x00E6B400.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()                            // inline here (out-of-line COMDAT copy 0x008AC970)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
}
using std::vector;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CPersonalScript_GlobalThingsScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CTestGateSwitch;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_GlobalThingsScript;
namespace NScript {
class CPersonalScript_GlobalThingsScript::CTestGateSwitch : public CEntityScriptBase {
public:
    void* ParentClass;                                 // +0x14 CPersonalScript_GlobalThingsScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  GatesOpen;                                   // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private
public:
};
}

void NScript::CPersonalScript_GlobalThingsScript::CTestGateSwitch::Main()
{
    vector<CScriptThing> gates;
    Game->GetAllThingsWithScriptName("TestGate", gates);
    Game->ShowOnScreenMessage("YOUR GATE MUST HAVE A SCRIPT NAME OF 'TestGate' - (case sensitive without the quotes)", 10.0f);
    unsigned int i;
    for (i = 0; i < gates.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (GatesOpen) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) {
ool:
                return;
            }
#pragma inline_depth()
            Game->OpenDoor(gates[i]);
        } else {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            Game->CloseDoor(gates[i]);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            GatesOpen = !GatesOpen;
            for (unsigned int i = 0; i < gates.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (GatesOpen) {
                    if (IsActiveThreadTerminating())
                        goto ool;
                    Game->OpenDoor(gates[i]);
                } else {
                    if (IsActiveThreadTerminating())
                        goto ool;
                    Game->CloseDoor(gates[i]);
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}