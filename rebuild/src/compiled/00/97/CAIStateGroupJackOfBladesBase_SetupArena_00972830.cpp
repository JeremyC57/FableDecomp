// CAIStateGroup_JackOfBladesBase::SetupArena() -- find the "FrescoCentre" thing within 50m (rough area) of the creature, latch it as the arena centre, and switch the camera manager to the CAMERA_MANAGER_SET_DEFAULT camera set
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972830, length 352 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / CCharString(const CCharString&) @ 0x0099EC30 / ~CCharString
//          @ 0x0099EAE0, CThing::GetPos @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools
//          @ 0x008FD000, CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasScriptName, CThing> @ 0x007FD110,
// 0x007FD110 is the <CThingFilter_HasScriptName, CThing> instance (filter operator 0x006A7E30 compares the ScriptName; naming wave 19)
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable 0x01238C6C), CAIStateGroupBase::GetPWorld
//          @ 0x008FCFC0, the world's +0x34 object virtual slot 9 (+0x24, camera manager out pointer),
//          CAIStateGroupBase::GetPDefinitionManager @ 0x008FCFB0 (manifest "_Cons_val" is propagated),
//          CDefinitionManager::GetDef<CCameraManagerSetDef> @ 0x004AB990, CGameCameraManager::SetCameraManagerSetDef
//          @ 0x0069AC90; the def's virtual slot 1 (destroy on the last release).
// Shape: the search is one full expression (arguments right to left: the filter temporary built from the name
//          temporary, then the 100m box around the truncated creature XY); the def pointer's release goes through the
//          ref-count base's DecRef/GetRefCount accessors.
// Layout: engine/CAIStateGroup_JackOfBladesBase.h (PArenaCentre +0x1c); PBrain is the first dword at +0x04.
// Leaf: ?SetupArena@CAIStateGroup_JackOfBladesBase@@MAEXXZ
// NF49 (round 7): ported onto the shared AI / def / camera headers with the define and include set of the landed sibling 0x00972F30
// (CAIStateGroup_JackOfBladesStage2::SetupArena, which calls this body): the manager accessor 0x008FCFB0 is the protected const
// ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ (was a made-up GetPDefinitionManager on a local
// CAIStateGroupJackOfBladesBaseMethods adapter), GetDef 0x004AB990 is ??$GetDef@VCCameraManagerSetDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCCameraManagerSetDef@@@@@Z
// (FableWin.pdb) and the row's own symbol is the PDB's protected virtual ?SetupArena@CAIStateGroup_JackOfBladesBase@@MAEXXZ (Ego_r.pdb /
// FableWin.pdb; reloc_bindings already binds 0x00972F30's call to it). The local CCharString / C2DBoxI / CThing / CThingSearchTools /
// CIntelligentPointer / CCameraManagerSetDef / CDefPointer / CDefinitionManager / CGameCameraManager / CWorld / CAIBrain /
// CAIStateGroupBase models are gone.
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_C2DCOORDI_COPY
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#include <stdio.h>
#define FABLE_RETAIL_JACK_SETUPARENA_PASS23
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/CGameCameraManager.h"
void CAIStateGroup_JackOfBladesBase::SetupArena()
{
    {
        CThing* centre = 0;
        PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 50), CThingFilter_HasScriptName("FrescoCentre"), &centre);
        PArenaCentre = CIntelligentPointer<CThing>(centre);
    }
    CGameCameraManager* camera;
    if (GetWorld().GetGameCamera()->GetCameraManager(&camera)) {
        CDefPointer<const CCameraManagerSetDef> def;
        PeekDefinitionManager().GetDef("CAMERA_MANAGER_SET_DEFAULT", &def);
        camera->SetCameraManagerSetDef(def);
    }
}
