// CTCDivineWrath::PerEffectInfo::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~PerEffectInfo runs Effect's destructor (CIntelligentPointer<CThing> at +0xC), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GPerEffectInfo@CTCDivineWrath@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E370, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCDivineWrath::PerEffectInfo::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GPerEffectInfo@CTCDivineWrath@@QAEPAXI@Z (PerCreatureInfo of the same class mangles as a struct); Ego_r.pdb PerEffectInfo is {C3DVector Position, CIntelligentPointer<CThing> Effect +0xC}; the body sits in the CTCDivineWrath module (next to a PerEffectInfo constructor 0x0078E3A0, before the CTCDivineWrath methods 0x0078F4D0.. of vtable 0x0126BE84); its byte twin 0x007939A0 is the CTCUnholyPower one.
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
class CTCDivineWrath {
public:
    struct PerEffectInfo {
        C3DVector Position;                                 // +0x00
        CIntelligentPointer<CThing> Effect;                 // +0x0c
    };
};
void DeleteOne(CTCDivineWrath::PerEffectInfo* p) { delete p; }