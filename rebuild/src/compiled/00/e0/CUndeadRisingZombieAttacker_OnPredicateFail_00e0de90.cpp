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
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                     // +0x04 Data, +0x08 Info
    ~CScriptThing() { }                                     // out of line 0x004AA840
    virtual void t01() const; virtual void t02() const; virtual void t03() const; virtual void t04() const; virtual void t05() const;
    virtual const C3DVector& GetPos() const                 // slot 6 (+0x18)
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t07() const; virtual void t08() const; virtual void t09() const; virtual void t10() const; virtual void t11() const; virtual void t12() const; virtual void t13() const; virtual void t14() const;
    virtual void t15() const; virtual void t16() const; virtual void t17() const; virtual void t18() const; virtual void t19() const; virtual void t20() const; virtual void t21() const; virtual void t22() const;
    virtual void t23() const; virtual void t24() const; virtual void t25() const; virtual void t26() const; virtual void t27() const; virtual void t28() const; virtual void t29() const; virtual void t30() const;
    virtual void t31() const; virtual void t32() const; virtual void t33() const; virtual void t34() const; virtual void t35() const; virtual void t36() const; virtual void t37() const; virtual void t38() const;
    virtual void t39() const; virtual void t40() const; virtual void t41() const; virtual void t42() const; virtual void t43() const; virtual void t44() const; virtual void t45() const; virtual void t46() const;
    virtual void t47() const; virtual void t48() const; virtual void t49() const; virtual void t50() const; virtual void t51() const; virtual void t52() const; virtual void t53() const; virtual void t54() const;
    virtual void t55() const; virtual void t56() const; virtual void t57() const; virtual void t58() const; virtual void t59() const; virtual void t60() const; virtual void t61() const; virtual void t62() const;
    virtual void t63() const; virtual void t64() const; virtual void t65() const; virtual void t66() const; virtual void t67() const; virtual void t68() const; virtual void t69() const; virtual void t70() const;
    virtual void t71() const; virtual void t72() const; virtual void t73() const; virtual void t74() const; virtual void t75() const; virtual void t76() const;
    virtual bool IsNull() const { return !PImp.Data || PImp.Data->IsNull(); }   // slot 77 (+0x134)
};
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

class CScriptBase { public:                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct CQ_UndeadRisingScript : CScriptBase {
    void ActivateNextLamp();                                // 0x00E0E040
};
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