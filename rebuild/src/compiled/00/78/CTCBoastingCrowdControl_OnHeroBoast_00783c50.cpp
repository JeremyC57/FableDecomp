// CTCBoastingCrowdControl::OnHeroBoast(long boast) -- when the parent already has a boasting crowd, pass the hero's boast on to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783C50, length 47 bytes (+ int3 padding).
// Callees: CTCBoastingCrowdControl::GetCrowd @ 0x00783A30 (no creation), CTCCreatureGroupBoastingCrowd::
// OnHeroBoast @ 0x00784920. Caller: 0x0056AB4C (the boast UI). The manifest label "OnHeroAttackCrowdMember"
// is propagated; the leaf follows the callee. The boast argument's type is not visible (a 4-byte value).
// Layout = Ego_r.pdb CTCBoastingCrowdControl with the retail 12-byte map: Active +0xc, HeroCrowdThings
// +0x10, CrowdSizesSuppressed +0x1c (PDB +0x20). GetCrowd(thing, &groupThing, &crowd, create) @ 0x00783A30
// (the manifest prototype: CThing*, CThing**, CTCCreatureGroupBoastingCrowd**, bool) looks the parent
// up in HeroCrowdThings and returns the crowd thing and its group component through the out pointers.
// Shape: as 0x00783C20 (the parent read into a local first) with the argument re-read from its slot for
// the call; the local also fixes the eax/ecx choice for the argument and the crowd pointer.
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
    void OnHeroBoast(long boast);
};

void CTCBoastingCrowdControl::OnHeroBoast(long boast)
{
    CThing* parent = ParentThing;
    CThing* groupThing;
    CTCCreatureGroupBoastingCrowd* crowd;
    if (GetCrowd(parent, &groupThing, &crowd, false))
        crowd->OnHeroBoast(boast);
}