// CTCDivineWrath::PerEffectInfo::PerEffectInfo(const PerEffectInfo& other) -- copy constructor of the divine-wrath spell's per-effect record: the 3D Position (three dwords), then the intelligent Effect thing pointer
// Leaf: ??0PerEffectInfo@CTCDivineWrath@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078E600, 65 bytes (+ padding to 80), __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<long,CIntelligentPointer<CThing> >" is propagated across byte-identical
//   bodies (a pair<long, ...> would hold the pointer at +4). The body copies three dwords and then a
//   CIntelligentPointer<CThing> (vtable 0x01238C6C) at +0xC: Ego_r.pdb CTCDivineWrath::PerEffectInfo (20 bytes) = C3DVector Position +0,
//   CIntelligentPointer<CThing> Effect +0xC; the PDB name sets have CTCDivineWrath::PerEffectInfo::PerEffectInfo(const PerEffectInfo&).
// Evidence: it sits in CTCDivineWrath's TU, after the landed PerEffectInfo default constructor 0x0078E3A0.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: the memberwise copy: C3DVector's memberwise copy constructor (three dword moves), then the pointer's copy
//   constructor inline (base constructor on `lea ebx,[esi+0xc]`, its vtable store, SetPItem(other.GetPItem())).
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // CIntelligentPointer<CThing>: vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class C3DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    float Z;                                            // +0x08
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CTCDivineWrath {
public:
    struct PerEffectInfo {
        C3DVector                     Position;   // +0x00
        CIntelligentPointer<CThing>   Effect;   // +0x0C
        PerEffectInfo(const PerEffectInfo& other);
    };
};

CTCDivineWrath::PerEffectInfo::PerEffectInfo(const PerEffectInfo& other)
    : Position(other.Position), Effect(other.Effect)
{
}