// CTCPhysicsStandard::ConstructFromParams(CTCParamsBase* params) -- base CTCPhysicsBase construction from the params (position), then the params' orientation set copied into RHSet and OldRHSet
// Leaf: ConstructFromParams
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723570, 99 bytes. CTCPhysicsStandard vtable 0x01265F84 slot 3 (catalog "vfunc_3"; the
//   CTCPhysicsBase slot-3 body 0x006AF700 is labelled CTCPhysicsBase::ConstructFromParams).
// Callees: CTCPhysicsBase::ConstructFromParams(CTCParamsBase*) @ 0x006AF700 (called non-virtually).
// Layout: engine/CTCPhysicsStandard.h / PDB (as the landed UpdateOldRHSet 0x00723930): PPhysicsMeshInfo +0x40,
//   OffsetVector3D +0x44, RHSet +0x50, OldRHSet +0x68 (the header flattens RHSet to bytes, so modelled locally).
//   The physics params: Position +0x04 (read by the base), RHSet +0x10 (CRightHandedSet: Up, Forward).
// Shape: two implicit 24-byte POD assignments from the same source (the source address kept in eax for the second copy).
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CTCParamsBase { public:
    void*     __vftable;                        // +0x00
};
struct CTCParamsPhysics : CTCParamsBase {
    C3DVector       Position;                   // +0x04
    CRightHandedSet RHSet;                      // +0x10
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void ConstructFromParams(CTCParamsBase* params);   // slot 3 (+0xc): 0x006AF700
    unsigned char _pad04[0x3c];                 // +0x04 CTCBase body .. Moving bits (+0x3c)
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void*           PPhysicsMeshInfo;           // +0x40
    C3DVector       OffsetVector3D;             // +0x44
    CRightHandedSet RHSet;                      // +0x50
    CRightHandedSet OldRHSet;                   // +0x68
    virtual void ConstructFromParams(CTCParamsBase* params);
};

void CTCPhysicsStandard::ConstructFromParams(CTCParamsBase* params)
{
    CTCPhysicsBase::ConstructFromParams(params);
    CTCParamsPhysics* physicsParams = static_cast<CTCParamsPhysics*>(params);
    RHSet = physicsParams->RHSet;
    OldRHSet = physicsParams->RHSet;
}