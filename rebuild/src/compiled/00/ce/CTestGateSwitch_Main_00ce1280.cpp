// CTestGateSwitch::Main() -- test gate switch: open or close every "TestGate" thing to match the switch state (with an
// on-screen reminder of the required script name), then whenever the hero uses the switch toggle the state and
// re-apply it, once per script frame until the thread terminates
// Leaf: Main
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
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                           // slot 0 (out of line 0x004AA840)
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
};
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
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CTestGateSwitch : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CPersonalScript_GlobalThingsScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  GatesOpen;                                   // +0x1c
    void Main();
};

void CTestGateSwitch::Main()
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