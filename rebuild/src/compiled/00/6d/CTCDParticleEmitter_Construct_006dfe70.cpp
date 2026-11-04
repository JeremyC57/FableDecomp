// CTCDParticleEmitter::Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad) -- place the emitter's physics at the position with the identity orientation, then record the particle type and the IndependantObject / ForceParticleToLoad flags
// Leaf: ?Construct@CTCDParticleEmitter@@AAEXJABVC3DVector@@_N1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFE70, 178 bytes, `ret 10h` (0x28-byte frame = the params temporary). Ego_r.pdb (private).
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300 (the CTCParamsPhysicsBase temporary, vtable 0x0125C818, its
//   ctor ??0CTCParamsPhysicsBase@@QAE@ABVC3DVector@@ABVCRightHandedSet@@@Z expanded in place); the physics TC's
//   CTCBase::ConstructFromParams(const CTCParamsBase&) (virtual slot 3, +0x0C). Global g_IdentityRHSet 0x01380104 (as the landed
//   CTCDParticleEmitter::OnCreate 0x006E13E0).
// Shape: CTCBase::GetParentThing().GetPhysicsTC() is the call's object; the temporary binds to the const reference parameter;
//   the two one-bit stores merge into one read-modify-write. Layout (Ego_r.pdb): ParticleTypeID +0x10, the bool:1 group +0x44.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
extern const CRightHandedSet g_IdentityRHSet;   // 0x01380104
class CTCParamsBase : public CBaseClassNonCopyable { };
class CTCParamsPhysicsBase : public CTCParamsBase {   // vtable 0x0125C818
public:
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) { }
    C3DVector       Position;                   // +0x04
    CRightHandedSet Rhs;                        // +0x10
};
class CTCDParticleEmitter : public CTCBase {
    void Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad);
    unsigned char _pad0c[0x10 - 0x0c];
    long ParticleTypeID;                        // +0x10
    unsigned char _pad14[0x44 - 0x14];
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;               // +0x44 bit 1
    bool IndependantObject : 1;                 // +0x44 bit 2
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
};

void CTCDParticleEmitter::Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad)
{
    GetParentThing().GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, g_IdentityRHSet));
    ParticleTypeID = particleType;
    IndependantObject = independent;
    ForceParticleToLoad = forceLoad;
}