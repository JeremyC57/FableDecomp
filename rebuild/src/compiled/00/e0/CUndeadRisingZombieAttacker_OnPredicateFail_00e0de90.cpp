// CUndeadRisingZombieAttacker::OnPredicateFail() -- when the zombie's predicate fails within 6 units of the Undead Rising centre, spawn the parent quest's ActivateNextLamp
// Leaf: ?OnPredicateFail@CUndeadRisingZombieAttacker@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0DE90, 428 bytes (432 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator+(const CCharString&,
//   const CCharString&) 0x0099F570 (`__fastcall`), operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc
//   vtable 0x012DFD50), CScriptBase::AddSpawnedFunction 0x00CB7E50, CQ_UndeadRisingScript::ActivateNextLamp 0x00E0E040,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slots 6 GetPos and 77 IsNull (inline bodies for the
//   local, virtual on Me); through the interface (+4) GetThingWithScriptName (slot 72, hidden result). The zero vector
//   0x0143E8E0, 36.0f 0x0126007C.
// Strings: "UndeadRisingCentre" 0x012DFE6C, "ParentClass." 0x012C9934, "ActivateNextLamp" 0x012E00B4, "" 0x0122D70E.
// Layout: the entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (Ego_r.pdb).
// Shape: the spawned name is two explicit temporaries joined by operator+ and the region an explicit CCharString(""),
//   built after the new-expression; the distance test is the inline (a - b).MagSquared() against 36.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
extern C3DVector g_ZeroVector;                              // 0x0143E8E0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }
inline bool CScriptThing::IsNull() const { return !PImp.Data || PImp.Data->IsNull(); }
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_UndeadRisingScript : public CScriptBase { public:
    void ActivateNextLamp();                                // 0x00E0E040
};
}
using NScript::CQ_UndeadRisingScript;
class CUndeadRisingZombieAttacker {
public:
    void*                  __vftable;                       // +0x00
    CGameScriptInterface*  Game;                            // +0x04
    CScriptThing           Me;                              // +0x08
    CQ_UndeadRisingScript* ParentClass;                     // +0x14
    void OnPredicateFail();
};

void CUndeadRisingZombieAttacker::OnPredicateFail()
{
    CScriptThing centre = Game->GetThingWithScriptName("UndeadRisingCentre");
    if (!centre.IsNull() && (Me.GetPos() - centre.GetPos()).MagSquared() < 36.0f)
        ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_UndeadRisingScript>(CCharString("ParentClass.") + CCharString("ActivateNextLamp"), 0, ParentClass, &CQ_UndeadRisingScript::ActivateNextLamp), CCharString(""));
}