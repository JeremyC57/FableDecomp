// CTCTargetingBase::GetTargetedPos(C3DVector& pos, float radius) const -- when the targeted thing is alive, its fire point (for the given radius) into pos and true; else false
// Leaf: GetTargetedPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5950, length 108 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on PTargetedThing), CTCBase::GetWorld @ 0x00686A80,
// NTargeting::GetTargetingPositionOnThing(const CWorld&, const CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx
// because C3DVector is non-POD, world in edx, `ret 8`; as the landed 0x008798C0; manifest "Creature::GetTargetFirePoint").
// Layout (Ego_r.pdb CTCTargetingBase = retail): PTargetedThing (CIntelligentPointer<CThing>) at the odd offset +0x25 (`#pragma pack(1)`).
// Shape: PTargetedThing is a reference local (`lea esi,[edi+25h]`) with an inline IsThingAlive; the helper result is a frame temporary
// copied into pos with dword moves (implicit C3DVector operator=; the user default ctor makes the
// type non-POD, which moves the hidden result pointer into ecx).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CWorld;
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}
#pragma pack(push, 1)
struct CTCTargetingBase {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    bool ScriptOverrideIgnoreEverything;        // +0x0c
    CBaseIntelligentPointer PNearestInterestingThing;           // +0x0d
    CBaseIntelligentPointer PScriptOverrideInterestingThing;    // +0x15
    CBaseIntelligentPointer PScriptOverrideRangedTarget;        // +0x1d
    CBaseIntelligentPointer PTargetedThing;                     // +0x25
    const CWorld& GetWorld() const;             // 0x00686A80 (CTCBase)
    bool GetTargetedPos(C3DVector& pos, float radius) const;
};
#pragma pack(pop)

static inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

bool CTCTargetingBase::GetTargetedPos(C3DVector& pos, float radius) const
{
    const CBaseIntelligentPointer& target = PTargetedThing;
    if (IsThingAlive(target)) {
        pos = NTargeting::GetTargetingPositionOnThing(GetWorld(), *target.GetPItem(), radius);
        return true;
    }
    return false;
}