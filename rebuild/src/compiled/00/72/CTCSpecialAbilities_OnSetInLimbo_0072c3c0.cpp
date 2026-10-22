// CTCSpecialAbilities::OnSetInLimbo(bool on) -- when switched on, kill every reclaimed undead follower whose thing is still alive and conscious
// Leaf: vfunc_16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C3C0, length 98 bytes (`ret 4`), CTCSpecialAbilities vtable 0x0126683C slot 16
// (a CTCBase bool notification; the catalog has no name for it).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times, once per use), CThing 0x0085EE40 (no-argument bool on the thing;
// manifest label "CThingFilter_IsUnconscious::operator()" is a byte twin, read here as CThing::IsUnconscious),
// CTCSpecialAbilities::KillReclaimedUndead(CThing*) @ 0x0072B3B0 (manifest "CCreatureAction_KillReclaimedUndead").
// Layout: PDB CTCSpecialAbilities: UndeadFollowers +0x14 (retail STLport list = one sentinel pointer); CUndeadFollower {long; CIntelligentPointer
// PThing} so PThing sits at node+0xc. CThing dead flag +0x91 bit 0.
// Shape: the intelligent pointer is a reference local (`lea esi,[edi+0Ch]` once per node) and every use calls GetPItem again; the loop
// compares the node with the sentinel re-read from `this`.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsUnconscious();                       // 0x0085EE40
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CUndeadFollower {                        // CTCSpecialAbilities::CUndeadFollower, 12 bytes
    long                    m_value;            // +0x00
    CBaseIntelligentPointer PThing;             // +0x04
};
struct CUndeadFollowerList {                    // retail std::list<CUndeadFollower>: one sentinel pointer
    struct node {
        node*           next;
        node*           prev;
        CUndeadFollower value;                  // +0x08
    };
    node* head;
};
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    CUndeadFollowerList UndeadFollowers;        // +0x14
    CThing* Creature;                           // +0x18 (PDB +0x20)
    void KillReclaimedUndead(CThing* thing);    // 0x0072B3B0
    void OnSetInLimbo(bool on);
};

void CTCSpecialAbilities::OnSetInLimbo(bool on)
{
    if (on) {
        for (CUndeadFollowerList::node* n = UndeadFollowers.head->next; n != UndeadFollowers.head; n = n->next) {
            CBaseIntelligentPointer& thing = n->value.PThing;
            if (thing.GetPItem() && !(thing.GetPItem()->m_flags91 & 1) && !thing.GetPItem()->IsUnconscious())
                KillReclaimedUndead(thing.GetPItem());
        }
    }
}