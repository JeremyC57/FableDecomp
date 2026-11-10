// CThingObject::Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet, unsigned long flags, const CCharString& scriptName) -- allocate an object thing, construct it from the params, place its physics component at the position (identity axes when no set is given) and run the activation sequence; null on any failure
// Leaf: ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00703210, 217 bytes, static __fastcall (ecx = def index, edx = position), `ret 0x10`.
// Identity: Ego_r.pdb / FableWin.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z; 40 landed callers.
// Callees: CThingObject::Alloc 0x00527320, CThingObject::ConstructFromParams 0x00703050, CTCParamsPhysicsBase(const C3DVector&,
//   const CRightHandedSet&) 0x006A06E0, the physics component's slot 3 CTCBase::ConstructFromParams(const CTCParamsBase&),
//   ~CBaseClassNonCopyable 0x0099A300 (the params temporary), CThing::DoActivationSequence 0x004C9CA0.
// Shape: the physics component is read through GetPhysicsTC() in each branch, and the real CTCParamsPhysicsBase ctor body is visible
//   (FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY, noinline): VC7.1 then knows the temporary is not captured and keeps the object in ebx
//   (a declaration-only ctor swaps ebx/edi). The no-set branch builds the default CRightHandedSet (Up 0,0,1 / Forward 0,1,0) inline.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#define FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCParamsPhysicsBase.h"

CThingObject* __fastcall CThingObject::Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet,
                                              unsigned long flags, const CCharString& scriptName)
{
    CThingObject* object = (CThingObject*)Alloc();
    CThingObject* result = 0;
    if (object && object->ConstructFromParams(defIndex, position, playerNumber, flags, scriptName)) {
        if (rhSet)
            object->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, *rhSet));
        else
            object->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (object->DoActivationSequence())
            result = object;
    }
    return result;
}