// AttackParticleAndEndPoints::AttackParticleAndEndPoints(const AttackParticleAndEndPoints&) -- copy constructor of an object-augmentation attack particle record: two dummy names, two longs and the particle-effect intelligent pointer
// Leaf: ??0AttackParticleAndEndPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769040, 79 bytes (`ret 4`, returns this).
// Callees: CCharString copy ctor 0x0099EC30 (twice), CBaseIntelligentPointer ctor 0x00A01B10,
// GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Identity: the catalog label `_Cons_val<allocator<AttackParticleAndEndPoints>,..>` names the
// construct helper that calls it; this body is a thiscall copy constructor (this in ecx, source on
// the stack, `mov eax,esi`).
// Layout: Ego_r.pdb AttackParticleAndEndPoints (0x18): four unnamed dwords +0..+0x0C, then
// CIntelligentPointer<CThing> ParticleEffect +0x10.  Names of the first four as in the landed
// CTCObjectAugmentations::FireCombatAugEffect 0x00766A60 (StartDummy, EndDummy, ParticleType,
// PeriodicFrames); the bytes show two CCharStrings and two dwords.
// Shape: plain memberwise copy in declaration order: two out-of-line string copies, two dword moves,
// then the pointer's inline copy constructor (base ctor, vtable 0x01238C6C, SetPItem(GetPItem())).
class CThing;
class CBaseObject;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct AttackParticleAndEndPoints {                     // 0x18 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        PeriodicFrames;                         // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;         // +0x10
    AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other);
};

AttackParticleAndEndPoints::AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other)
    : StartDummy(other.StartDummy), EndDummy(other.EndDummy), ParticleType(other.ParticleType),
      PeriodicFrames(other.PeriodicFrames), ParticleEffect(other.ParticleEffect)
{
}