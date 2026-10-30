# Chunk 584 notes

8 of 10 rows RELOCATION_MATCH (no plain MATCH, no retail_asm); 2 parked in differ/. Tests PASS under g++ (tall.sh) and
VC7.1 (tall.py). The read-only vland check reports WINS: 8, with no RELOC_IDENTITY (only RELOC_NAME warnings about manifest labels).

## Names and evidence (all proven; all 10 manifest labels were wrong)
| addr | name | evidence |
|---|---|---|
| 00DA2CA0 | NScript::CQ_MinionCampScript::CBriarRose::PlayCutscene(CCharString) (private) | Ego_r/FableWin name sets; "ROSE" cast key; seh_me +0x1c; neighbours DoSpeech/UpdateTarget |
| 00E0E040 | NScript::CQ_UndeadRisingScript::ActivateNextLamp() | the landed 0x00E0DE90 spawns it under "ActivateNextLamp"; Ego_r name set |
| 00E68B20 | NScript::CV_ChickenKickingScript::PlayCutscene(CCharString) | Ego_r; caller CChickenMaster::Main 0x00E64FB0; csargs/seh_* offsets |
| 00E7B4E0 | NScript::CV_FishermanScript::Main() | script vtable 0x012EA024 slot 2 (RegisterMain/Init landed); Ego_r/FableWin |
| 00E7BD60 | NScript::CV_FishermanScript::CFisherman::Init() (private virtual) | entity vtable 0x012EA0B4 slot 2, stored by landed Alloc 0x00E7B7B0; FableWin |
| 00E9BBC0 | NScript::CV_HeroDuelScript::DuelChallenge() | CS_GIBBETHILL_HERODUEL_CHALLENGE(_W) strings; Ego_r name set and order |
| 00EC2520 | NScript::CV_RandomPopulationSimScript::SetCurrentPopulationRegionFromRegionName(CCharString&) | the landed caller 0x00EC2240 binds it; Ego_r |
| 00EDA5E0 | NScript::CV_TalentlessBardScript::IsHeroDressedAsAWoman() | Ego_r order of the four dress checks = retail address order; DRESS strings; caller 0x00ED9BB0 |
| 00EF5840 (parked) | NScript::CPersonalScript_EmptyScript::CBanditWalker::Main() (private virtual) | entity vtable 0x012F60F0 slot 1, stored by landed Alloc 0x00EF1B70; Ego_r/FableWin |
| 00F3CF50 (parked) | CTriangleStripifier2::CalcSectionPolyMask(...) (private) | Ego_r AAE spelling; landed caller 0x00F3D830 |

## Label corrections (manifest -> real)
Game_InitializeRoseCharacter -> CBriarRose::PlayCutscene; CScriptBase_HandleGraveyardSpawn -> ActivateNextLamp;
Game_InitializeArena -> CV_ChickenKickingScript::PlayCutscene; sub_00e7b4e0 -> CV_FishermanScript::Main;
CScriptGameResourceObjectScriptedThingBase_HandleFishermanNPCLogic -> CFisherman::Init; DoMission_HeroDuelChallenge ->
DuelChallenge; CScriptBase_ProcessAlbionPopulationSim_Filter -> SetCurrentPopulationRegionFromRegionName;
ProcessSongsClothes_ApplyDressCode -> IsHeroDressedAsAWoman. The manifest also mislabels these callees: 0x00CB8930 (labelled
CTCVillage::OnInitialActivate; really CScriptBase::PostAddScriptedEntities), 0x006E7AB0 (labelled CCountedPointer_Release; really
~CCountedPointer<CScriptGameResourceObjectMovieBase>), 0x00CDBFB0 (labelled CRetailTree_erase...; really the
map<CCharString, ScriptedThingBase> destructor).

## Parked
- 00EF5840 DIFFER(768v727), 3 grades. The only residue: inside the inlined destructor of the outer
  CPredicate_And<CPredicate_And<A,A>,A> temporary, retail calls the member's ~CPredicate_And<A,A> out of line (0x00EF5B50).
  Retail still expands the inner temporary's identical destructor. Every other byte and the frame layout match.
- 00F3CF50 DIFFER(679v683), 4 grades. Two causes: retail reuses a dead argument slot for the allocator temporary, and it
  strength-reduces i*2 in the first loop.

## Header debt (local stand-ins; see LESSONS.md "Review at landing")
- 00DA2CA0, 00E68B20, 00E9BBC0 (and the parked 00EF5840) each carry their own copies of CBaseObject (vtable 0x0129A7D8),
  CScriptGameResourceObjectBase (0x0126008C), CScriptGameResourceObjectScriptedThingBase (0x0127094C),
  CScriptGameResourceObjectMovieBase (0x01260EF4), and an STLport map/_Rb_tree. The shared engine/CBaseObject.h and
  engine/CScriptGameResourceObjectScriptedThingBase.h are generated flat layouts with no hierarchy or vtable, so they cannot
  model these classes.
- 00E7BD60: local CSpawnedFunc1<T,P1> template (vtable 0x012EA164 for <CV_FishermanScript, CScriptThing>).
- 00E0E040 (and the parked 00F3CF50): local minimal std::vector / allocator.
- 00EC2520: local CBasicString / CCharStringData / CCharString (the landed 0x00F088E0 model).
- 00E9BBC0: opaque local CQ_SunnyvaleMasterData {_before[0x74]; bool WhisperKilledByHero;}.
- SpawnFunction (00E7BD60) is a source-level __forceinline helper; it has no retail symbol.
