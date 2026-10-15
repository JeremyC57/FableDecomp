// CIcicleData::CIcicleData(const CIcicleData& other) -- copy an ice-troll icicle record: the effect and trap intelligent pointers re-registered on the same things, the two timers and the position copied
// Leaf: ??0CIcicleData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2FC0, length 105 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90
// (each pointer: base constructor, vtable 0x01238C6C, SetPItem(other.GetPItem())).
// Layout = Ego_r.pdb CIcicleData (36 bytes): PEffect +0x00, PTrap +0x08 (CIntelligentPointer<CThing>), EffectTimer
// +0x10, TrapTimer +0x14, C3DVector Position +0x18.
// Shape: an out-of-line copy constructor whose members copy one register per word; the position goes through
// C3DVector's memberwise user copy constructor (a POD position copies through `lea` and a re-based pointer).
struct CThing;
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) {}
};
struct CIcicleData {
    CIntelligentPointer<CThing> PEffect;           // +0x00
    CIntelligentPointer<CThing> PTrap;             // +0x08
    long                        EffectTimer;       // +0x10
    long                        TrapTimer;         // +0x14
    C3DVector                   Position;          // +0x18
    CIcicleData(const CIcicleData& other);
};

CIcicleData::CIcicleData(const CIcicleData& other)
    : PEffect(other.PEffect), PTrap(other.PTrap), EffectTimer(other.EffectTimer), TrapTimer(other.TrapTimer), Position(other.Position)
{
}
