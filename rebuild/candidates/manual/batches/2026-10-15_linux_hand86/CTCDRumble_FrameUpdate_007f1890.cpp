// CTCDRumble::FrameUpdate() -- count the life timer down and kill the parent thing once it has run out
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F1890, length 24 bytes.
// Callees: CThing::Kill(bool) @ 0x004C9B80.
// CTCDRumble vtable 0x012408BC slot 9 (rtti-vtables.tsv; chunk label "CTCDRumble::vfunc_9"); slot 9 of the
// CTCBase-derived vtables is FrameUpdate in 93 classes of the manifest.
// PDB CTCDRumble (0x20 bytes): PDef +0x0c, Strength +0x10, Length +0x14, GlobalRumble +0x18, LifeTimer +0x1c.
// Shape: the OLD timer value is tested (`test eax,eax` before the `lea edx,[eax-1]` store, then `jge`):
// a post-decrement compared with zero. Kill takes a stack argument the caller does not have, so
// the call stays `call ; ret` (no sibling jump).
struct CThing {
    void Kill(bool immediately);                            // 0x004C9B80
};

struct CTCDRumble {
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    void*         PDef;                                     // +0x0c CDefPointer<const CRumbleDef>
    float         Strength;                                 // +0x10
    float         Length;                                   // +0x14
    bool          GlobalRumble;                             // +0x18
    long          LifeTimer;                                // +0x1c
    void FrameUpdate();
};

void CTCDRumble::FrameUpdate()
{
    if (LifeTimer-- < 0)
        ParentThing->Kill(true);
}
