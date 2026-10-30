// NScript::CQ_RansomVictimScript::CRansommer::Init() -- makes the entity unpushable by the hero: Game->SetIsPushableByHero(Me, false), Me passed by value
// Leaf: ?Init@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE1CC0, 49 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): NScript::CQ_RansomVictimScript::CRansommer::Init(); evidence: this body is slot 2 (Init) of the RTTI-less entity-script vtable 0x012DD1C8 (.rdata 0x012DD1D0); that vtable is stored, after the CEntityScriptBase vtable 0x012C3224, by the landed factory 0x00DE1BA0 ?Alloc@CRansommer@CQ_RansomVictimScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z (`new` 0x1C bytes); the Ego_r/FableWin.pdb name sets hold ?Init@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Label: the manifest's "sub_00de1cc0" is a gap-scan placeholder (family J038: six one-call entity-script Init bodies).
// Callee: CGameScriptInterfaceBase virtual slot 844 (+0xD30) SetIsPushableByHero(CScriptThing, bool) (ghidra_out/cgsi_slots_aligned.tsv:
//   ?SetIsPushableByHero@CGameScriptInterface@@UAEXVCScriptThing@@_N@Z, implementation 0x008A6DD0); the by-value CScriptThing is
//   copy-constructed inline in the argument slot (CBaseClass vtable 0x01231710, then CScriptThing 0x01238C8C, PImp shared with
//   the count bumped) and destroyed by the callee.
// Layout: Ego_r.pdb CEntityScriptBase (0x14) {vptr, Game (CGameScriptInterfaceBase*) +4, Me (CScriptThing, 0x0C) +8};
//   CRansommer (0x1C): ParentClass (CQ_RansomVictimScript*) +0x14, PMasterData (CQ_SunnyvaleMasterData*) +0x18.
class CCPPointerInfo {
public:
    long RefCount;                                      // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {               // 0x0C bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CEntityScriptBase {                              // vtable 0x012C3224
public:
    virtual ~CEntityScriptBase();                      // slot 0
private:
    virtual void Main();                               // slot 1
    virtual void Init();                               // slot 2
public:
    CGameScriptInterfaceBase* Game;                    // +0x04
    CScriptThing              Me;                      // +0x08
};
class CQ_RansomVictimScript {
public:
    class CRansommer;
};
class CQ_RansomVictimScript::CRansommer : public CEntityScriptBase {   // vtable 0x012DD1C8
private:
    virtual void Init();                               // slot 2
public:
    CQ_RansomVictimScript*            ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
};
void CQ_RansomVictimScript::CRansommer::Init()
{
    Game->SetIsPushableByHero(Me, false);
}
}