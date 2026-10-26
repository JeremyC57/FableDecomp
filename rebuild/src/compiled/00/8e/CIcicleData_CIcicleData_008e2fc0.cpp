// CIcicleData::CIcicleData(const CIcicleData& other) -- copy an ice-troll icicle record: the effect and trap intelligent pointers re-registered on the same things, the two timers and the position copied
// Leaf: ??0CIcicleData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2FC0, length 105 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90
// (each pointer: base constructor, vtable 0x01238C6C, SetPItem(other.GetPItem())).
// Layout = Ego_r.pdb CIcicleData (36 bytes): PEffect +0x00, PTrap +0x08 (CIntelligentPointer<CThing>), EffectTimer
// +0x10, TrapTimer +0x14, C3DVector Position +0x18.
// Shape: an out-of-line copy constructor whose members copy one register per word; the position goes through
// C3DVector's memberwise user copy constructor (a POD position copies through `lea` and a re-based pointer).
class CThing;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class C3DVector { public:
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