// CJackOfBladesBattleDef::Transfer(CPersistContext& context) -- persist the Jack of Blades boss-battle tuning: one float, the long hit count, then fifteen floats
// Leaf: ?Transfer@CJackOfBladesBattleDef@@UAEXAAVCPersistContext@@@Z
// Pragma: `#pragma optimize("s", on)` -- a size TU: every member address goes through eax.
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E39E6, 200 bytes, `ret 4`.
// Callees: the persist context's typed transfer helpers @ 0x00431061 (TransferFloat) and @ 0x00431102 (TransferLong);
//   both manifest labels ("CEngineLightingManager::UpdateShadowScene") are propagated -- the corrected names come from
//   ghidra_out/labels_identity_corrections_2026-10-19.tsv.
// Layout: Ego_r.pdb CJackOfBladesBattleDef (0x84 bytes) with every member 4 bytes lower in retail (the def base is
//   0x3C bytes in retail, PDB 0x40): IdealShootDistance +0x3c, NumGetHitsBeforeRush +0x40 (the only long, hence the
//   one TransferLong call), TimeBetweenShots +0x44 .. RushAwayDistance +0x7c. No engine header exists for the class,
//   so the def base is a vptr + pad model.
// Shape: each member goes by address in declaration order; the last member's address is formed in place
//   (`add esi,7Ch ; push esi`), the natural `&RushAwayDistance` of the final call.
class CPersistContext { public:
    template <class T> void Transfer(T& value);              // 0x00431102
};

class CJackOfBladesBattleDef { public:
    virtual void v00();
    unsigned char _defBase[0x38];                // CDefClassBase / CParentDefClassBase (retail 0x3c bytes incl. vptr)
    float IdealShootDistance;                    // +0x3c
    long NumGetHitsBeforeRush;                   // +0x40
    float TimeBetweenShots;                      // +0x44
    float ScreamInAirTime;                       // +0x48
    float TimeBetweenPillars;                    // +0x4c
    float BattleFloatHeight;                     // +0x50
    float BattleScale;                           // +0x54
    float ScaleUpTime;                           // +0x58
    float ScaleDownTime;                         // +0x5c
    float DeathScreamTime;                       // +0x60
    float DeathHeight;                           // +0x64
    float BurnDamage;                            // +0x68
    float FlashDuration;                         // +0x6c
    float FlashBuildUpTime;                      // +0x70
    float RushTriggerDistance;                   // +0x74
    float RushToDistance;                        // +0x78
    float RushAwayDistance;                      // +0x7c
    virtual void Transfer(CPersistContext& context);
};

void CJackOfBladesBattleDef::Transfer(CPersistContext& context)
{
    context.Transfer(*(float*)(&IdealShootDistance));
    context.Transfer(*(long*)(&NumGetHitsBeforeRush));
    context.Transfer(*(float*)(&TimeBetweenShots));
    context.Transfer(*(float*)(&ScreamInAirTime));
    context.Transfer(*(float*)(&TimeBetweenPillars));
    context.Transfer(*(float*)(&BattleFloatHeight));
    context.Transfer(*(float*)(&BattleScale));
    context.Transfer(*(float*)(&ScaleUpTime));
    context.Transfer(*(float*)(&ScaleDownTime));
    context.Transfer(*(float*)(&DeathScreamTime));
    context.Transfer(*(float*)(&DeathHeight));
    context.Transfer(*(float*)(&BurnDamage));
    context.Transfer(*(float*)(&FlashDuration));
    context.Transfer(*(float*)(&FlashBuildUpTime));
    context.Transfer(*(float*)(&RushTriggerDistance));
    context.Transfer(*(float*)(&RushToDistance));
    context.Transfer(*(float*)(&RushAwayDistance));
}