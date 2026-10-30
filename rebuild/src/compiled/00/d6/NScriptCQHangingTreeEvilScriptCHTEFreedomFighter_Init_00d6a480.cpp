// NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Init() -- HeroHitMe = 0; make Me and the hero allies both ways and put Me in FACTION_BANDITS_FRIENDLY
// Leaf: ?Init@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6A480, 107 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D2170, which the entity Alloc 0x00D6A3D0 (`NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Alloc`) stores; ?Init@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d6a480" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 70 GetHero, 599 EntitySetThingAsAllyOfThing, 601 EntitySetInFaction.
// Data: "FACTION_BANDITS_FRIENDLY" @ 0x012D218C.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CHTE_FreedomFighter: HeroHitMe +0x1c.
// Shape: the outer call's vtable is read before the GetHero argument call and reused for the call (ebx).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript {
public:
    class CHTE_FreedomFighter;
};
class CQ_HangingTreeEvilScript::CHTE_FreedomFighter : public CEntityScriptBase {
public:
    CQ_HangingTreeEvilScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      HeroHitMe;                  // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Init()
{
    HeroHitMe = 0;
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
}