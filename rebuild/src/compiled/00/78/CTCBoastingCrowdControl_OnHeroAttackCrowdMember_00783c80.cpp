// CTCBoastingCrowdControl::OnHeroAttackCrowdMember() -- tell an existing boasting crowd that the hero attacked one of its members, then suppress the crowd sizes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783C80, length 51 bytes (+ int3 padding).
// Callees: CTCBoastingCrowdControl::GetCrowd @ 0x00783A30 (no creation), CTCCreatureGroupBoastingCrowd::
// OnHeroAttackCrowdMember @ 0x007849C0. Caller: 0x00683170.
// Layout = Ego_r.pdb CTCBoastingCrowdControl with the retail 12-byte map: Active +0xc, HeroCrowdThings
// +0x10, CrowdSizesSuppressed +0x1c (PDB +0x20). GetCrowd(thing, &groupThing, &crowd, create) @ 0x00783A30
// (the manifest prototype: CThing*, CThing**, CTCCreatureGroupBoastingCrowd**, bool) looks the parent
// up in HeroCrowdThings and returns the crowd thing and its group component through the out pointers.
// Shape: this is kept in esi for the trailing CrowdSizesSuppressed store; the parent pointer is read into
// a local first (before `push 0`).
struct CThing;

struct CTCCreatureGroupBoastingCrowd {
    void OnHeroApproachBoastingArea();                      // 0x00784600
    void OnHeroEnterBoastingArea();                         // 0x00784690
    void OnHeroLeaveBoastingArea();                         // 0x007846B0
    void OnHeroBoast(long boast);                           // 0x00784920
    void OnHeroAttackCrowdMember();                         // 0x007849C0
};

struct CTCBoastingCrowdControl {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    bool          Active;                                   // +0x0c
    unsigned char _pad0d[0x3];
    unsigned char HeroCrowdThings[0xc];                     // +0x10 map<unsigned __int64, CIntelligentPointer<CThing>> (12 bytes in retail)
    bool          CrowdSizesSuppressed;                     // +0x1c (PDB +0x20)
    bool GetCrowd(CThing* thing, CThing** groupThing, CTCCreatureGroupBoastingCrowd** crowd, bool create);   // 0x00783A30
    void OnHeroAttackCrowdMember();
};

void CTCBoastingCrowdControl::OnHeroAttackCrowdMember()
{
    CThing* parent = ParentThing;
    CThing* groupThing;
    CTCCreatureGroupBoastingCrowd* crowd;
    if (GetCrowd(parent, &groupThing, &crowd, false))
        crowd->OnHeroAttackCrowdMember();
    CrowdSizesSuppressed = true;
}