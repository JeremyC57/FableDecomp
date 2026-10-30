// CSecretPassageFirstGate::Main() -- the graveyard secret-passage gate: mark it on the mini-map (until the demon door is
//   open), keep it usable and persistent; while the graveyard keeper lives and the doors are shut, using it shows the
//   "gate locked" notice; then wait for the hero to use it, open it, take the gate key and move the marker to the
//   skeleton.
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC8030, 938 bytes (+ int3 padding), plain `ret`. Entity-script slot 1 (Main).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; ~CScriptThing 0x004AA840 (out-of-line
//   copy) / ~CBaseClass 0x0099A2E0; operator delete 0x00BFE9BC; CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30.
//   CGameScriptInterface slots (ghidra_out/cgsi_slots_aligned.tsv) 7 NewScriptFrame, 40 MsgIsGameInfoClickedPast,
//   72 GetThingWithScriptName, 115 DisplayGameInfo, 125 TakeObjectFromHero, 350 MiniMapAddMarker, 352
//   MiniMapRemoveMarker, 425 OpenDoor, 447 SetThingAsUsable, 477 SetThingPersistent; CScriptThing slots 20 MsgIsUsedBy,
//   75 IsAlive. Literals "HUD_ORB_QUEST_CORE" 0x01258AB0, "SecretPassageGraveyardKeeper" 0x012DA318, "SCRIPT_NAME_HERO"
//   0x0125D1C8, "TEXT_QST_049_GATE_LOCKED" 0x012DA9C8, "OBJECT_GRAVEYARD_GATE_KEY" 0x012DA674, "SecretPassageSkeleton"
//   0x012DA364.
// Layout: Ego_r.pdb CSecretPassageFirstGate (28 bytes, = retail): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass
//   +0x14 (CQ_OpeningGraveyardSecretPassageScript, retail = PDB - 0x14: doors_open +0x4c, HaveOpenedDemonDoor +0x72),
//   PMasterData +0x18. CScriptThing model as the landed CSecretBook::Main 0x00DDE940 / CBanditAttacker::Main 0x00D1F610.
// Shape: the keeper test is a `&&` loop condition, so its two temporaries carry VC's destruction flags (the frame word
//   zeroed in the prologue); the second wait is `while (!used)` (materialised `neg/sbb/inc`).
namespace NScript { class CPredicateBase; }
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const;
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const;
    virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const;
    virtual void s019() const;
    virtual bool MsgIsUsedBy(const CCharString& user) const;            // slot 20 (+0x50)
    virtual void s021() const; virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const;
    virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const; virtual void s032() const;
    virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const;
    virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const;
    virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const;
    virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; virtual void s056() const;
    virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const;
    virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const;
    virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const;                                       // slot 75 (+0x12c)
    void*           PImp_Data;                          // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
    ~CScriptThing()                                     // inline here; out-of-line copy 0x004AA840
    {
        if (PImp_Info) {
            --PImp_Info->RefCount;
            if (PImp_Info->RefCount == 0) {
                PImp_Info->DeleteFunc(PImp_Info->Data);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad00[0x4c];
    bool          doors_open;                           // +0x4c
    unsigned char _pad4d[0x72 - 0x4d];
    bool          HaveOpenedDemonDoor;                  // +0x72
};
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};
struct CSecretPassageFirstGate : CEntityScriptBase {
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData;   // +0x18
    void Main();
};

void CSecretPassageFirstGate::Main()
{
    if (!ParentClass->HaveOpenedDemonDoor) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    }
    Game->SetThingAsUsable(Me, true);
    Game->SetThingPersistent(Me, true);
    while (Game->GetThingWithScriptName("SecretPassageGraveyardKeeper").IsAlive() && !ParentClass->doors_open) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_049_GATE_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->doors_open) {
        if (IsActiveThreadTerminating())
            return;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->OpenDoor(Me);
        Game->SetThingAsUsable(Me, false);
        ParentClass->doors_open = true;
        Game->TakeObjectFromHero("OBJECT_GRAVEYARD_GATE_KEY");
    }
    if (!ParentClass->HaveOpenedDemonDoor) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(Game->GetThingWithScriptName("SecretPassageSkeleton"), "HUD_ORB_QUEST_CORE");
        Game->MiniMapRemoveMarker(Me);
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}