// C3DClothPrimitive::C3DClothPrimitive() -- default constructor: the CBaseClassNonCopyable base, the C3DClothPrimitive vtable, and a null ParticleProgram counted pointer
// Leaf: ??0C3DClothPrimitive@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A95FE0, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x0129DF74 (RTTI .?AVC3DClothPrimitive@@, bases CBaseClassNonCopyable (ctor 0x0099A2F0) -> CBaseClass); FableWin.pdb ??0C3DClothPrimitive@@QAE@XZ; Ego_r.pdb member ParticleProgram (CCountedPointer<CParticleProgram>, +4) then PrimitiveIndex/MaterialIndex, which the default ctor leaves alone; no xrefs.
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/C3DClothPrimitive.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {        // RTTI base of the class below
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0 (PDB ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
#include "engine/CCountedPointer.h"
class CParticleProgram;

class C3DClothPrimitive : public CBaseClassNonCopyable {
public:
    C3DClothPrimitive();
    virtual ~C3DClothPrimitive();
    CCountedPointer<CParticleProgram> ParticleProgram;   // +0x04
    unsigned long PrimitiveIndex;                       // +0x0C (not initialised by the default ctor)
    unsigned long MaterialIndex;                        // +0x10
};
C3DClothPrimitive::C3DClothPrimitive() { }