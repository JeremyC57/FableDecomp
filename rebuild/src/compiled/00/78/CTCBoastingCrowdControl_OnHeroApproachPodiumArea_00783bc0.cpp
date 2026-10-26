// CTCBoastingCrowdControl::OnHeroApproachPodiumArea() -- fetch (creating if needed) the parent's boasting crowd and tell it the hero is approaching
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783BC0, length 36 bytes (+ int3 padding).
// Callees: CTCBoastingCrowdControl::GetCrowd @ 0x00783A30 (result ignored), CTCCreatureGroupBoastingCrowd::
// OnHeroApproachBoastingArea @ 0x00784600. Caller: 0x00782A71 (CTCBoastingArea).
// Layout = Ego_r.pdb CTCBoastingCrowdControl with the retail 12-byte map: Active +0xc, HeroCrowdThings
// +0x10, CrowdSizesSuppressed +0x1c (PDB +0x20). GetCrowd(thing, &groupThing, &crowd, create) @ 0x00783A30
// (the manifest prototype: CThing*, CThing**, CTCCreatureGroupBoastingCrowd**, bool) looks the parent
// up in HeroCrowdThings and returns the crowd thing and its group component through the out pointers.
// Shape: two out-pointer locals (crowd at [esp], the group thing at [esp+4]); the create flag is
// pushed first; the crowd pointer is used without a null test.
class CThing;

class CTCCreatureGroupBoastingCrowd { public:
    void OnHeroApproachBoastingArea();                      // 0x00784600
    void OnHeroEnterBoastingArea();                         // 0x00784690
    void OnHeroLeaveBoastingArea();                         // 0x007846B0
    void OnHeroBoast(long boast);                           // 0x00784920
    void OnHeroAttackCrowdMember();                         // 0x007849C0
};

class CTCBoastingCrowdControl { public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    bool          Active;                                   // +0x0c
    unsigned char _pad0d[0x3];
    unsigned char HeroCrowdThings[0xc];                     // +0x10 map<unsigned __int64, CIntelligentPointer<CThing>> (12 bytes in retail)
    bool          CrowdSizesSuppressed;                     // +0x1c (PDB +0x20)
    bool GetCrowd(CThing* thing, CThing** groupThing, CTCCreatureGroupBoastingCrowd** crowd, bool create);   // 0x00783A30
    void OnHeroApproachPodiumArea();
};

void CTCBoastingCrowdControl::OnHeroApproachPodiumArea()
{
    CThing* groupThing;
    CTCCreatureGroupBoastingCrowd* crowd;
    GetCrowd(ParentThing, &groupThing, &crowd, true);
    crowd->OnHeroApproachBoastingArea();
}