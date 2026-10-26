// CWorld::EAChargeQuickAccessItem(const CGameEvent*) -- game-event action: start or stop charging a quick-access item for the event player's hero and switch the player's charging mode with it
// Leaf: EAChargeQuickAccessItem
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00629B10, 298 bytes (304 with padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?EAChargeQuickAccessItem@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer(long) 0x0049D850 (x2), CPlayer::HasMode 0x00634160, CPlayer::GetHeroCreature 0x00487DC0,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0,
//   CPlayerGui::ToggleQuickAccessCharge(long, bool) 0x00437A10 and IsOpen() const 0x004374F0 on g_pPlayerGui 0x013B8790,
//   CPlayer::GetCurrentMode 0x00633BE0, AddMode(EPlayerMode, bool) 0x006345C0, RemoveMode 0x00634190,
//   __security_check_cookie 0x00BFE9F9.
// Shape: the first player is a named local (GetPlayer before the 0x2F push); the hero gate is one `||` early return
//   (CThing dead bit +0x91 bit 0); the payload is the 44-byte CMemoryBuffer idiom; the charge flag comes from an inline
//   by-value ReadChar() so its byte and the normalised bool (`!= 0`) share the dead `event` argument slot, the bool
//   stays in bl (ebx pushed only on that path) for the mode switch.
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline char ReadChar() { char value; Read(1, &value); return value; }
};
class CGameEvent { public:
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
    bool IsPerformingAnyAction();                  // 0x006622A0
};
enum EPlayerMode { PLAYER_MODE_CHARGING_QUICK_ACCESS = 0x1c, PLAYER_MODE_0x2F = 0x2f };
class CPlayer { public:
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
    EPlayerMode GetCurrentMode() const;            // 0x00633BE0
    void AddMode(EPlayerMode mode, bool b);        // 0x006345C0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};
class CPlayerGui { public:
    void ToggleQuickAccessCharge(long item, bool charge);   // 0x00437A10
    bool IsOpen() const;                           // 0x004374F0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAChargeQuickAccessItem(const CGameEvent* event);
};
void CWorld::EAChargeQuickAccessItem(const CGameEvent* event)
{
    CPlayer* eventPlayer = GetPlayer(event->Player);
    if (eventPlayer->HasMode(PLAYER_MODE_0x2F))
        return;
    CPlayer* player = GetPlayer(event->Player);
    CThingCreatureBase* hero = player->GetHeroCreature();
    if (!hero || hero->Dead || hero->IsPerformingAnyAction())
        return;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long item;
    buffer.Read(4, &item);
    bool charge = buffer.ReadChar() != 0;
    g_pPlayerGui->ToggleQuickAccessCharge(item, charge);
    if (g_pPlayerGui->IsOpen()) {
        EPlayerMode mode = player->GetCurrentMode();
        if (charge) {
            if (mode != PLAYER_MODE_CHARGING_QUICK_ACCESS)
                player->AddMode(PLAYER_MODE_CHARGING_QUICK_ACCESS, false);
        } else if (mode == PLAYER_MODE_CHARGING_QUICK_ACCESS)
            player->RemoveMode(mode);
    }
}