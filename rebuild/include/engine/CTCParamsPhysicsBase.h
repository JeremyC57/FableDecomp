#ifndef FABLE_ENGINE_CTCPARAMSPHYSICSBASE_H
#define FABLE_ENGINE_CTCPARAMSPHYSICSBASE_H
// New header (chunk 1014, row 0x00703210 CThingObject::Create). Ego_r.pdb: CTCParamsBase (4 bytes, no members) and
// CTCParamsPhysicsBase (40 bytes: Position +4, Rhs +0x10); RTTI CTCParamsPhysicsBase : CTCParamsBase : CBaseClassNonCopyable : CBaseClass
// (vtables 0x0125C818 / 0x0125C810). Ego_r.pdb ??0CTCParamsPhysicsBase@@QAE@ABVC3DVector@@ABVCRightHandedSet@@@Z is out of line
// in retail (0x006A06E0: CBaseClassNonCopyable() 0x0099A2F0, then the vtable and the copies); the implicit destructors reach
// ~CBaseClassNonCopyable 0x0099A300 directly.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CTCParamsBase : public CBaseClassNonCopyable {
};
class CTCParamsPhysicsBase : public CTCParamsBase {
public:
    C3DVector Position;                                                  // +0x04
    CRightHandedSet Rhs;                                                 // +0x10
#ifdef FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY
    // The real body, kept out of line (retail calls 0x006A06E0 at every site): visible to the caller's escape analysis.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) {}   // 0x006A06E0
#else
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);   // 0x006A06E0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCParamsPhysicsBase) == 0x28);
#endif // FABLE_ENGINE_CTCPARAMSPHYSICSBASE_H
