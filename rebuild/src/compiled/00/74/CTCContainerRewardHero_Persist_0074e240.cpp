// CTCContainerRewardHero::Persist(CPersistContext*, bool) -- when the flag is set the "RewardEnabled" flag (default true) is persisted, then CTCContainer's hook persists the contents
// Leaf: Persist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E240, length 58 bytes (+ int3 padding). Vtable 0x01269424
// (CTCContainerRewardHero) slot 1, the TC persist hook; the manifest calls it OnSerialise.
// Callees: CPersistContext::Transfer<bool>(name, value, default) @ 0x004045C0 (folded with
// Transfer<signed char>), CTCContainer's slot-1 persist @ 0x008ED2B0 (called non-virtually).
// Layout: Enabled +0x18 (Ego_r.pdb +0x1c less the 4 bytes of the retail CTCContainer vector).
// Shape: the flag is held in ebx across the conditional transfer; the `true` default is a one-byte
// temporary bound to the const reference and homed in the flag's own argument slot
// (`mov byte ptr [esp+20h],1`) although the flag is still live in ebx; the base hook comes last.
class CPersistContext { public:
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCContainer_Calls {                                 // retail 0x18 bytes
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);   // slot 1 (0x008ED2B0)
    unsigned char _pad04[0x14];                             // +0x04 CTCBase, ContainerItems
};

class CTCContainerRewardHero : public CTCContainer_Calls { public:        // vtable 0x01269424
    bool Enabled;                                           // +0x18
    bool CreateObjectsThatTimeOut;                          // +0x19
    virtual void Persist(CPersistContext* context, bool flag);
};

void CTCContainerRewardHero::Persist(CPersistContext* context, bool flag)
{
    if (flag)
        context->Transfer("RewardEnabled", Enabled, true);
    CTCContainer_Calls::Persist(context, flag);
}