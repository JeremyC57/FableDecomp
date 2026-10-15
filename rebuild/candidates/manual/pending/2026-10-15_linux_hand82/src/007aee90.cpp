// CTCBoss::IsReadyForPhaseChange() const -- true when phase changes are enabled, the parent is a physical thing and its Health has dropped below the current phase's threshold
//
// Retail 0x007AEE90, 47 bytes. Callee: CTCBoss::GetMinHealthForPhase(long) const @ 0x007AEE40
// (float in st0). Layout: EnablePhaseChanges +0x21, currentBossPhase +0x18, ParentThing +0x04;
// the physical-thing type flag is ThingTypeFlags (+0x6c) bit 2 and Health is CThingPhysical +0xb4
// (PDB +0xcc; the retail CThing sits 0x10 lower and CThingPhysical a further 8 bytes lower).
// Shape: the parent thing is a named local (it survives the call in esi); `fcomp [Health] ;
// test ah,41h ; jne` is `threshold > Health` with the call result on the left; two separate exits
// (`mov al,1` / the shared `xor al,al`) come from the `if (...) return true; return false;` form.

struct CThing
{
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                   // +0x6c
};

struct CThingPhysical : CThing
{
    unsigned char _pad_0x70[0x40];
    float         MaxHealth;                        // +0xb0
    float         Health;                           // +0xb4
};

enum { THING_TYPE_PHYSICAL = 4 };

#pragma pack(push, 1)
struct CTCBoss
{
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PBossDef;                         // +0x0c CDefPointer<const CBossDef>
    long          GenericCounter;                   // +0x10
    long          StrikesSinceHit;                  // +0x14
    long          currentBossPhase;                 // +0x18
    long          NextWeapon;                       // +0x1c
    bool          BCountStrikes;                    // +0x20
    bool          EnablePhaseChanges;               // +0x21

    float GetMinHealthForPhase(long phase) const;   // 0x007AEE40
    bool  IsReadyForPhaseChange() const;
};
#pragma pack(pop)

bool CTCBoss::IsReadyForPhaseChange() const
{
    if (EnablePhaseChanges)
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & THING_TYPE_PHYSICAL)
        {
            if (GetMinHealthForPhase(currentBossPhase) > static_cast<CThingPhysical*>(thing)->Health)
                return true;
        }
    }
    return false;
}
