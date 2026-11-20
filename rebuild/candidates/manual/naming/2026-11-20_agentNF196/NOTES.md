# NF196 notes (round 10)

## For the coordinator (decisions and reports)

- **Casts to shared classes.**
  - 0x0065BFFA (CTCVillage::IsReportedOrUnreportedCrimeKnown) reaches the const `CThing::GetTC` through `static_cast<const CThing*>(villager)`. That cast is Inferred: with no const, the non-const LowerBound 0x004365B0 binds (RELOC_CONFLICT). It is a static_cast between real shared types, not a cast from a stand-in.
  - 0x0093C6D0 (q6456) uses two C-style downcasts, `(CThingCreatureBase*)GetCreature()` (row lines 104-105). They go from `CThing*` (the row-local CAIBrain model's PCreature, which predates this round) to the shared CThingCreatureBase. HEAD made the same cast, but onto the stand-in CThingCreatureAnim.
    - Its row-local CCreatureAction_PlayAnimation model, which also predates this round, now has the ctor (CThingCreatureBase&, const CAnimationEntry&, const CSoundPair&, bool, bool, bool, long, long). That is `??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@_N33JJ@Z`, present in both Ego_r.pdb (0x0051CA80) and FableWin.pdb (0x01DDF0C0), and the spelling the landed 0x00842E40 already binds at 0x00842570.
    - The shared engine/CCreatureAction_PlayAnimation.h declares only the two 11-parameter ctors, so the row keeps its local model for this one. That is a header gap: the 8-parameter PDB ctor is missing.
- **Count-neutral callee moves (NAME_FIXES.tsv).** In each case the landed self row defines a different spelling from the PDB one the callers now need:
  - 0x008BCA00: CCreatureAction_CastWillLightningSpell ctor; PDB AAV form, self defines PAV/JJ.
  - 0x0065D639: IS_THING_DEAD<CThingAICreature*>.
  - 0x004A88F0: IS_THING_ALIVE<CThingAICreature*>.
  - Callers I ported bind the PDB spellings. Until the self rows are respelled, those binding pairs stay count-neutral, the same state as before the port.
- **0x0072F0E0 (NAME_FIXES.tsv, count-neutral).** `?SetRespawnable@CTCDummyVillager@@QAEX_N@Z` is in Ego_r.pdb 0x007374E0 and FableWin.pdb 0x0257BD90. The landed 0x0065B142 binds it, and so now does 0x0065C45A, whose HEAD bound `?SetEnabled@CTCDummyVillager@@QAEX_N@Z`.
  - The self row is the global stand-in CTCPhysicsNavigator_EnableNavigator; its Leaf line reads "EnableNavigator".
  - vland's RELOC_NAME warning on 0x0065C45A comes from the catalog label `_global::SetMember_byte0x48_0072f0e0`.
- **Landed row that fails under my grader: 0x0065A5BB** (CTCVillage::GetRegionLeavingDurationForCurrentCrimes, not my row).
  - It gave DIFFER(211v203) in the q6449 census, with no flags.
  - Its preprocessed include list is fable_compiler.h, CVectorMap.h, CArray.h and stlport/fable_containers.h: none of the four overlay headers. So the overlay cannot cause it.
  - It has no reloc_audit.tsv line. I did not investigate further.
- **Implicit COMDAT.** 0x00956380 now emits `??1CCreatureAction_CastWillLightningSpell@@UAE@XZ` as an implicit COMDAT from the shared header (the same as other includers). vland graded it RELOCATION_MATCH / PASS.
- **Decision needed: q6449, ConstantFPS 0x01375550.** Four landed rows still bind stand-ins:
  - 0x0057CB7E, 0x0075AEC0, 0x008C6DB0 bind `?g_framesPerSecond@@3JA`;
  - 0x006D8580 binds `?g_FramesPerSecond@@3JA`.

  The shared spelling is the *protected* static `?ConstantFPS@CMainGameComponent@@1JA` (fable_game.h). A row can reach it only through the inline accessor `CMainGameComponent::GetConstantFPS()`, as the landed naming-wave-18 rows do. Measured with `FABLE_RETAIL_CGAMECOMPONENT` + `_CMAINGAMECOMPONENT` + `_SCOPED_PTR` + fable_game.h:

  | Row | Grade | What differs |
  |---|---|---|
  | 0x0057CB7E | DIFFER(572v567) | The ConstantFPS load is hoisted ahead of the other calls in the expression and spilled to the frame; retail has `fidiv [01375550h]` in place after them. |
  | 0x006D8580 | DIFFER(346v330) | Same hoist and spill; retail has `fild [01375550h]` in place. |
  | 0x0075AEC0 | DIFFER(306v306) | The ConstantFPS `fild` itself matches. One later FPU sequence (`fld [esp+54h]; fmul [esp+0Ch]; faddp`) is scheduled after another `faddp`. Cause not isolated. |
  | 0x008C6DB0 | DIFFER(1088v1090) | The `(float)(long)(float)` conversion around the read is scheduled differently (retail `fild; fstp [esp+10h]; fxch; fmul [esp+10h]`). Cause not isolated. |

  The friend-or-accessor question below explains only the first two rows. The last two may have a separate cause, such as fable_game.h changing another inline or the accessor's expression tree.
  Moving the include above the row's `#pragma optimize` did not help. For 0x0057CB7E / 0x006D8580, direct `CMainGameComponent::ConstantFPS` is illegal from these classes, because it is protected and the `1` in the mangled name encodes that access. A real fix would need retail evidence for a friend or for a different accessor, so the rows stay blocked. Drafts: `work/<addr>.cpp`. Locks are released.
- **Rejected claim.** q6447 claimed the PAV spelling, but that is the landed symbol of 0x0040DCE0. The PBV instance `??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z` was applied to 0x00436380 instead (REJECTED.tsv + RENAMES.tsv).

## Pick-any lever (CTCVillage TU)

In the CTCVillage-TU rows (0x0065B2C4, 0x0065B349, 0x0065BFFA, 0x0065C45A), place `#pragma optimize("s", on)` / `("y", off)` *after* the shared includes, and define FABLE_RETAIL_STLP_BITSET_TEST_DECL. Header-inline COMDATs then stay byte-identical to their owners:
- list iterator base 0x0041C780 / 0x0041C7C0 / 0x0041C7A0;
- bitset::test 0x00410DE0.

With the pragma before the includes, the ??9 copy was 18 vs 22 bytes and bitset::test was 31 vs 63. Each was checked with cmpbody against the owner object.

## Visible-body explicit specialisation

When a row needs the PBV `IS_THING_ALIVE<const CThing*>` body in the same TU (0x0064A1AE, 0x0057D5A0), use an inline `__declspec(noinline)` explicit specialisation with the retail body:
`if(thing!=0){if(!(reinterpret_cast<const unsigned char*>(thing)[0x91]&1))return true;}return false;`
A declaration alone gave DIFFER(301v301). The specialisation's COMDAT is byte-identical to the owner 0x00436380.

## Descriptive names

None made up this round: every spelling applied is from Ego_r.pdb / FableWin.pdb.

## Entries this round (after the first context compaction)

- **q6513 (NullString 0x0143E8EC): blocked, 1 row left.**
  - 22 of the 23 REFS rows already bind `?NullString@CScriptThing@@2VCCharString@@A`.
  - The remaining row, 0x0122AE60, is NullString's dynamic atexit destructor (`mov ecx,0x0143E8EC; jmp ~CCharString`). It is landed as the row-local AutoTinyThunk stand-in, which binds `?g_AutoTinyThunkObject_0143e8ec@@3UAutoTinyThunkTarget@@A`.
  - 296 landed rows use that stand-in shape. Fixing them is a TU-emitter decision: define the object in the TU, as `_$E1` 0x01224C50 does, and grade the compiler-made `??__F` thunk. It is the same kind of question as the `??_G` rows under q4411.
- **q6522 0x00CF0C00:** self rename onto the shared header's existing ENTRANCEGUARD opt-in; MATCH.
- **q6523 0x00E53CB0 / q6524 0x00E362B0:** self renames off the propagated global `?GetRandomSpeech@CAC_Owner@@QAE?AVCCharString@@XZ`. In HEAD that one name was defined by three rows: 0x00E53CB0, 0x00E362B0 and 0x00E2E440. Their callers already bind the PDB spellings.
- **q6532 0x00E2E440 (not reached; queue order puts q6530 first).**
  - A prechecked, ungraded draft is in `work/00e2e440.cpp`: CV_AmbushScamScript::CAS_Trickster::GetRandomSpeech, `% 5`, generated from the q6524 row.
  - It is the third copy of the same global name. Until it lands, the old global spelling stays defined by 0x00E2E440 alone, so landing q6523 / q6524 first creates no split.
- **q6530 (CTyler::CleanupAfterFight 0x00E83680): open, not started.**
  - Its family is the self row plus 0x00E83620 (CTyler::OnPredicateFail, which binds the global `?CleanupAfterFight@CTyler@@QAEXXZ`). Both need whole-row ports onto `engine/CV_FisticuffsClubScript.h`, because the self row models its CScriptThing / script-interface vtables locally (138 lines).
  - I handed back at this entry boundary for context. Locks on 0x00E83680 / 0x00E83620 / 0x00E87030 still read NF196, because the coordinator pre-assigned them to my slice; I have not edited those rows.

## Rule slip

- I recorded q6521 as done-already from table and source evidence while the q6513 census was still running in the background. q6521's terminal state was therefore written before q6513's. No compile overlapped, and q6513 was finished right after.
