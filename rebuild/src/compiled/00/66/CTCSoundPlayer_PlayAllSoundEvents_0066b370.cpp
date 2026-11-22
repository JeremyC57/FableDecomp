// CTCSoundPlayer::PlayAllSoundEvents() -- play the sound events of the parent's current animations: for every anim set (the combination anims and the main anim) take the anim with the strongest blend and, for each of its events between the last and the current percentage complete, either stop the criteria that contain the event name (a stop event) or play the first sound event that exists from "<identifier>;<event>" with the voice / extra tag / floor material suffixes
// Leaf: ?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066B370, 1808 bytes, thiscall, no arguments. Identity: Ego_r.pdb / FableWin.pdb
//   ?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ (Ego_r 0x005749A0, FableWin 0x01E7E870); sole retail caller CTCSoundPlayer::FrameUpdate's
//   tail jump. Text: Ego_d.pdb's locals, blocks and line table of this function (every local below carries its Ego_d name and type).
// Callees: GFWorld3DDistanceSmallerThan 0x006614F0, CThing::GetPos 0x004C73D0, CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604,
//   CThing::GetSampleBank 0x004C7A80, CTCBase::PeekWorldMap 0x00686D20, CWorldMap::GetStrongestEngineThemeAt 0x0063ADF0,
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CEngineThemeDef> 0x0063F3A0, CThing::PeekDefinitionManager
//   0x004C7990, GetDef<CMaterialDef> 0x0066C340 (same TU), CDefString::operator CCharString 0x00415DD0, CTCAnimationComplex::GetAnimation
//   0x0070B460, NAnimationEvents::GetSoundEventManager 0x006F90C0, CAnimateThing::GetAllCombinationAnims 0x0070D100,
//   vector<CAnimSet*>::push_back 0x0066C4D0 (same TU), CManager::GetAnimEventsBetweenTimes 0x006F99B0,
//   CSymbolMap::FindFirstSymbolWithVal(long) 0x009E5090, CIntelligentPointerBase::GetPItem 0x00A01B50, operator+(CCharString, const char*)
//   0x0099F600, operator+(CCharString, CCharString) 0x0099F570, CTCSoundPlayer::InternalPlaySoundEvent 0x0066B120.
// Headers (overlay, AUTH22 / AUTH23): opt-ins FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS (declarations), FABLE_RETAIL_CANIMSET_ANIMINDICES
//   (four FableWin.pdb inline accessors), FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED (see Inferred below),
//   FABLE_RETAIL_CTHINGSOUNDEMITTER_STOPALLCRITERIACONTAININGSTRING (inline forwarder to CASoundEmitter slot 12; DESCRIPTIVE name),
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY (scaffolding: gives this TU's pick-any GetSubDef<CEntitySoundDef> the
//   owner 0x0065F604's size / frame-pointer body; no effect on this function's bytes);
//   new headers engine/CSmallVector.h, engine/NAnimationEvents_CEvent.h, engine/NAnimationEvents_CManager.h (PDB names and layouts).
//   The visible GetDef / GetSubDef bodies (existing ..._GETDEF_GLOBAL_BODY / ..._GETSUBDEF_BODY opt-ins) let VC7.1 keep the three def
//   handles in registers and release them in place, as retail does; CThing's slot-38 / slot-40 virtuals GetThingStandingOn /
//   GetFloorMaterial come from the existing FABLE_RETAIL_CTHING_ORIGINAL_VTABLE.
// Inferred: FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED (engine/CAnimSet.h) makes GetPercentageComplete / GetLastPercentageComplete return
//   `const float&`. The call text is Ego_d line 208's (both accessors as arguments of GetAnimEventsBetweenTimes); all three later VC10 PDBs
//   give the accessors by value, which under VC7.1 leaves two stored temporaries here (frame 0x7C, DIFFER(1844v1808)); retail 0x0066B6E0
//   has none. Owner decision 2026-11-22 (land as Inferred, as 0x0093D270).
// Inferred: the stop-event arm (retail 0x0066B750..0x0066B77D: GetPItem x2 and the emitter's slot-12 virtual) has no code in Ego_d's line
//   table (lines 216..227 are empty there); its text is fitted to retail's bytes.
// Descriptive: g_bPlayAnimationSoundEvents 0x01375758 (a .data bool, read only at 0x0066B383; no name in either PDB);
//   CThingSoundEmitter::StopAllCriteriaContainingString (see the header); CEngineThemeDef is a local model (no shared header; the
//   same text as landed 0x007F0300).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDFIRSTSYMBOLWITHVAL_VALUE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMSET_ANIMINDICES
#define FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_STOPALLCRITERIACONTAININGSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSymbolMap.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CMaterialDef.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/NAnimationEvents_CManager.h"
#include "engine/CTCSoundPlayer.h"

class CASoundBank;
extern bool g_bSoundEnabled;                       // 0x0137E974
extern bool g_bPlayAnimationSoundEvents;           // 0x01375758 (descriptive)
extern const unsigned long g_InvalidSoundHandle;   // 0x0129D25C
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x8c - 0x25];
    long          MaterialDef;                                  // +0x8c
};

