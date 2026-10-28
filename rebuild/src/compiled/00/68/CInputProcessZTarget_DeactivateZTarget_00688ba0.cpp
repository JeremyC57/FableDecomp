// CInputProcessZTarget::DeactivateZTarget(CProcessedInput& processed) -- queue game event 0x3e (deactivate Z-target) and, when the live hero is in ranged third-person mode, game event 0x3a as well
// Grade: DIFFER(149v149) -- /GS lane gap: `processed` is loaded into edi before the GetPControlledCreature call (parameter shadowing); retail loads it after the call
// Leaf: DeactivateZTarget
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688BA0, length 149 bytes.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), the hero test @ 0x0068EB20 (component kind 0x46 lookup),
// CProcessedInput::AddGameEvent @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); two event
// locals (0x50-byte frame + cookie).
class CInputEvent;
struct CMemoryBuffer {
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsInThirdPersonRangedMode();              // 0x0068EB20
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessZTarget : public CInputProcessGameBase {
public:
    void DeactivateZTarget(CProcessedInput& processed);
};

void CInputProcessZTarget::DeactivateZTarget(CProcessedInput& processed)
{
    CThingCreatureBase* hero = MyPlayer->GetPControlledCreature();
    CGameEvent deactivate(0x3e);
    processed.AddGameEvent(deactivate);
    if (hero && !hero->m_dead && hero->IsInThirdPersonRangedMode()) {
        CGameEvent ranged(0x3a);
        processed.AddGameEvent(ranged);
    }
}