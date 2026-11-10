// CThingMarker::Create(long def_index, const C3DVector& position, const CCharString& script_name) -- allocate and construct a marker thing, construct it from the def index / position / script name (health and max health 1.0), give its physics component the position with the identity axes, and activate it; null on any failure
// Leaf: ?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D460, 247 bytes (+9 int3 pad; the catalog row's 336 bytes run on into four
//   further functions at 0x0083D560 / 570 / 580 / 590: SPLITS.tsv), static `__fastcall` (def index in ecx, position in edx, the
//   script name on the stack), `ret 4`.
// Name: Ego_r.pdb (0x0082EB20) / FableWin.pdb (0x027FF950) ?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z, right
//   after ?ConstructFromParams@CThingMarker@@IAE_NJABVC3DVector@@ABVCCharString@@@Z; the object's final vtable 0x01245524 has the
//   RTTI .?AVCThingMarker@@ and the 0xD0-byte allocation is sizeof(CThingMarker) (landed caller CAtmosProcess::Update 0x006B2260).
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380, CThingPhysical::CThingPhysical(unsigned long) 0x005296B0 (type 11 =
//   CThingMarker::StatGetType), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CThingGameObject::ConstructFromParams
//   0x008388D0, CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0, the physics
//   component's CTCBase slot 3 ConstructFromParams (+0x0C), ~CBaseClassNonCopyable 0x0099A300 (the params' destructor),
//   CThing::DoActivationSequence() 0x004C9CA0.
// Headers: opt-ins FABLE_RETAIL_CTHINGMARKER_CONSTRUCT (the FableWin.pdb inline constructor and the Ego_r.pdb ConstructFromParams
//   body), FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR (FableWin.pdb inline ??0CThingGameObject@@QAE@K@Z), FABLE_RETAIL_CTHINGPHYSICAL_CTOR
//   (implied by the former; the out-of-line Ego_r.pdb ??0CThingPhysical@@QAE@K@Z).
// Shape: the shared `Create` of the thing classes (CThingSwitch 0x0083C5D0, CThingHolySite 0x0083BBF0): a null result set to the
//   thing only when the activation succeeds, returned once. VC7.1 splits that return three ways: the thing (`mov eax,esi`,
//   0x0083D534), zero after a failed activation (`xor eax,eax`) and the zero register edi on the early failures (`mov eax,edi`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGMARKER_CREATE
#define FABLE_RETAIL_CTHINGMARKER_CONSTRUCT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCParamsPhysicsBase.h"
#include "rebuild_abi.h"

CThingMarker* FABLE_FASTCALL CThingMarker::Create(long defIndex, const C3DVector& position, const CCharString& scriptName)
{
    CThingMarker* p_marker = new (false) CThingMarker;
    CThingMarker* p_result = 0;
    if (p_marker && p_marker->ConstructFromParams(defIndex, position, scriptName)) {
        p_marker->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (p_marker->DoActivationSequence())
            p_result = p_marker;
    }
    return p_result;
}