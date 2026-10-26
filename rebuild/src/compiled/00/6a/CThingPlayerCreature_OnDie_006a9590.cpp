// CThingPlayerCreature::OnDie() -- tells the owning player a creature died; for the main player records a world value and lets the nearest village process crime deeds; then the base OnDie
// Retail: Fable.exe 0x006A9590, 73 bytes.
// Callees: CThing::GetPlayer 0x004C7A30, CPlayer::GlobalEventCreatureDied 0x0087C580, CPlayer::IsMainPlayer
// 0x00487520, 0x004C79A0 (a member that ignores `this` and returns a world sub-object; its manifest
// label "CAnimateThing::GetMeshBank" is propagated), CPlayer::GetPNearestTCVillage 0x00488620, the
// village's crime-deed processing 0x0065C2D0, CThingCreatureBase::OnDie 0x00663F50 (tail jump).
// Shape: the player in a local (esi), `this` kept in edi; the world value is read and stored to the
// player's +0x3c before the village call; the base call is the last statement (sibling `jmp`).
struct CWorldState {
    unsigned char _pad00[0x80];
    long          m_value80;                             // +0x80
};

class CTCVillage { public:
    void ProcessCrimeDeeds();                            // 0x0065C2D0
};

class CThingCreatureBase;

class CPlayer { public:
    unsigned char _pad00[0x3c];
    long          PlayerRespawnDelay;                             // +0x3c
    void        GlobalEventCreatureDied(CThingCreatureBase* creature);   // 0x0087C580
    bool        IsMainPlayer();                          // 0x00487520
    CTCVillage* GetPNearestTCVillage();                  // 0x00488620
};

class CThingCreatureBase { public:
    CPlayer*     GetPlayer();                            // 0x004C7A30 (CThing)
    CWorldState* GetWorldState();                        // 0x004C79A0
    void         OnDie();                                // 0x00663F50
};

class CThingPlayerCreature : public CThingCreatureBase { public:
    void OnDie();
};

void CThingPlayerCreature::OnDie()
{
    CPlayer* player = GetPlayer();
    player->GlobalEventCreatureDied(this);
    if (player->IsMainPlayer())
    {
        player->PlayerRespawnDelay = GetWorldState()->m_value80;
        CTCVillage* village = player->GetPNearestTCVillage();
        if (village)
            village->ProcessCrimeDeeds();
    }
    CThingCreatureBase::OnDie();
}