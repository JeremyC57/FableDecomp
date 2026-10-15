// CTCDegradableBase::IsAvailableToHit() const -- true once more than one world frame has passed since the last hit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC7E0, length 31 bytes.
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (returns g->+0x24; its non-const twin is 0x00686A70; the manifest
// label "CViewBase::GetDisplayEngine" is propagated); the static frame getter @ 0x0049D870 (returns the global
// @ 0x013B89BC; manifest label "GFGetPreMainMemoryUsed" is propagated).
// Name and constness from ghidra_out/egor_signatures.tsv (`?IsAvailableToHit@CTCDegradableBase@@IBE_NXZ`).
// PDB CTCDegradableBase (0x1c bytes): LastWorldFrameHit +0x0c, PDefinition +0x10, PCurrentDegradableInfo +0x14,
// PParentThingPhysical +0x18.
// Shape: the static frame getter is called through the world pointer (`GetWorld()->GetWorldFrame()`), so
// GetWorld runs for nothing and no `mov ecx,eax` follows it; the false result is preloaded in bl before
// the calls and the true arm is `mov al,1 ; jg` -- a result local initialised to false and set in the if.
struct CWorld {
    static long GetWorldFrame();                            // 0x0049D870
};

struct CTCDegradableBase {
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    long          LastWorldFrameHit;                        // +0x0c
    void*         PDefinition;                              // +0x10 CDefPointer<const CDegradableDef>
    void*         PCurrentDegradableInfo;                   // +0x14 CDegradableInfo*
    void*         PParentThingPhysical;                     // +0x18 CThingPhysical*
    const CWorld* GetWorld() const;                         // 0x00686A80 (CTCBase)
    bool IsAvailableToHit() const;
};

bool CTCDegradableBase::IsAvailableToHit() const
{
    bool available = false;
    if (GetWorld()->GetWorldFrame() - LastWorldFrameHit > 1)
        available = true;
    return available;
}