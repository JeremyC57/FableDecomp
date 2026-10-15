// CTCBoastingCrowdControl::vfunc_1(CPersistContext& context, bool transfer) -- when asked, persist CrowdSizesSuppressed with a false default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783210, length 39 bytes (+ int3 padding).
// Callees: CPersistContext::Transfer<bool>(name, value, default) @ 0x004045C0 (manifest
// "Transfer<signed char>": a one-byte transfer). String 0x0126B38C = "CrowdSizesSuppressed".
// Identity: slot 1 of vtable 0x0126B3AC = CTCBoastingCrowdControl (the virtual transfer hook that
// CTCBase::Serialise 0x00686AA0 forwards to with its two arguments).
// Layout = Ego_r.pdb CTCBoastingCrowdControl with the retail 12-byte map: Active +0xc, HeroCrowdThings
// +0x10, CrowdSizesSuppressed +0x1c (PDB +0x20). GetCrowd(thing, &groupThing, &crowd, create) @ 0x00783A30
// (the manifest prototype: CThing*, CThing**, CTCCreatureGroupBoastingCrowd**, bool) looks the parent
// up in HeroCrowdThings and returns the crowd thing and its group component through the out pointers.
// Shape: the literal false default is a one-byte temporary that VC homes in the dead argument slot of
// the already-tested flag (`mov byte ptr [esp+14h],0`, `lea eax,[esp+8]`).
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

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
    void vfunc_1(CPersistContext& context, bool transfer);
};

void CTCBoastingCrowdControl::vfunc_1(CPersistContext& context, bool transfer)
{
    if (transfer)
        context.Transfer("CrowdSizesSuppressed", CrowdSizesSuppressed, false);
}
