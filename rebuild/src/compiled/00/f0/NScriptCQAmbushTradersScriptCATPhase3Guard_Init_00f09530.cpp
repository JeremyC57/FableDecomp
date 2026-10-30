// NScript::CQ_AmbushTradersScript::CAT_Phase3Guard::Init() -- spawn the parent script's WatchForEnemyDeath thread on the entity's own thing
// Leaf: ?Init@CAT_Phase3Guard@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f09530, 258 bytes, thiscall, plain `ret` (virtual, entity vtable slot 2), no EH frame.
// Identity (proven): slot 2 (Init) of the entity vtable 0x012F7E68 that the landed NScript::CQ_AmbushTradersScript::CAT_Phase3Guard::Alloc 0x00F050E0 stores; Ego_r.pdb / FableWin.pdb name ?Init@CAT_Phase3Guard@CQ_AmbushTradersScript@NScript@@EAEXXZ in the PDB sets; it spawns ?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z 0x00F08590 (the member pointer stored at +0x34; landed) under the name "ParentClass." + "WatchForEnemyDeath".
// Callees: operator new 0x00BFEA1A (0x48), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall), CSpawnedFuncBase(const CCharString&, unsigned
//   long) 0x00CDD450, CScriptThing(const CScriptThing&) 0x004ABE90 (the functor's member copy) / ~CScriptThing 0x004AA840,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50. Data: CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing> vtable 0x012F7EBC,
//   "ParentClass." 0x012C9934, "WatchForEnemyDeath" 0x012F8114, "" 0x0122D70E.
// Shape (family L139): Ego_r.pdb CSpawnedFunc1<T, CScriptThing> (0x48: Func +0x34, PClass +0x38, Par1 +0x3c); the by-value
//   argument copy of Me is inline, the member copy inside the inlined functor constructor is the out-of-line 0x004ABE90
//   (`#pragma inline_depth(1)` around the function, the counted-pointer copy __forceinline); the name temporaries are
//   conditional (flags 7 in ebx); the region name is SpawnFunction's default CCharString argument, built after the
//   new-expression so ParentClass is re-read for the call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& lhs, const CCharString& rhs);   // 0x0099F570
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CCountBlock { long References; };
template <class T> class CCountedPointer { public:
    void* Data;
    CCountBlock* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptBase {
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void SpawnFunction(CSpawnedFuncBase* func, const CCharString& regionName = CCharString("")) { AddSpawnedFunction(func, regionName); }
};
class CGameScriptInterface;
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    void WatchForEnemyDeath(CScriptThing thing);
    struct CAT_Phase3Guard {
        CGameScriptInterface* Game;
        CScriptThing Me;
        CQ_AmbushTradersScript* ParentClass;
        void* PMasterData;
        virtual void v0();
        virtual void Main();
    private:
        virtual void Init();
    };
};
#pragma inline_depth(1)
void CQ_AmbushTradersScript::CAT_Phase3Guard::Init()
{
    ParentClass->SpawnFunction(new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForEnemyDeath"), 0, ParentClass, &CQ_AmbushTradersScript::WatchForEnemyDeath, Me));
}
}