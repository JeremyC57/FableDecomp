// CTCSmokeGenerator::CGenerationPos::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CGenerationPos runs PEmitter's destructor (CIntelligentPointer<CThing> at +0xC), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCGenerationPos@CTCSmokeGenerator@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722930, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCSmokeGenerator::CGenerationPos::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GCGenerationPos@CTCSmokeGenerator@@QAEPAXI@Z; Ego_r.pdb CTCSmokeGenerator::CGenerationPos is {C3DVector Pos, CIntelligentPointer<CThing> PEmitter +0xC}; the body follows CTCSmokeGenerator::CreatePositions 0x00722590 / FrameUpdate 0x00722740.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() (the implicit ~CIntelligentPointer<CThing>) 0x00A01C10, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C3DVector {                                            // 12 bytes
public:
    float X, Y, Z;
};
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CTCSmokeGenerator {
public:
    class CGenerationPos {
    public:
        C3DVector Pos;                                      // +0x00
        CIntelligentPointer<CThing> PEmitter;               // +0x0c
    };
};
void DeleteOne(CTCSmokeGenerator::CGenerationPos* p) { delete p; }