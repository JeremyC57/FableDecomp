// CTCCarryable::GetInterpolatedPickupOffsetAndOrientationAt(const C3DVector&, const CRightHandedSet&, C3DVector*, CRightHandedSet*, bool) -- where the carrier's hand dummy (right hand, else left hand) would be for the given placement; the offset from that placement is written out when either dummy exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705190, 168 bytes (`ret 14h`, 0x18-byte frame: the dummy position and the difference temporary).
// Callees: CTCGraphicAppearance::GetDummyObjectAssumingObjectAt(const CCharString&, const C3DVector&, const CRightHandedSet&,
// long, unsigned long, C3DVector*, CRightHandedSet*) @ 0x004C0C80 (twice); the hand names are the global CCharString objects
// g_RightHandAttachmentName @ 0x013BAC30 and g_LeftHandAttachmentName @ 0x013BAC34 (names as the landed CTCFireballSpell
// SetWeaponState 0x007C8660).
// Layout: CTCBase ParentThing +0x04; CThing PTCGraphicAppearance +0x64 (retail; PDB +0x74).
// Shape: the dummy flags (0 when the bool is set, else 0x21) are chosen into edi before the first lookup; `||` of the two
// lookups shares the success block -- written as two sequential tests each with its own copy-out (the first returning),
// which VC cross-jumps into one block; the `||` form recolours flags/position (esi/edi) and frames the dummy above the temporary; the difference is a C3DVector temporary built field by field (x first) by an inline
// operator- and copied out through integer registers (no user copy ctor).
class CCharString { public: void* m_pData; };
extern CCharString g_RightHandAttachmentName;           // 0x013BAC30
extern CCharString g_LeftHandAttachmentName;            // 0x013BAC34
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator-(const C3DVector& other) const
    {
        C3DVector r;
        r.X = X - other.X;
        r.Y = Y - other.Y;
        r.Z = Z - other.Z;
        return r;
    }
};
class CRightHandedSet;
struct CTCGraphicAppearance {
    bool GetDummyObjectAssumingObjectAt(const CCharString& name, const C3DVector& position, const CRightHandedSet& orientation,
                                        long lod, unsigned long flags, C3DVector* outPosition, CRightHandedSet* outOrientation);   // 0x004C0C80
};
class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
};
class CTCCarryable { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CTCGraphicAppearance* GetGraphicAppearance() const { return ParentThing->PTCGraphicAppearance; }
    void GetInterpolatedPickupOffsetAndOrientationAt(const C3DVector& position, const CRightHandedSet& orientation,
                                                     C3DVector* outOffset, CRightHandedSet* outOrientation, bool exact);
};

void CTCCarryable::GetInterpolatedPickupOffsetAndOrientationAt(const C3DVector& position, const CRightHandedSet& orientation,
                                                               C3DVector* outOffset, CRightHandedSet* outOrientation, bool exact)
{
    C3DVector dummy;
    unsigned long flags = 0;
    if (!exact)
        flags = 0x21;
    if (GetGraphicAppearance()->GetDummyObjectAssumingObjectAt(g_RightHandAttachmentName, position, orientation, 0, flags, &dummy, outOrientation)) {
        *outOffset = dummy - position;
        return;
    }
    if (GetGraphicAppearance()->GetDummyObjectAssumingObjectAt(g_LeftHandAttachmentName, position, orientation, 0, flags, &dummy, outOrientation))
        *outOffset = dummy - position;
}