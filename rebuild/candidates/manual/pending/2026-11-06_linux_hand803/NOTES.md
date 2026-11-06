# Chunk 803 notes

Four of the six rows RELOCATION_MATCH, and their tests PASS under g++ (tall.sh) and VC7.1 (tall.py). The read-only vland run lists 4 WINS (tmp/vland.log). Two rows are parked in differ/. Every row name is proven from two sources:
1. its entity vtable slot 1, read from the .rdata dwords (each vtable has a single reference, which the Alloc stores);
2. the Ego_r.pdb and FableWin.pdb name sets (`?Main@<Entity>@<Script>@NScript@@EAEXXZ`, private virtual).

No row name is descriptive.

## 00f1b9c0 CQ_ArenaScript::CArenaCellExitGuard::Main (RELOCATION_MATCH)
- **Shape:** the landed CArenaCellDoorGuard2::Main 0x00F19BB0.
- **Opt-ins:**
  - IMPLICIT_ASSIGNMENT and IMPLICIT_COPY.
  - CWIDESTRING, because `L""` converts implicitly to CWideString for SetQuestAsFailed.
  - A row-local inline `~CScriptThing() {}`.
- **Flow:**
  - Talked to: FIRST + FIRST_EXTRA, or SECOND, then the yes/no question.
  - On no, the quest fails and MissionFailed (+0xe1) is set.
  - Hit: heal by 10000, allies, InHitCutsceneAlready.
  - Otherwise: walk back to CellExitMarker, or turn once.

## 00e158f0 CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Main (RELOCATION_MATCH)
- **Predicate:** CIsThingConscious, vtable 0x012C2FE8, with the ctor and dtor inline.
- **Stages:** 2, 4 and 6 of BalverineState.
  - Hit counter on UpdateQuestInfoCounter(WBCounter).
  - Escape walks, HOWL / ST_RUN_JUMP, then EFFECT_GRASS_CLUMPS + RemoveThing.
  - At stage 6: the 5-second CTimer run, then FadeOutAndKillEntity (state 7).
- **Key:** same-TU `__declspec(noinline) inline` CTimer ctor and dtor under auto_inline(off), plus `__forceinline` operator= / GetTime.

## 00db6c60 CQ_NewOakValeIntroScript::CNOVI_BarrelThug::Main (RELOCATION_MATCH)
- **Opt-ins:**
  - IMPLICIT_ASSIGNMENT, but not IMPLICIT_COPY: retail calls 0x004ABE90.
  - FORCEINLINE pause ctor.
  - `__forceinline` GetTime.
  - for(;!false;) loop.
- **Callee:** AddBadDeed 0x00DAEA70 is bound under a wrong landed spelling; see NAME_FIXES.

## 00db09e0 CQ_NewOakValeIntroScript::CNOVI_AffairMan::Main (RELOCATION_MATCH)
- **Model:** BarrelThug's model. The one fix was moving `ClearThingHasInformation(Me); EncounterOver = true;` after the yes/no if-else.
- **Callees and evidence:**
  - IsDistanceBetweenThingsUnder 0x00CBE2FF: `?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z`, the reference form.
  - IsDistanceFromThingToPositionOver 0x00CBE45C, GFIntToCharString 0x0099F830, and operator+(CCharString, CCharString) 0x0099F570 (fable_script_text.h): all bound-same.
  - rand: the import thunk 0x00BFEB16.
  - ClearAllActions 0x007E7400: newly bound (header slot 21).
- **Dirty-words lines:** `CCharString(lit) + GFIntToCharString(BadgerIndex)` as named locals. At BadgerIndex 10 the woman's line comes first.
- **Parent flags:** HeroDiscoveredInfidelity +0x71, ReceiveKiss +0x83, ReceiveHug +0x84, TalkingToWoman +0x93.
- **GiveHeroGold:** called with 1.

## 00ed5f50 CV_SwordInTheStoneScript::CSwordCrowd::Main (DIFFER 4971v5024, 413 lines, parked)
- **What matches:** all calls, branches and inline choices, in order. The residue is the frame packing of temporaries (see differ/ line 2).
- **Spawned function:**
  - `new CSpawnedFunc1<CV_SwordInTheStoneScript, CScriptThing>("ParentClass." + "WatchForDeathDuringSwordPull", 62, &ParentClass, &..::WatchForDeathDuringSwordPull, Me)`, with vtable 0x012F3374.
  - It is killed with KillSpawnedFunction(name, 62).
- **Cast map:** out-of-line operator= (OUTOFLINE_ASSIGN), the map ctor/dtor declared only, and `__forceinline` CScriptThing::GetPos.
- **Hit threshold:** GetHealth(Me) > 0.0001f, the constant 0x0129BA3C. It is bound to `?DELTA_NOTIONAL_ZERO@@3MB` / `?g_NormaliseEpsilon@@3MB` (two spellings, one address; the literal is `__real@38d1b717`).
- **Header debt:** none beyond the opt-in.

## 00ddf090 CQ_PrisonWardenGameScript::CPrisonWarden::Main (DIFFER 4455v4599, 669 lines, parked)
- **What matches:** the same callee multiset. Retail additionally expands:
  - the first two cast-map stores, inline (CBaseObject::operator= 0x0099A3B0 + CCountedPointer share);
  - one seh_me dtor.

  The frame layout differs too.
- **Model:** a new header, engine/CQ_PrisonWardenGameScript.h.
- **Sprintf:** needs the FABLE_RETAIL_CCHARSTRING_SPRINTF opt-in in fable_string.h (Sprintf 0x0099F1F0, cdecl).
- **Header debt:** land `void __cdecl Sprintf(const char*, ...)` in fable_string.h's retail CCharString. The hdr copy is opt-in; the unchanged fable_definition_table.h copy is only an overlay workaround.
- **Globals and flags:**
  - pGlobalScriptDef PrisonWardenGameSneakModifier +0xdd8 and PrisonPoetryMaxLines +0xddc.
  - PMasterData PrisonRaceNumber +0xe8, PrisonKeyStolenByHero +0xee, PrisonGameDoneBefore +0xf3.

## Labels and bindings
- **0x00DAEA70:** AddBadDeed has the wrong landed spelling; see NAME_FIXES. It was already queued by chunk 797.
- **0x0122DEDC:** `__real@00000000`, the known float-zero with many invented spellings (no action).
- **No new symbol is bound at a second address.** I checked each callee against reloc_bindings.tsv before binding it.
