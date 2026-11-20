# NF122 notes (round 10, PRIORITY slice 6 remainder; 23 queue lines over 17 addresses) -- early hand-back

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log / selfcheck.log.
- Entries (23): 9 done (q4266 q4490 q4985 q5010 q6228 q6229 q6325 q6929 q6936), 1 done-already (q6225), 0 rejected,
  5 blocked: port (q6226 q6227 q6278 q6417 q6471: the binders done, one or two rows left each), 0 deferred,
  8 open (the CTCHeroExperience family q4486 q4762 q4764 q4765 q4766; q5107 / q5151; q5137, probed only).
- Rows: 54 ported (rows/), 212 not-a-binder (probe.tsv / probe_q4490.tsv), 4 blocked (0x006E11F0, 0x008F7BD0, 0x00CFD6C0, 0x00D6AEA0).
  RENAMES: 7 self renames (s1), 57 caller respellings (s2).
- recheck_final.log: one vland --recheck-landed run over the 54 rows/ rows: 54 RELOCATION_MATCH, every test PASS. selfcheck.log: 0 problems.
- Final-log WARNs reviewed: STANDIN_LEAF / RELOC_NAME on 0x006B83F0 (its catalog `_Cons_val<..>` label changes at landing, RENAMES s1);
  RELOC_NAME 0x008818A0 (catalog/UNMATCHED label wrong: NAME_FIXES); the rest are pre-existing RELOC_NAME / SHARED_HEADER_AVAILABLE lines
  on local models the landed rows already had. One GENERIC_STRUCT on 0x00946630 (pre-existing owner-rule stand-in). No RELOC_CONFLICT.
- Header opt-ins: 4 overlay headers, 4 opt-ins (hdr/README.md): NScreenEffect_CScreenEffectPriorityStack.h DELETESCREENFILTERENTRY,
  CGameTimeManager.h ISDAY, CTCRegionFollower.h FOLLOWERS_ONLY (partial mode), CTCDParticleEmitter.h METHODS_ONLY (partial mode).

## Probe
work/probe.tsv: every REFS row of the 10 not-started entries compiled once (284 lines, all RELOCATION_MATCH), with the symbol each binds.
work/probe_q4490.tsv: the 10 REFS rows of q4490. It found 0x00946630 binding 0x006BB2C0 as ?Check_006bb2c0@M@@QAE_NXZ (not in WORKLIST).

## Claims settled
- q6936: 0x006B83F0's callee 0x008818A0 is NScreenEffect::CScreenEffectPriorityStack::DeleteScreenFilterEntry(void*, float) (Ego_r
  run DeleteRadialBlurEntry / DeleteDisplacementEntry / DeleteScreenFilterEntry = retail 0x00881820 / 0x00881860 / 0x008818A0; argument
  order (handle, time) as pushed). 0x0079D110's def member at +0x188 is the fade time: Ego_r CSpecialAbilitiesBulletTimeDef
  ScreenFilterFadeOutSecs (float; Ego_r +424, retail +0x188 after the 4-byte-smaller STLport vectors).
- q4490: 0x006BB2C0 = IsDay: Ego_r member order (IsDaySounds / IsNightTime / IsEvening / IsDawn / IsDay / SetAsUsingVillageTime /
  GetTimeAs24HourClock) matches retail 0x006BB1E0 / 0x006BB210 / .. / 0x006BB2C0 / 0x006BB2F0 / 0x006BB300. Found in passing (not
  applied, one source): the catalog calls 0x006BB1E0 "IsDay" too; by that order it is IsDaySounds, and 0x006BB2F0 (catalog
  CXMVPlaybackInfo::CXMVPlaybackInfo) is SetAsUsingVillageTime.
- q6278 / q6417 / q6471: FableWin.pdb has ??4?$CCountedPointer@VCScriptThing@@ (0x027C7B70); Ego_r has only the CDiskFileWin32
  instance (0x00A35EE0), so 0x00704580 may be ICF-folded. No landed row references the address with another T, so nothing is forced
  across a fold (GOTCHAS nf104): the self row takes the spelling all its landed binders use. Not rejected.
- q6325: all three respelled binders reach 0x006AEDC0 through interface 0x7A = TCI_REGION_FOLLOWER (CTCRegionFollower.h RTTI note).

## Partial header modes (round-9 rule)
CTCRegionFollower.h FOLLOWERS_ONLY and CTCDParticleEmitter.h METHODS_ONLY declare the class without its CTCBase base, for rows that
still model CThing / ETCInterfaceType locally (they cannot include CTCBase.h). Each lets a row drop a local stand-in class
(CTCPlayerCharacterLinks, CTCFollowerList, CTCRegionFollowerMethods, EmitterMethods, CParticleEmitterManager::CreateEmitter).

