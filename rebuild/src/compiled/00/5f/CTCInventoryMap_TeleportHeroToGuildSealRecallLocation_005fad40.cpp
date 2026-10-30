// CTCInventoryMap::TeleportHeroToGuildSealRecallLocation() -- while the hero creature is alive, show the mini map on the region load and queue a move-hero-to-region game event (0x2C) carrying the world's guild-seal recall position and angle, via a teleporter and neither forced nor by door, from the main player
// Leaf: TeleportHeroToGuildSealRecallLocation
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FAD40, 336 bytes, `ret`, esp frame (speed TU), /GS (cookie
//   0x0139C8A8, __security_check_cookie @ 0x00BFE9F9).
// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
//   CPlayer::PeekPControlledCreature @ 0x00487DD0 (as the landed sibling TeleportHeroToGuild 0x005FBD90), CTCBase::GetWorld
//   @ 0x00686A70 and PeekWorld @ 0x00686A80, CMemoryBuffer::Copy(unsigned long, const void*) @ 0x009F1760,
//   CPlayerManager::GetMainPlayerNumber @ 0x00449700 (engine 0x013B86A0 +0x1C), CWorld::HandleMoveHeroToRegionGameEvent
//   @ 0x0049EAF0 (landed; it reads X, Y, Z, angle and three flag bytes in this order).
// Layout: retail CWorld (Ego_r.pdb order, 16 bytes less ahead of these members): DisplayMiniMapOnRegionLoad +0x12F,
//   GuildSealRecallPos +0x154, GuildSealRecallAngleXY +0x160; CGameEvent as the landed CInputProcessFishing
//   0x00690B20; retail CThing dead bit +0x91.
class C3DVector { public: float X, Y, Z; };
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char IsDead : 1;                         // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CEngine { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                    // +0x1C
};
extern CEngine* g_pEngine;                            // 0x013B86A0
class CWorld { public:
    unsigned char _pad000[0x12f];
    bool DisplayMiniMapOnRegionLoad;                  // +0x12F
    unsigned char _pad130[0x154 - 0x130];
    C3DVector GuildSealRecallPos;                     // +0x154
    float GuildSealRecallAngleXY;                     // +0x160
    void HandleMoveHeroToRegionGameEvent(const CGameEvent* event);   // 0x0049EAF0
};

class CTCBase { protected:                            // Ego_r.pdb CTCBase protected accessors
    CWorld& GetWorld() const;                         // 0x00686A70
    const CWorld& PeekWorld() const;                  // 0x00686A80
    const CPlayerManager& PeekPlayerManager() const;  // 0x00686D50
};
class CTCInventoryMap : public CTCBase { public:
    const CWorld& GetWorldConst() const { return PeekWorld(); }
    void TeleportHeroToGuildSealRecallLocation();
};

void CTCInventoryMap::TeleportHeroToGuildSealRecallLocation()
{
    const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead) {
        GetWorld().DisplayMiniMapOnRegionLoad = true;
        const C3DVector& pos = GetWorldConst().GuildSealRecallPos;
        float angle = GetWorldConst().GuildSealRecallAngleXY;
        CGameEvent event(0x2c);
        event.AddFloat(pos.X);
        event.AddFloat(pos.Y);
        event.AddFloat(pos.Z);
        event.AddFloat(angle);
        event.AddBool(true);
        event.AddBool(false);
        event.AddBool(false);
        event.Player = (signed char)g_pEngine->PlayerManager->GetMainPlayerNumber();
        GetWorld().HandleMoveHeroToRegionGameEvent(&event);
    }
}