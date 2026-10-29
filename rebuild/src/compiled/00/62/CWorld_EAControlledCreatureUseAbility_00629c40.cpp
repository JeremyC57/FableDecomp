// CWorld::EAControlledCreatureUseAbility(const CGameEvent*) -- game-event action: make the event player's hero use the ability from the event (main player only) and flash the GUI attack event
// Leaf: ?EAControlledCreatureUseAbility@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
// Shape: one CPlayer* local holds both GetPlayer results (the mode test, then the hero lookup), which gives retail's
//   callee-saved assignment (event in ebp, creature in edi).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline unsigned short ReadUShort() { unsigned short value; Read(2, &value); return value; }
    __forceinline float ReadFloat() { float value; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};
class CThingCreatureBase { public:
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void UseAbility(long ability, float charge);   // 0x00665510
public:
    bool IsBlocking() const;   // 0x00664D00
};
class CThingPlayerCreature : public CThingCreatureBase { };
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
class CPlayer { public:
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
class CPlayerManager { public:
    long GetMainPlayerNumber() const;                    // 0x00449700
};
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CPlayerGui { public:
    void OnEventAttack();                          // 0x004379B0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CWorld { public:
    unsigned char       _pad00[0xc];
    CPlayerManager*     PlayerManager;             // +0x0C
    CDefinitionManager* DefinitionManager;         // +0x10
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAControlledCreatureUseAbility(const CGameEvent* event);
};
void CWorld::EAControlledCreatureUseAbility(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(PLAYER_MODE_0x2F))
        return;
    player = GetPlayer(event->Player);
    CThingCreatureBase* creature = player->GetPControlledCreature();
    if (!creature || creature->Dead || creature->IsBlocking())
        return;
    signed char playerNumber = event->Player;
    if (playerNumber != PlayerManager->GetMainPlayerNumber())
        return;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    unsigned short ability = buffer.ReadUShort();
    float charge = buffer.ReadFloat();
    creature->UseAbility(ability, 0.0f);
    unsigned short attack = (unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK");
    if (attack != 0)
        g_pPlayerGui->OnEventAttack();
}