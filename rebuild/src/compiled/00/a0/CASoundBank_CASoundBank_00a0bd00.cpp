// CASoundBank::CASoundBank() -- default constructor: the CBaseClassNonCopyable base, the CASoundBank vtable, and a null PSymbolMap counted pointer
// Leaf: ??0CASoundBank@@QAE@XZ
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
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};
class CCRCSymbolMap;

class CASoundBank : public CBaseClassNonCopyable {
public:
    CASoundBank();
    virtual ~CASoundBank();
    CCountedPointer<CCRCSymbolMap> PSymbolMap;   // +0x04
};
CASoundBank::CASoundBank() { }