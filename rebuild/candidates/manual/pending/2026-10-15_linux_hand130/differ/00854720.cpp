// CCreatureAction_SpiritReturnToHero::PerformAction() -- send the spirit creature's physics towards the main player's hero (or towards its own position when the player has no hero)
// Grade: DIFFER(104v104) -- register swap only: retail keeps `this` in edi and the player in esi (`push esi ; push edi ; mov edi,ecx`), the lane gives `this` esi and the player edi (push edi scheduled after the global load); every other byte matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854720, length 104 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (the world's player manager at +0x1c; manifest "Catch@0040d959" is propagated),
// CPlayer::GetHeroCreature @ 0x00487DC0 (`add ecx,2Ch ; jmp GetPItem`, called twice; manifest "_Cons_val<...>" is propagated),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CThing::GetPosition @ 0x004C73D0 (const C3DVector&),
// the physics component's vtable slot 31 (`call [edx+7Ch]`, a target position by reference).
// Globals: g_pWorld @ 0x013B86A0 (loaded before the frame is built).
// Shape: the player is a named local (esi); the target is the ternary `hero ? hero : own creature` with the hero accessor called twice; its
// position is copied into a 12-byte frame local (POD copy through ecx/edx/eax) before the creature/physics/vtable fetch and the push of its address.
// The PDB header (CCreatureAction_SpiritReturnToHero.h) flattens the base; `this` is modelled locally (retail CCreatureActionBase is 0xa8 bytes).
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void SetTargetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
struct CThing {
    unsigned char _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    const C3DVector& GetPosition() const;   // 0x004C73D0
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }   // inline accessor: loads the component straight into ecx
};
struct CPlayer {
    CThing* GetHeroCreature();              // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();               // 0x00449970
};
struct CWorld {
    unsigned char _pad0[0x1c];
    CPlayerManager* PlayerManager;          // +0x1c
};
extern CWorld* g_pWorld;                    // 0x013B86A0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
struct CCreatureAction_SpiritReturnToHero : CCreatureActionBase {
    virtual void PerformAction();
};

void CCreatureAction_SpiritReturnToHero::PerformAction()
{
    CPlayer* player = g_pWorld->PlayerManager->GetMainPlayer();
    C3DVector target = (player->GetHeroCreature() ? player->GetHeroCreature() : PCreature.GetPItem())->GetPosition();
    PCreature.GetPItem()->GetPhysics()->SetTargetPosition(target);
}
