// NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate(CAnimateThing& anim) -- stand animation update, then drive the tail-curl combination anim (type 0x21) by time: frame = clamp(FrameCount / fps / TailStrikePrepareTime, 0, 1); once the previous frame reached 0.8 swap the belly hit location off and the normal one on and kill a live eye emitter, and at 1.0 mark the tail curl finished
// Leaf: ?AnimUpdate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXAAVCAnimateThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9B00, 408 bytes, thiscall, `ret 4`. Grade: RELOCATION_MATCH (408/408). Identity: Ego_r.pdb
//   0x0081B1D0 / FableWin.pdb ?AnimUpdate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXAAVCAnimateThing@@@Z.
// Callees: CModeStandBase::AnimUpdate 0x00862EC0, CAnimateThing::GetFirstCombinationAnimFromType 0x0070C4E0 (Ego_r / FableWin spelling),
//   CDefClassBase::GetSubDef<CScorpionKingBattleDef> 0x008C5A50, CVectorMap::LowerBound 0x0040F020 (inline GetTC), CCharString 0x0099EBF0 /
//   0x0099EAE0, CTCHitLocations::GetPHitLocationWithDefName 0x00815820, NHitLocations::CHitLocation::SetEnabled 0x00816390,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80.
// Shape: Ego_d.pdb lines 2241..2291, parameter anim and locals panim_set, { const_fps, current_time, last_frame, p_battle_def,
//   total_time, frame_info (const CAnimFrameInfo&), cur_frame, { p_TC_hit_locations, p_hit_location_belly, p_hit_location_normal } }.
//   Lines in order: 2256 frame_info (18), 2259 cur_frame (9), 2260 the clamp (25 = VC7.1 29 with two literal temporaries, VC10 fldz /
//   fld1 + fstp 5 B each v `mov [ebp-x], imm` 7), 2261 last_frame (8), 2262 SetFrame (31 = 25 + 3 + 3, two float-variable arguments).
//   The clamp is GFLimit<float,float,float> (engine/GFLimit.h; Ego_d proc GFLimit<float,float,float>, FableWin
//   ??$GFLimit@MMM@@YIXAAMABM1@Z): retail's `fcom 0.0 ; test ah,5 ; jp` / `fcom 1.0 ; test ah,41h ; jne` is its if / else-if; an if /
//   else-if written in place compiles the same but would be several Ego_d lines where 2260 is one. Reading last_frame AFTER the clamp
//   (Ego_d's order) gives retail's frame (`sub esp,0Ch`, last_frame at [esp+0Ch]); reading it first gives `sub esp,8`.
//   Other deltas are the known VC10 ones: 2250 +7 (EH state of the def handle), 2253 +6 (float cast spill), 2270 / 2271 55 v 38 (the
//   CCharString temporaries' EH states). Lines 2273 / 2274 (207 B each, after the two lookups) emit nothing in retail, and the
//   GetSubDef / GetTC lines 2251 / 2268 (458 / 480 B) carry debug-only code; no release bytes stand for either.
//   IS_THING_ALIVE(PEyeEmitter) takes the intelligent pointer itself (three GetPItem calls, 0x008D9C40 / 4C / 5E; with .Get() VC7.1
//   keeps two); its `!= 0` is the existing CINTELLIGENTPOINTER_COMPARE_NULL_INT operator.
// Same-TU recipe: the define set of the landed TU-mate FrameUpdate 0x008E0A10, plus the def / anim / hit-location opt-ins this body uses.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ANIMUPDATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CScorpionKingBattleDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CMainGameComponent.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"
#include "engine/ThingAlive.h"
#include "engine/GFLimit.h"

void NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate(CAnimateThing& anim)
{
    CModeStandBase::AnimUpdate(anim);
    CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x21);
    if (panim_set) {
        CDefPointer<const CScorpionKingBattleDef> p_battle_def;
        PCreature->PeekBaseDef().GetSubDef(&p_battle_def);
        float const_fps = (float)CMainGameComponent::GetConstantFPS();
        float current_time = FrameCount / const_fps;
        float total_time = p_battle_def->TailStrikePrepareTime;
        const CAnimFrameInfo& frame_info = panim_set->GetFrame();
        float cur_frame = current_time / total_time;
        GFLimit(cur_frame, 0.0f, 1.0f);
        float last_frame = frame_info.Current;
        panim_set->SetFrame(CAnimFrameInfo(cur_frame, last_frame));
        if (last_frame >= 0.8f) {
            CTCHitLocations* p_TC_hit_locations = 0;
            PCreature->GetTC(TCI_HIT_LOCATIONS, &p_TC_hit_locations);
            NHitLocations::CHitLocation* p_hit_location_belly = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING_BELLY");
            NHitLocations::CHitLocation* p_hit_location_normal = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING");
            p_hit_location_belly->SetEnabled(false);
            p_hit_location_normal->SetEnabled(true);
            if (IS_THING_ALIVE(PEyeEmitter))
                PEyeEmitter->Kill(true);
        }
        if (last_frame >= 1.0f)
            TailCurlFinished = true;
    }
}