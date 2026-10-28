// CTCInventoryMap::TeleportHeroToRegionTeleporter(long) -- if the hero is alive, look up the region's teleporter and, if that is alive too, teleport the hero to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FC840, 87 bytes (+9 int3).
// Callees: player-manager accessor @ 0x00686D50 (`[g_system]+0x30`), main-player accessor @ 0x00449960, the player's
//          character thing @ 0x00487DD0 (CIntelligentPointer::GetPItem on +0x2C), world-map accessor @ 0x00686D20
//          (`[g_system]+0x28`), CWorldMap::GetRegion(long) @ 0x004FC180 (returns CRegion&),
//          CTCInventoryMap::GetRegionTeleporter(CRegion*) @ 0x005FBEA0, CTCInventoryMap::TeleportHeroToTeleporter(CThing*)
//          @ 0x005FBD20.
//
// Shape: two IS_THING_ALIVE tests (`test eax,eax ; je` + `test byte ptr [eax+91h],1 ; jne`, the dead bit, as the landed
// global IsThingDead 0x00836BE0) guarding nested calls; the region index argument is pushed before the world-map
// accessor (right-to-left: the argument of GetRegion, then its object expression). The two system accessors are
// members of the screen that ignore `this` (retail still calls them with ecx = this).
// Local model: no engine headers for CTCInventoryMap / CThing / CWorldMap.
class CRegion;

class CThing { public:
    unsigned char _pad_0x00[0x91];
    unsigned char m_flags;                    // +0x91, bit 0: dead
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;              // 0x00487DD0
};

class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                 // 0x00449960
};

class CWorldMap { public:
    CRegion& GetRegion(long index);           // 0x004FC180
};

class CTCInventoryMap { public:
    CPlayerManager* GetPlayerManager();       // 0x00686D50
    CWorldMap* GetWorldMap();                 // 0x00686D20
    CThing* GetRegionTeleporter(CRegion* region);       // 0x005FBEA0
    void TeleportHeroToTeleporter(CThing* teleporter);  // 0x005FBD20
    void TeleportHeroToRegionTeleporter(long regionIndex);
};

void CTCInventoryMap::TeleportHeroToRegionTeleporter(long regionIndex)
{
    const CThing* hero = GetPlayerManager()->GetMainPlayer()->PeekPControlledCreature();
    if (hero && !(hero->m_flags & 1)) {
        CThing* teleporter = GetRegionTeleporter(&GetWorldMap()->GetRegion(regionIndex));
        if (teleporter && !(teleporter->m_flags & 1))
            TeleportHeroToTeleporter(teleporter);
    }
}