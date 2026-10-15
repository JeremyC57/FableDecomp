// CTCChest::Persist(CPersistContext*, bool) -- the container contents persisted through CTCContainer's hook, then the "ChestOpen" flag (default false)
// Leaf: Persist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750010, length 50 bytes (+ int3 padding). Vtable 0x0126957C
// (CTCChest) slot 1, the TC persist hook.
// Callees: CTCContainer's slot-1 persist @ 0x008ED2B0 (called non-virtually: CTCChest skips
// CTCContainerRewardHero's own hook 0x0074E240; the manifest names it "Persist_TransferContainerContents"),
// CPersistContext::Transfer<bool>(name, value, default) @ 0x004045C0 (folded with Transfer<signed char>).
// Layout: ChestOpen +0x30 (Ego_r.pdb +0x34 less the 4 bytes of the retail CTCContainer vector).
// Shape: the base hook first (both arguments forwarded), then the transfer; its `false` default is a
// one-byte temporary bound to the const reference and homed in the dead second-argument slot
// (`mov byte ptr [esp+1Ch],0`), the member address formed by advancing `this` (`add esi,30h`).
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCContainer_Calls {                                 // retail 0x18 bytes
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);   // slot 1 (0x008ED2B0)
    unsigned char _pad04[0x14];                             // +0x04 CTCBase, ContainerItems
};

struct CTCContainerRewardHero_Calls : CTCContainer_Calls {  // retail 0x1c bytes
    bool          Enabled;                                  // +0x18
    bool          CreateObjectsThatTimeOut;                 // +0x19
    unsigned char _pad1a[2];
};

struct CTCChest : CTCContainerRewardHero_Calls {            // vtable 0x0126957C
    unsigned char ChestOpener[0x14];                        // +0x1c COpenCloseHelper
    bool          ChestOpen;                                // +0x30
    virtual void Persist(CPersistContext* context, bool flag);
};

void CTCChest::Persist(CPersistContext* context, bool flag)
{
    CTCContainer_Calls::Persist(context, flag);
    context->Transfer("ChestOpen", ChestOpen, false);
}