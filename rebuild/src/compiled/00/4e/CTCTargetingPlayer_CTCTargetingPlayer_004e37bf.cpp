// CTCTargetingPlayer::~CTCTargetingPlayer() -- the implicit destructor: the four thing pointers (+0xEC .. +0xD4), RejectedTargetPoints (+0x74) and Targets (+0x68) through ~vector<NPlayerTargeting::CTargetPoint> 0x004E1AAF, TargetedPoint's thing pointer (+0x4C), then ~CTCTargetingBase 0x004DA8B7 (tail jump)
// Leaf: ??1CTCTargetingPlayer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E37BF, length 79 bytes; thiscall, size-optimised TU; called only from the ??_G 0x004E37A3.
// Identity (agentNF69, q10854): Ego_r.pdb / FableWin.pdb ??1CTCTargetingPlayer@@UAE@XZ; docs/journal/2026-11/EGOD_FINDINGS.md section 4a maps
//   the body onto Ego_r's CTCTargetingPlayer (256 bytes; retail's STLport vectors are 12 bytes, Ego_r's Dinkumware 16, so Targets /
//   RejectedTargetPoints sit at +0x68 / +0x74 and the four CIntelligentPointer<CThing> members 8 bytes lower at +0xD4 .. +0xEC). The base is
//   CTCTargetingBase (RTTI CTCTargetingPlayer : CTCTargetingBase), so the tail call is ??1CTCTargetingBase@@UAE@XZ 0x004DA8B7 (q10855: the
//   entry's CTCTargeted spelling is rejected, see 0x004DA8B7). No ICF in this link (EGOD_FINDINGS section 2), so the catalog name holds.
// Layout: engine/CTCTargetingPlayer.h retail branch with FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST (typed members: NPlayerTargeting::CTargetPoint
//   TargetedPoint, vector<CTargetPoint> Targets / RejectedTargetPoints, CIntelligentPointer<CThing> PLastThreatenedThing .. PTargetOnlyThing)
//   on engine/CTCTargetingBase.h (FABLE_RETAIL_UPDATE_NEAREST_INTERESTING). Was the CGlobalDef stand-in over CDefBase_004da8b7 /
//   CDefMember_004e1aaf.
// Shape: no vtable store, the implicit destructor (DestroyOne, a destruction by name, makes VC7.1 emit it out of line); the base's implicit
//   destructor too (hdr/ overlay opt-in FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR), called out of line under /Os; ~vector out of line
//   (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE: the COMDAT 0x004E1AAF); CTargetPoint's implicit destructor expands to its PThing's
//   ~CBaseIntelligentPointer 0x00A01C10.
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTargetingDef.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTargetingPlayer.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCTargetingPlayer* object)
{
    object->CTCTargetingPlayer::~CTCTargetingPlayer();
}
