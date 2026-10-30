// CCreatureAction_SpiritReturnToHero::PerformAction() -- send the spirit creature's physics towards the main player's hero (or towards its own position when the player has no hero)
// Grade: DIFFER(104v104) -- wave 2 sec3: callees respelled to their bound PDB names (GetPControlledCreature 0x00487DC0, GetPos 0x004C73D0, GetPItem 0x00A01B50 -> CBaseObject*), player via the PDB inline CCreatureActionBase::GetPlayer(), GetPhysicsTC()/GetPCreature() accessors, engine/CPlayerManager.h; residue unchanged: this esi vs retail edi, player edi vs esi (tried: GetPlayer accessor, if/else select, GetPCreature accessor)
// Leaf: ?PerformAction@CCreatureAction_SpiritReturnToHero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854720, length 104 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (the main game component's player manager at +0x1c),
// CPlayer::GetPControlledCreature() const @ 0x00487DC0 (called twice), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50
// (PCreature at +8, twice), CThing::GetPos() const @ 0x004C73D0, the physics component's vtable slot 31 (`call [edx+7Ch]`).
// Globals: g_pMainGameComponent @ 0x013B86A0.
// Shape: the player comes from the PDB's protected inline CCreatureActionBase::GetPlayer() (Ego_r ?GetPlayer@CCreatureActionBase@@IAEAAVCPlayer@@XZ);
// the target is the ternary `hero ? hero : own creature` with the hero accessor called twice.
#include "engine/CPlayerManager.h"
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void SetTargetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing { public:
    unsigned char _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;              // +0x60
    const C3DVector& GetPos() const;        // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }   // Ego_r ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
};
class CThingCreatureBase : public CThing { };
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CMainGameComponent { public:
    unsigned char _pad0[0x1c];
    CPlayerManager* PPlayerManager;         // +0x1c
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }   // Ego_r ?GetPlayerManager@CMainGameComponent@@QAEAAVCPlayerManager@@XZ
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CBaseObject;
class CBaseIntelligentPointer { public:
    virtual void v00();
    CBaseObject* PItem;                     // +4
    CBaseObject* GetPItem() const;          // 0x00A01B50
};
class CCreatureActionBase {                 // retail sizeof 0xa8
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    const CThingCreatureBase* GetPCreature() { return (const CThingCreatureBase*)PCreature.GetPItem(); }   // Ego_r ?GetPCreature@CCreatureActionBase@@QAEPBVCThingCreatureBase@@XZ
protected:
    CPlayer& GetPlayer() { return g_pMainGameComponent->GetPlayerManager().GetMainPlayer(); }
};
class CCreatureAction_SpiritReturnToHero : public CCreatureActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_SpiritReturnToHero::PerformAction()
{
    CPlayer& player = GetPlayer();
    C3DVector target = (player.GetPControlledCreature() ? player.GetPControlledCreature() : GetPCreature())->GetPos();
    const_cast<CThingCreatureBase*>(GetPCreature())->GetPhysicsTC()->SetTargetPosition(target);
}