void CTCSoundPlayer::PlayAllSoundEvents()
{
    if (!g_bSoundEnabled || !g_bPlayAnimationSoundEvents ||
        !GFWorld3DDistanceSmallerThan(CMainGameComponent::Get()->PeekWorld().GetGameLODCentre(), GetParentThing().GetPos(), AnimCriteriaClipDistance))
    {
        return;
    }
    CTCAnimationComplex* ptc_animation;
    if (GetParentThing().GetTC(&ptc_animation))
    {
        CCharString sound_identifier;
        CDefPointer<const CEntitySoundDef> psound_def;
        if (GetParentThing().PeekBaseDef().GetSubDef(&psound_def))
            sound_identifier = psound_def->SoundIdentifier;
        if (sound_identifier != "")
        {
            CASoundBank& bank = GetParentThing().GetSampleBank();
            CCharString material_name;
            long material_index = 0;
            CThing* pstanding_on_thing = GetParentThing().GetThingStandingOn();
            if (pstanding_on_thing)
            {
                material_index = pstanding_on_thing->GetFloorMaterial();
                LastMaterialIndex = material_index;
            }
            else if (GetParentThing().GetPhysicsTC()->IsOnGroundSurface())
            {
                CDefPointer<const CEngineThemeDef> pdef;
                long strength;
                if (GetDefinitionManager().GetDef(PeekWorldMap().GetStrongestEngineThemeAt(GetParentThing().GetPos(), strength), &pdef))
                {
                    material_index = pdef->MaterialDef;
                    LastMaterialIndex = material_index;
                }
            }
            else
                material_index = LastMaterialIndex;
            if (material_index > 0)
            {
                CDefPointer<const CMaterialDef> pmaterial_def;
                GetParentThing().PeekDefinitionManager().GetDef(material_index, &pmaterial_def);
                material_name = pmaterial_def->GetInstantiationName();
            }
            CAnimateThing& anim = ptc_animation->GetAnimation();
            const NAnimationEvents::CManager& sound_event_manager = NAnimationEvents::GetSoundEventManager();
            std::vector<CAnimSet*> anim_sets;
            anim.GetAllCombinationAnims(anim_sets);
            CAnimSet& main_anim = anim.GetMainAnim();
            anim_sets.push_back(&main_anim);
            for (std::vector<CAnimSet*>::const_iterator it = anim_sets.begin(); it != anim_sets.end(); ++it)
            {
                const CAnimSet* panim_set = *it;
                const std::vector<short>& anim_indeces = panim_set->GetAnimIndices();
                long index_to_use = 0;
                float best_strength = 0.0f;
                for (long c1 = 0; c1 < anim_indeces.size(); c1++)
                {
                    float strength = panim_set->GetAnimBlendStrength(c1);
                    if (strength > best_strength)
                    {
                        best_strength = strength;
                        index_to_use = anim_indeces[c1];
                    }
                }
                if (index_to_use != 0 &&
                    panim_set->GetPercentageComplete() > panim_set->GetLastPercentageComplete())
                {
                    CSmallVector<NAnimationEvents::CEvent, 32> event_container;
                    if (sound_event_manager.GetAnimEventsBetweenTimes(index_to_use, event_container, panim_set->GetLastPercentageComplete(), panim_set->GetPercentageComplete()))
                    {
                        for (const NAnimationEvents::CEvent* event_it = event_container.begin(); event_it != event_container.end(); event_it++)
                        {
                            const CSymbolMap::CSymbolInfo* psymbol_info = sound_event_manager.GetEventSymbolMap().FindFirstSymbolWithVal(event_it->Index);
                            const CCharString& loop_event_name = psymbol_info->EntryName;
                            if (event_it->StopEvent)
                            {
                                if (PAttachedEmitter.GetPItem())
                                    PAttachedEmitter->StopAllCriteriaContainingString(loop_event_name);
                            }
                            else
                            {
                                CCharString sound_name;
                                sound_name += sound_identifier;
                                sound_name += ";";
                                sound_name += loop_event_name;
                                CCharString voice_name(VoiceName);
                                CCharString extra_tag(ExtraTag);
                                bool played_sound = false;
                                if (voice_name != "")
                                {
                                    if (extra_tag != "")
                                    {
                                        CCharString sound_name_and_voice = sound_name + ";" + voice_name + ";" + extra_tag;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_voice, false, false, false, -1) != g_InvalidSoundHandle)
                                            played_sound = true;
                                    }
                                    else
                                    {
                                        CCharString sound_name_and_voice = sound_name + ";" + voice_name;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_voice, false, false, false, -1) != g_InvalidSoundHandle)
                                            played_sound = true;
                                    }
                                }
                                if (!played_sound)
                                {
                                    CCharString sound_name_and_tag_name_and_material = sound_name + ";" + material_name + ";" + extra_tag;
                                    if (InternalPlaySoundEvent(bank, sound_name_and_tag_name_and_material, false, false, false, -1) == g_InvalidSoundHandle)
                                    {
                                        CCharString sound_name_and_material = sound_name + ";" + material_name;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_material, false, false, false, -1) == g_InvalidSoundHandle)
                                            if (InternalPlaySoundEvent(bank, sound_name, false, false, false, -1) == g_InvalidSoundHandle)
                                                InternalPlaySoundEvent(bank, loop_event_name, false, false, false, -1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}