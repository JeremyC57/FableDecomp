// CThingSearchTools::GetPBestThing<CThingFilter_HasTC, CThingCompare_Nearest, CThingAICreature>(const CThingFilter_HasTC&, const CThingCompare_Nearest&, CThingAICreature**) const -- nearest scan-visible AI creature that has the thing component
// Leaf: ??$GetPBestThing@VCThingFilter_HasTC@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVCThingFilter_HasTC@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F744, length 112 bytes (`ret 0Ch`), a size-optimised TU (ebp frame).
// Names (NF115, PRIORITY q4495; was the local stand-in CThingSearchToolsMethods::GetPBestThing over local CThing / CThingAICreature models):
//   Ego_r.pdb + FableWin.pdb ??$GetPBestThing@VCThingFilter_HasTC@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_N...@Z
//   (const member template); callers CTCVillage::CalculateSecuritySweepTasks 0x00659800 (nearest-guard search, binds this spelling) and
//   CAIStateGroup_MinionGuardFollowPlayer::IsRunnable 0x0090C1A0. T = CThingAICreature, not the CThingCreatureBase instance both PDBs
//   also list: retail's type test is `and eax,1Fh ; cmp al,1Fh` (0x0065F76F), CThingAICreature::StatGetTypeFlags 0x1F (CThingCreatureBase 0x0F),
//   and the comparator called is CThingCompare_Nearest::operator()<CThingAICreature> 0x0065DA97.
// Callees: PeekClassList 0x0049C770, CThingFilter_HasTC::operator() 0x006A80A0, CThingCompare_Nearest::operator()<CThingAICreature> 0x0065DA97.
// Shape: explicit instantiation of engine/CThingSearchTools.h FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY in the CTCVillage TU
//   setup of 0x00659800 (its caller in the same retail TU), with CTHINGFILTER_HASTC_OUTOFLINE (retail calls 0x006A80A0) and the NF115
//   nested overlay opt-in CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST (retail's `test edi,edi ; je` before the filter).
#define FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23

#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_STLP_BINARY_COMPOSE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_SECURITY_SWEEP_LIFETIMES_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_SCOPED_PTR_RESET
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "engine/CMainGameComponent.h"
#include <math.h>
#include "engine/CThingCompare_Nearest.h"
#include "engine/HasGuardProperty.h"
#include "stlport/fable_count_if.h"
#include "stlport/fable_algorithm.h"

template bool CThingSearchTools::GetPBestThing<CThingFilter_HasTC,CThingCompare_Nearest,CThingAICreature>(
    const CThingFilter_HasTC&,const CThingCompare_Nearest&,CThingAICreature**) const;
