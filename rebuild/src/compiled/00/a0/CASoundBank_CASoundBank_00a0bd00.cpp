// CASoundBank::CASoundBank() -- default constructor: the CBaseClassNonCopyable base, the CASoundBank vtable, and a null PSymbolMap counted pointer
// Leaf: ??0CASoundBank@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BD00, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x0129C9AC (RTTI .?AVCASoundBank@@, bases CBaseClassNonCopyable (ctor 0x0099A2F0) -> CBaseClass); Ego_r.pdb member PSymbolMap (CCountedPointer<CCRCSymbolMap>, +4), the class's only member; no xrefs (out-of-line COMDAT of the inline default ctor).
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/CASoundBank.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {        // RTTI base of the class below
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0 (PDB ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
#include "engine/CCountedPointer.h"
class CCRCSymbolMap;

class CASoundBank : public CBaseClassNonCopyable {
public:
    CASoundBank();
    virtual ~CASoundBank();
    CCountedPointer<CCRCSymbolMap> PSymbolMap;   // +0x04
};
CASoundBank::CASoundBank() { }