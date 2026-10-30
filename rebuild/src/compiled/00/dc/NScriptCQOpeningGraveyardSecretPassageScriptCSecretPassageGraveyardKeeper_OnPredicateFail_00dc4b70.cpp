// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::OnPredicateFail() -- unless the doors are open, tell the hero about the key and give him the graveyard gate key; when the keeper is killed by the hero count a friendly killed and at four record that every innocent was killed
// Leaf: ?OnPredicateFail@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC4B70, 175 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012DA658, which the entity Alloc 0x00DC4AA0 (`NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Alloc`) stores; ?OnPredicateFail@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dc4b70" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 115 DisplayGameInfo, 121 GiveHeroObject; CScriptThing::MsgIsKilledBy (slot 18).
// Data: "TEXT_QST_049_KEY_PICKUP" @ 0x012DA690, "OBJECT_GRAVEYARD_GATE_KEY" @ 0x012DA674, "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent doors_open +0x4c, FriendliesKilled +0x6c (Ego_r.pdb +0x60/+0x80); CQ_SunnyvaleMasterData GraveyardKillAllInnocents +0x11c.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb offsets (same in retail)
public:
    unsigned char _pad000[0x11c];
    bool GraveyardKillAllInnocents;                     // +0x11c
};
class CQ_OpeningGraveyardSecretPassageScript {
public:
    unsigned char _pad000[0x4c];
    bool doors_open;                                    // +0x4c (Ego_r.pdb +0x60)
    unsigned char _pad04d[0x1f];
    long FriendliesKilled;                              // +0x6c (Ego_r.pdb +0x80)
    class CSecretPassageGraveyardKeeper;
};
class CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper : public CEntityScriptBase {
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      already_talked_to;          // +0x1c
    bool                      Helmfound;                  // +0x1d
    bool                      Swordfound;                 // +0x1e
    bool                      Shieldfound;                // +0x1f
    bool                      Armourfound;                // +0x20
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::OnPredicateFail()
{
    if (!ParentClass->doors_open) {
        Game->DisplayGameInfo("TEXT_QST_049_KEY_PICKUP");
        Game->GiveHeroObject("OBJECT_GRAVEYARD_GATE_KEY", -1, false);
    }
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO")) {
        ParentClass->FriendliesKilled++;
        if (ParentClass->FriendliesKilled == 4)
            PMasterData->GraveyardKillAllInnocents = true;
    }
}