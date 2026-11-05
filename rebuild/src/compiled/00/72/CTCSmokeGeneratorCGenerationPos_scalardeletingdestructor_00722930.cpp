// CTCSmokeGenerator::CGenerationPos::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CGenerationPos runs PEmitter's destructor (CIntelligentPointer<CThing> at +0xC), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCGenerationPos@CTCSmokeGenerator@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722930, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCSmokeGenerator::CGenerationPos::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GCGenerationPos@CTCSmokeGenerator@@QAEPAXI@Z; Ego_r.pdb CTCSmokeGenerator::CGenerationPos is {C3DVector Pos, CIntelligentPointer<CThing> PEmitter +0xC}; the body follows CTCSmokeGenerator::CreatePositions 0x00722590 / FrameUpdate 0x00722740.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() (the implicit ~CIntelligentPointer<CThing>) 0x00A01C10, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
#include "engine/C3DVector.h"
class CBaseObject;
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCSmokeGenerator {
public:
    class CGenerationPos {
    public:
        C3DVector Pos;                                      // +0x00
        CIntelligentPointer<CThing> PEmitter;               // +0x0c
    };
};
void DeleteOne(CTCSmokeGenerator::CGenerationPos* p) { delete p; }