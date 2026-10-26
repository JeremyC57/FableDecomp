// CTCCreatureItemLevitationNymph::UpdateLevitationTarget() -- while the hover target is alive, re-aim the hover direction from the target towards the nymph (XY plane, normalised)
// Leaf: UpdateLevitationTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F84C0, length 116 bytes (+ int3 padding). Vtable 0x012415F4 slot 31 (catalog
// "vfunc_31"; CTCCreatureItemLevitationBase::FrameUpdate 0x007F8300 calls this slot first every frame).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::GetPosition @ 0x004C73D0 (parent, then target),
// C3DVector::AccurateNormalise @ 0x00A14440.
// Shape: inline IS_THING_ALIVE, then the same C3DVector(dx, dy, 0) temporary as SetTarget; Normalise is void and the
// C2DVector member is filled by an inline Set(x, y) (a C2DVector temporary adds a frame copy).
// Layout: Ego_r.pdb CTCCreatureItemLevitationNymph (retail 8 bytes lower: the base list is one pointer, PDB 12):
// NymphHoverXYDirectionFromTarget +0x18 (C2DVector), NymphHoverHeightOffset +0x20, NymphHoverXYMagnitude +0x24,
// PTarget +0x28 (CIntelligentPointer<const CThing>, vtable 0x01237F1C). Local polymorphic model.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& AccurateNormalise();                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
struct C2DVector {
    float X, Y;
    void Set(float x, float y) { X = x; Y = y; }
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing() {}
    CIntelligentPointerConstThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
struct CTCCreatureItemLevitationNymph {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x18 - 0x08];          // CTCBase tail, LevitatingItems, NumItems, LastAddFrame
    C2DVector NymphHoverXYDirectionFromTarget;  // +0x18
    float NymphHoverHeightOffset;               // +0x20
    float NymphHoverXYMagnitude;                // +0x24
    CIntelligentPointerConstThing PTarget;      // +0x28
    void SetTarget(CThing* target, float xyMagnitude, float heightOffset);
    void UpdateLevitationTarget();              // vtable 0x012415F4 slot 31
};

void CTCCreatureItemLevitationNymph::UpdateLevitationTarget()
{
    if (IS_THING_ALIVE(PTarget)) {
        const C3DVector& position = ParentThing->GetPosition();
        const C3DVector& targetPosition = PTarget.GetPItem()->GetPosition();
        C3DVector direction(position.X - targetPosition.X, position.Y - targetPosition.Y, 0.0f);
        direction.AccurateNormalise();
        NymphHoverXYDirectionFromTarget.Set(direction.X, direction.Y);
    }
}