## Blocked rows (blocked: port; none attempted, each needs local models moved first)
- 0x006E11F0 (q6226 self row): free stand-in Create inside namespace X_006e11f0 on local C3DVector, CRightHandedSet, CCharString,
  CBaseClassNonCopyable, CTCParamsPhysicsBase, CVectorMap, CParticleCreationParameters, CTCDParticleEmitter, CThing, CWorldMap views.
  Every landed binder now binds the PDB spelling, so its stand-in self leaf is a NAME_FIXES item, not a split.
- 0x008F7BD0 (q6227): whole row in namespace NFoot008f7bd0 (local C3DVector, CCharString, vector, CParticleUpdateParameters,
  CTCDParticleEmitter, CThing, CThingCreatureBase).
- 0x00CFD6C0 / 0x00D6AEA0 (q6278 group): a non-template local CCountedPointer whose Data is a separate pimpl model (its slot 10
  GetHeading is spelled as a non-virtual inline on the local CScriptThing); needs engine/CScriptThing.h + CCountedPointer.h.

## Casts / shapes worth a look at landing
- 0x0097B780: the combatant local is now `CThingCreatureBase* = (CThingCreatureBase*)PBrain->PCreature`, the same cast the row
  already applies twice to that member (its local CAIBrain types PCreature as CThing*).
- 0x0080D0B0: `CParticleEmitterManager::Get()` (0x00AEAAF0) is still called first; its result is now unused (the emitter create is the
  static CTCDParticleEmitter::Create), and the bytes match.
- tools/dereg.py and tools/gnwp.py also rewrote the call text inside a few `// Shape:` / `// Grade:` comments (pointer -> object), so the
  comments still describe the code.

## Not changed (found in passing)
- 0x006CB620 / 0x006CB840 / 0x009804A0 declare the old GetNearbyWanderPosition spelling on their local models but never call it
  (no binding).
- 0x006ED510: Ego_r ?DeregisterAttackingCombatant@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z; its binders 0x006ED9C0 and 0x006ED9E0
  spell it two other ways and must move together (NAME_FIXES).

## Descriptive names
None new. Library spellings: none. The kept pre-existing descriptive names: g_dayStart_0125d624 / g_dayEnd_0125d628 (0x006BB2C0).

## Funclets
No EH funclet row includes a changed row (grep of rebuild/src/compiled/01/21 for the changed files' names found none). Self renames
that move parent files: 0x006B83F0, 0x0088FFB0, 0x006BB2C0, 0x004A9EF0, 0x006CB6E0, 0x00704580, 0x006ED9C0 (none has funclets).

## Coordinator request (folded in)
q4985 row 0x006A1E00: IsCarryingThing respelled to (const CThing&) const with `IsCarryingThing(*thing)` (NF121 recipe), closing the
0x006A1060 split.

## Flags lines
None (no row needed flags its build entry lacks; the rows that gained a `// Flags: none` line had no CompilerFlags).

## Inferred lines
None new.

## Split gate (reasoned; gate.py needs the coordinator's landing directory and pair files, so it was not run here)
- 0x00704580 (q6278 group): the self leaf moves from the stand-in @CCountedPtr_assign@12 (bound by no row) to the spelling 20 of its
  24 referencing rows already bind; the two blocked rows 0x00CFD6C0 / 0x00D6AEA0 keep the non-template spelling they had. The
  referencing-row spelling set shrinks from 3 to 2 (the CScriptThingImpl spelling goes); nothing is created or grown. The gate counts
  referencing rows only, never the target's self row (NF118 lesson), so the self rename has no gate effect.
- Every other renamed target ends with all its referencing rows on one spelling (0x006B83F0, 0x0088FFB0, 0x006BB2C0, 0x004A9EF0,
  0x006CB6E0, 0x006ED9C0), or only shrinks an existing split (0x006A1060, 0x006AEBA0, 0x006E0480, 0x006E0BE0, 0x006E11F0 callers,
  0x0044C6B0 / 0x009AD410 / 0x0049C780 / 0x006A7960, which move to spellings other rows already bind).
- Section 2 was checked against an unfiltered before/after binding diff of all 54 rows (work/pdall.log, tools/pdiff.sh).
- The section-1 old_leaf values were checked against the defined symbols of the HEAD-compiled objects (pH): all seven agree.

## Rule slips
- 0x0089F9E0 was edited before it was claimed (it was not in rows.txt); the lock was free and was taken afterwards (claim.sh, NF122).
- 0x0079D110's lock said `released`; I overwrote the owner file with NF122 instead of treating it as a fresh claim.
- 0x0088FFB0: two of three forms were SRC_FAIL (missing FABLE_RETAIL_CGAMEDEFINITIONMANAGER / FABLE_RETAIL_CWORLD prerequisites) before
  the third matched; within the 3-form budget.
- The four blocked rows are "port, not attempted" (port/authoring-sized), not blocked after 3 failed forms.
- GOTCHAS.md and EXTRA_RULES.md were read only in part (the recent GOTCHAS lines, the named nf104 / nf96a lines).
