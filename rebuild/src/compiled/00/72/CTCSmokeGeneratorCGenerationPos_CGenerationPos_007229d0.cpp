// CTCSmokeGenerator::CGenerationPos::CGenerationPos(const CGenerationPos& other) -- copy constructor of a smoke generator's emission position: the 3D Pos (three dwords), then the intelligent PEmitter thing pointer
// Leaf: ??0CGenerationPos@CTCSmokeGenerator@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007229D0, 65 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<long,CIntelligentPointer<CThing> >" is propagated across byte-identical
//   bodies (a pair<long, ...> would hold the pointer at +4). The body copies three dwords and then a
//   CIntelligentPointer<CThing> (vtable 0x01238C6C) at +0xC: Ego_r.pdb CTCSmokeGenerator::CGenerationPos (20 bytes) = C3DVector Pos +0,
//   CIntelligentPointer<CThing> PEmitter +0xC; the PDB name sets have CTCSmokeGenerator::CGenerationPos::CGenerationPos(const CGenerationPos&).
// Evidence: it sits in CTCSmokeGenerator's TU (0x007224C0..0x00723240), next to the landed CGenerationPos __copy 0x00722C90.
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
    ~CBaseIntelligentPointer();                         // 0x00A01C10
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

class CTCSmokeGenerator {
public:
    class CGenerationPos {
    public:
        C3DVector                     Pos;        // +0x00
        CIntelligentPointer<CThing>   PEmitter;   // +0x0C
        CGenerationPos(const CGenerationPos& other);
    };
};

CTCSmokeGenerator::CGenerationPos::CGenerationPos(const CGenerationPos& other)
    : Pos(other.Pos), PEmitter(other.PEmitter)
{
}