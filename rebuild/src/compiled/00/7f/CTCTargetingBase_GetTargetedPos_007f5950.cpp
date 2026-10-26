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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CWorld;
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}
#pragma pack(push, 1)
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCTargetingBase : public CTCBase { public:
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    bool ScriptOverrideIgnoreEverything;        // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;           // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;    // +0x15
    CIntelligentPointer<CThing> PScriptOverrideRangedTarget;        // +0x1d
    CIntelligentPointer<CThing> PTargetedThing;                     // +0x25
    bool GetTargetedPos(C3DVector& pos, float radius) const;
};
#pragma pack(pop)

static inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

bool CTCTargetingBase::GetTargetedPos(C3DVector& pos, float radius) const
{
    const CIntelligentPointer<CThing>& target = PTargetedThing;
    if (IsThingAlive(target)) {
        pos = NTargeting::GetTargetingPositionOnThing(PeekWorld(), *target.GetPItem(), radius);
        return true;
    }
    return false;
}