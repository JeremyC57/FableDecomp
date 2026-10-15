// CTCBase::Serialise(CPersistContext* context, long arg) -- in the save modes (1 and 3) transfer only when the component asks to be saved (virtual slot 23); then forward to the virtual transfer hook (slot 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686AA0, length 50 bytes (+ int3 padding).
// Callees: virtual slot 23 (+0x5c; CTCBase's own implementation @ 0x00686970 returns false) and
// virtual slot 1 (+0x04; CTCBase's own implementation @ 0x00686960 is an empty `ret 8`), both on this.
// Callers: the thing persist code @ 0x004C8782 / 0x004CA86E (a non-virtual wrapper).
// Layout: CPersistContext (Ego_r.pdb, 44 bytes) Mode at +0x18 (EMode). engine/CTCBase.h holds the
// layout only (its vptr is a plain word), so the virtual calls go through a local polymorphic model
// `CTCBase_Calls` (as the landed 0x007AEE10 / 0x007B2720); slot names are descriptive.
// Shape: `mode == 1 || mode == 3` with the mode loaded once; the slot-23 test and the forward share
// the exit, the `&&` folds into one `if` around the forward.
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                     // +0x18 EMode
};

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    virtual void Transfer(CPersistContext* context, long arg);   // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22();
    virtual bool ShouldBeSaved();                           // slot 23 (+0x5c)
    void*         ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    void Serialise(CPersistContext* context, long arg);
};

void CTCBase_Calls::Serialise(CPersistContext* context, long arg)
{
    long mode = context->Mode;
    if ((mode == 1 || mode == 3) && !ShouldBeSaved())
        return;
    Transfer(context, arg);
}
