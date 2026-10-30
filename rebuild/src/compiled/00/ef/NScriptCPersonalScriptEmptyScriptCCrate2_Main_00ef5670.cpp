// NScript::CPersonalScript_EmptyScript::CCrate2::Main() -- the second blast-box crate: find the CrateDeathMarker, guard the thread with "the crate and the marker are alive", wait until the crate is within 2 of the marker, then set off a THING_EXPLOSION_BLASTBOX at the crate and fade it out and kill it (0.5 s)
// Leaf: ?Main@CCrate2@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF5670, 457 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): the row is referenced once, from slot 1 (Main) of the RTTI-less entity vtable 0x012F5FAC (0x012F5FB0),
//   which the landed NScript::CPersonalScript_EmptyScript::CCrate2::Alloc 0x00EF1130 stores; Ego_r.pdb gives CCrate2 exactly
//   Alloc and ?Main@CCrate2@CPersonalScript_EmptyScript@NScript@@EAEXXZ (private virtual). Its twin CCrate1::Main 0x00EF54A0
//   (vtable 0x012F5F90) ends in RemoveThing instead. Catalog label sub_00ef5670 (gap scan).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CScriptThing copy 0x004ABE90 / dtor 0x004AA840
//   (out of line at the later exits), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   CPredicate_And<CIsThingAlive, CIsThingAlive>(const CIsThingAlive&, const CIsThingAlive&) 0x00E24760, ~CIsThingAlive 0x00E241B0,
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF;
//   CScriptThing slot 6 GetPos; CGameScriptInterface slots 7 NewScriptFrame, 72 GetThingWithScriptName, 104 CreateExplosion
//   (ghidra_out/cgsi_slots_aligned.tsv); FadeOutAndKillEntity 0x00CBE716 (__fastcall; Ego_r / FableWin
//   ?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z: thing in ecx, the first bool in dl, the float and the last bool pushed).
// Data: "CrateDeathMarker" 0x012F5E30, "THING_EXPLOSION_BLASTBOX" 0x012F660C, "" 0x0122D70E (shared with CCrate1); vtables CIsThingAlive 0x012C32F0,
//   CPredicate_And<CIsThingAlive, CIsThingAlive> 0x012E1B70, CPredicateBase 0x012C2FF4, CScriptThing 0x01238C8C; 2.0f / 0.5f immediates.
// Layout: Ego_r.pdb CCrate2 (28 bytes = the landed Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass
//   +0x14, PMasterData +0x18. CPredicate_And: Predicate1 +4, Predicate2 +0x14 (Ego_r, 36 bytes); CIsThingAlive: Thing +4.
// Shape: `SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker))` -- the inline operator&& builds the CPredicate_And in
//   place (operands right to left), its inline dtor stores both vtables; then the rotated distance wait.
//   CScriptThing's virtual dtor is a plain inline (defined after the shared header; not __forceinline): VC7.1 expands it at the first return
//   only and calls the out-of-line 0x004AA840 at the later exits and for the CreateExplosion result, as retail does.
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
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
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // vtable 0x01238C8C (79 slots), copy 0x004ABE90, ~CScriptThing 0x004AA840
inline CScriptThing::~CScriptThing() { }                // plain inline: expanded at the first exit, 0x004AA840 at the later ones
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool remove);       // 0x00CBE716
#include "engine/CIsThingAlive.h"                      // vtable 0x012C32F0, ~CIsThingAlive 0x00E241B0
#include "engine/CPredicate_And.h"                     // <CIsThingAlive, CIsThingAlive> vtable 0x012E1B70, ctor 0x00E24760
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
template <class P1, class P2> inline NScript::CPredicate_And<P1, P2>::~CPredicate_And() { }
#include "engine/CGameScriptInterfaceBase.h"           // vtable slots: CGameScriptInterface 0x01260F0C
#include "engine/CEntityScriptBase.h"                  // vtable 0x012C3224; SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30

namespace NScript {
class CEmptyData;
class CPersonalScript_EmptyScript {
public:
    class CCrate2;
};
class CPersonalScript_EmptyScript::CCrate2 : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
private:
    virtual void Main();
};

void CPersonalScript_EmptyScript::CCrate2::Main()
{
    CScriptThing marker = Game->GetThingWithScriptName("CrateDeathMarker");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, marker, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CreateExplosion("THING_EXPLOSION_BLASTBOX", Me.GetPos(), 0, "");
    FadeOutAndKillEntity(Me, 0.5f, true, true);
}
}   // namespace NScript