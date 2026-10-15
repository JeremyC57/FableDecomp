// CTCBoastingCrowdControl::vfunc_16(bool flag) -- with the flag set, release every crowd thing; otherwise start the crowd control up unless it is already active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783A10, length 31 bytes (+ int3 padding).
// Callees: the crowd-thing teardown @ 0x007832B0 (walks HeroCrowdThings; manifest "LTextTree_KillAllThings"),
// CTCBoastingCrowdControl::Startup @ 0x00783850.
// Identity: slot 16 of vtable 0x0126B3AC = CTCBoastingCrowdControl (rtti-vtables.tsv; CTCBase's own slot 16
// @ 0x00686910 is an empty `ret 4`, and CTCAvoidRegionExit's override also takes a bool).
// Layout = Ego_r.pdb CTCBoastingCrowdControl with the retail 12-byte map: Active +0xc, HeroCrowdThings
// +0x10, CrowdSizesSuppressed +0x1c (PDB +0x20). GetCrowd(thing, &groupThing, &crowd, create) @ 0x00783A30
// (the manifest prototype: CThing*, CThing**, CTCCreatureGroupBoastingCrowd**, bool) looks the parent
// up in HeroCrowdThings and returns the crowd thing and its group component through the out pointers.
// Shape: two exits, each with its own `ret 4`; the calls keep this in ecx.
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
    void KillAllCrowdThings();                              // 0x007832B0
    void Startup();                                         // 0x00783850
    void vfunc_16(bool flag);
};

void CTCBoastingCrowdControl::vfunc_16(bool flag)
{
    if (flag)
        KillAllCrowdThings();
    else if (!Active)
        Startup();
}