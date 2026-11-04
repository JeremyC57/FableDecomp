// CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) -- base, vtable, copy Position and Rhs
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A06E0, length 80 bytes.
// Callees: the root base constructor CBase::CBase() @ 0x0099A2F0 (no arguments).
// PDB CTCParamsPhysicsBase (0x28 bytes): Position (C3DVector) +0x04, Rhs (CRightHandedSet, two C3DVectors) +0x10.
// Shape: both members copied word by word through ecx/edx/eax, each argument reference loaded from its slot right before
// its copy: C3DVector has a MEMBERWISE user copy constructor (the implicit copy of a plain float record copies through
// `lea eax,[esi+4]` instead; a plain float copy needs no x87 here).
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

struct CBaseClassNonCopyable {
    virtual void v00();
protected:
    CBaseClassNonCopyable();                               // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)
public:
};

class CTCParamsPhysicsBase : public CBaseClassNonCopyable { public:
    C3DVector       Position;                       // +0x04
    CRightHandedSet Rhs;                            // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);
    virtual void v00();
};

CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs)
    : Position(position), Rhs(rhs)
{
}