// CThingTrackNode::Create(long, const C3DVector&, CCharString) -- allocate a track node thing, construct it from the def index / position / track name (passed on by value), add the village-member and standard-physics components, place the physics component at the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingTrackNode@@SIPAV1@JABVC3DVector@@VCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008383D0, 265 bytes (0x008383D0..0x008384D8, `ret 4`), static __fastcall (ecx = def index,
//   edx = position, the CCharString by value on the stack).
// Identity: Ego_r.pdb ?Create@CThingTrackNode@@SIPAV1@JABVC3DVector@@VCCharString@@@Z (the one CThingTrackNode factory taking a
//   long, a C3DVector reference and a by-value CCharString); it allocates 0xF0 bytes = sizeof(retail CThingTrackNode) and runs the
//   landed CThingTrackNode::CThingTrackNode() 0x00837C70 and the landed CThingTrackNode::ConstructFromParams 0x00836C50 (whose only
//   caller is 0x00838413, inside this body); the manifest row is `CThingTrackNode::Create`.
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380, CThingTrackNode::CThingTrackNode() 0x00837C70, CCharString copy
//   constructor 0x0099EC30 (the by-value argument built in its stack slot), CThingTrackNode::ConstructFromParams(long, const
//   C3DVector&, CCharString) 0x00836C50, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 for the two component
//   names, CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60, CTCParamsPhysicsBase::CTCParamsPhysicsBase(const
//   C3DVector&, const CRightHandedSet&) 0x006A06E0, the physics component's slot 3 (+0x0C, CTCBase::ConstructFromParams(const
//   CTCParamsBase&)), ~CBaseClassNonCopyable 0x0099A300 (the params' implicit destructor, expanded), CThing::DoActivationSequence()
//   0x004C9CA0, ~CCharString 0x0099EAE0 (the by-value parameter, on every exit).
// Literals: "CTCVillageMember" 0x0123A5D8, "CTCPhysicsStandard" 0x01239F38.
// Layout: the retail CThing chain (PhysicsTC +0x60); CRightHandedSet = Up (0,0,1) + Forward (0,1,0); the params object is
//   vptr + Position + CRightHandedSet (0x28 bytes, Ego_r.pdb CTCParamsPhysicsBase: Position +4, Rhs +0x10).
// Shape: the landed CThingSwitch::Create 0x0083C5D0 form with the two AddTC calls of ConstructFromLevelScript 0x00837E40 in between:
//   `new (false)` (an out-of-line constructor keeps the `p = mem ? ctor(mem) : 0` select), a zero result declared after it, one
//   `p && ConstructFromParams` test, the two component names as temporaries (built before the pushes, destroyed after the call),
//   the physics params temporary, then `if (Activate) result = p;`; the by-value name is destroyed once at the shared exit.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingTrackNode.h"
#include "engine/CRightHandedSet.h"
#include "engine/CBaseClassNonCopyable.h"

// The TC parameter classes have no engine header yet (Ego_r.pdb: CTCParamsBase 4 bytes, no members; CTCParamsPhysicsBase 40 bytes,
// Position +4, Rhs +0x10). The identity right-handed set is the one every CThing*::Create builds (Up (0,0,1), Forward (0,1,0)).
class CIdentityRightHandedSet : public CRightHandedSet {
public:
    CIdentityRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
};
class CTCParamsBase : public CBaseClassNonCopyable {
};
class CTCParamsPhysicsBase : public CTCParamsBase {
public:
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);   // 0x006A06E0
    C3DVector       Position;                                                   // +0x04
    CRightHandedSet Rhs;                                                        // +0x10
};

CThingTrackNode* __fastcall CThingTrackNode::Create(long defIndex, const C3DVector& position, CCharString trackName)
{
    CThingTrackNode* p = new (false) CThingTrackNode;
    CThingTrackNode* result = 0;
    if (p && p->ConstructFromParams(defIndex, position, trackName)) {
        p->AddTC("CTCVillageMember", false, 0);
        p->AddTC("CTCPhysicsStandard", false, 0);
        CTCPhysicsBase* physics = p->GetPhysicsTC();
        physics->ConstructFromParams(CTCParamsPhysicsBase(position, CIdentityRightHandedSet()));
        if (p->DoActivationSequence())
            result = p;
    }
    return result;
}