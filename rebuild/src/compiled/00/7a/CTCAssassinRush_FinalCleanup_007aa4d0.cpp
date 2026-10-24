// CTCAssassinRush::FinalCleanup() -- when the parent thing is alive: release its special-abilities lock if ability 13 (assassin rush) is running and, unless the exit modes already ran, resume the player's animation (player-controlled parent) or end this TC's bullet-time slowdown; then kill both trail effects that are still alive
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AA4D0, 231 bytes (+9 int3), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface, type 0xA7),
//   CThing::SetSuspendAnimation 0x00833A10, CTCBase::GetWorld 0x00686A70, CBulletTimeManager::RemoveBulletTimeSlowdown
//   0x004C6140 (world +0x68), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80.
// Naming: Ego_r.pdb ?FinalCleanup@CTCAssassinRush@@AAEXXZ; the only references are the two vtable thunks
//   0x007AA820 (slot 14) and 0x007AA830 (slot 2) of CTCAssassinRush (vtable 0x0126CF2C) that tail-jump here
//   (OnKill / OnRemove forwarding to the private FinalCleanup). Catalog label "CTCInventory_OnDeactivate" is propagated.
// Layout: as the landed FrameUpdate 0x007AB190: ParentThing +4, AssassinTrail +0x18, AssassinDisplacementTrail +0x20,
//   CompletedExitModes +0xa4; CThing state byte +0x10 (1 = player), bits +0x20, interface map +0x44, dead bit +0x91.
// Shape: the parent is read from the member at each use (no local: eax for the alive test and the lookup, ecx for the
//   state test and the call); the constant 1 lives in ebx (dead-bit tests, state compare, Kill(true)) only when the
//   three alive tests go through an inline `bool IsDead() const { return (m_stateFlags & 1) != 0; }` (the raw
//   `!(flags & 1)` test leaves immediates, 230 v 231); the two ability clears are immediate stores.
// Leaf: ?FinalCleanup@CTCAssassinRush
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char State;                                // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_stateFlags & 1) != 0; }
    void Kill(bool immediately);                        // 0x004C9B80
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
struct CTCSpecialAbilities {                            // interface 0xA7
    unsigned char _pad00[0xc];
    bool          AbilityMutexLock;                     // +0x0c
    long          CurrentAbility;                       // +0x10 (13 = assassin rush)
};
struct CBulletTimeManager {
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
struct CWorld {
    unsigned char _pad00[0x68];
    CBulletTimeManager* PBulletTimeManager;             // +0x68
    void RemoveBulletTimeSlowdown(unsigned long key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
struct CTCAssassinRush {
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x18 - 0x08];
    CBaseIntelligentPointer AssassinTrail;              // +0x18
    CBaseIntelligentPointer AssassinDisplacementTrail;  // +0x20
    unsigned char           _pad28[0xa4 - 0x28];
    bool                    CompletedExitModes;         // +0xa4
    CWorld* GetWorld();                                 // 0x00686A70
    void FinalCleanup();
};

void CTCAssassinRush::FinalCleanup()
{
    if (IS_THING_ALIVE(ParentThing)) {
        CTCSpecialAbilities* abilities;
        if (ParentThing->GetInterface(&abilities, 0xa7)) {
            if (abilities->CurrentAbility == 13) {
                abilities->AbilityMutexLock = false;
                abilities->CurrentAbility = 0;
            }
        }
        if (!CompletedExitModes) {
            if (ParentThing->State == 1)
                ParentThing->SetSuspendAnimation(false, 0);
            else
                GetWorld()->RemoveBulletTimeSlowdown((unsigned long)this);
        }
    }
    if (IS_THING_ALIVE(AssassinTrail))
        AssassinTrail.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(AssassinDisplacementTrail))
        AssassinDisplacementTrail.GetPItem()->Kill(true);
}