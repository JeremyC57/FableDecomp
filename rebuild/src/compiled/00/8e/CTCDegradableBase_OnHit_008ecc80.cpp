// CTCDegradableBase::OnHit(const C3DVector& position, const C3DVector& direction) -- stamp the hit frame and, when the component was available to hit, re-evaluate the parent's health
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ECC80, length 53 bytes.
// Callees: CTCBase::GetWorld() const @ 0x00686A70 and PeekWorld() const @ 0x00686A80 (both return g->+0x24; manifest label
// "CViewBase::GetDisplayEngine" propagated); the static frame getter @ 0x0049D870; CTCDegradableBase::
// OnParentHealthChanged(bool) @ 0x008ECAC0.
// Name/signature from ghidra_out/egor_signatures.tsv (`?OnHit@CTCDegradableBase@@UAEXABVC3DVector@@0@Z`, virtual:
// slot 30 of the CTCDegradableBarrier vtable 0x0126A6A4).
// Shape: the frame is read through GetWorld (0x00686A70), then IsAvailableToHit() const
// (0x008EC7E0, defined in this TU) and SetAvailabilityToHitFromFrame(frame) (0x008EC800) are inlined: the
// PeekWorld (0x00686A80) + frame getter and the `cmp eax,1`. The LastWorldFrameHit store sits between
// the compare and its `jle` because the store opens BOTH arms of the if/else and is hoisted above the
// branch; a `bool available = IsAvailableToHit(); Set(frame); if (available)` form keeps the inline's
// bl result register instead. Both position arguments are unused (`ret 8`).
class C3DVector;

class CWorld { public:
    static long __fastcall GetFrame();                            // 0x0049D870
};

class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    CWorld& GetWorld() const;                    // 0x00686A70 (Ego_r.pdb CTCBase, protected)
    const CWorld& PeekWorld() const;             // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCDegradableBase : public CTCBase { public:
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    long          LastWorldFrameHit;                        // +0x0c
    void*         PDefinition;                              // +0x10 CDefPointer<const CDegradableDef>
    void*         PCurrentDegradableInfo;                   // +0x14 CDegradableInfo*
    void*         PParentThingPhysical;                     // +0x18 CThingPhysical*
    bool IsAvailableToHit() const;                          // 0x008EC7E0
    void SetAvailabilityToHitFromFrame(long frame) { LastWorldFrameHit = frame; }
    void OnParentHealthChanged(bool fromHit);               // 0x008ECAC0
    void OnHit(const C3DVector& position, const C3DVector& direction);
};

bool CTCDegradableBase::IsAvailableToHit() const
{
    bool available = false;
    if (PeekWorld().GetFrame() - LastWorldFrameHit > 1)
        available = true;
    return available;
}

void CTCDegradableBase::OnHit(const C3DVector& position, const C3DVector& direction)
{
    long frame = GetWorld().GetFrame();
    if (IsAvailableToHit()) {
        SetAvailabilityToHitFromFrame(frame);
        OnParentHealthChanged(false);
    } else {
        SetAvailabilityToHitFromFrame(frame);
    }
}