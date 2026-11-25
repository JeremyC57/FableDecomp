# NF433 hand-back (2026-11-25): CGameScriptInterface script API, round 2 (NF431's family)

One ACCEPT batch, `batches/nf433_cgsi_b1`, with 28 rows:
- 24 of NF431's 80 `body_port` rows, ported onto engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) as the real RTTI-placed `CGameScriptInterface::<Method>`;
- the 4 CWorld accessor debt rows (0x0088F450 0x0088F950 0x0088E380 0x0088F460), respelled onto FableWin's inline CWorld accessors.

No HOLD batch. The other 56 body_port rows, plus the 8 t1_caller_respell and 5 tc_query rows, are residue (`RESIDUE.tsv`, `T3.txt`).

| batch | rows | grades HEAD -> new |
|---|---:|---|
| nf433_cgsi_b1 | 28 | 10 MATCH -> MATCH, 18 RELOCATION_MATCH -> RELOCATION_MATCH |

## Rows by shape
- **Tail jumps / forwards (6):**
  - 006e7290 EndLetterBox;
  - 008907c0 RemoveConversation;
  - 00892d70 SetCreatureGeneratorsEnabledDuringScript;
  - 0088e3d0 MiniMapAllowRouteBetweenRegions (CWorld slot 12 GetWorldMap);
  - 00891c60 DisplayMiniGameInfo (CPlayerGui::GetQuestInfo);
  - 0088fcc0 GiveHeroAbility.
- **CScriptThing -> GetPThing -> IS_THING_ALIVE / type checks (10):**
  - physical-thing bit setters 00890e80 / 00890ec0 / 00890f00;
  - GetHealth 00890400;
  - ModifyThingHealth 00890430 (virtual +0xEC);
  - state groups 0088f6b0 / 0088f6e0;
  - 00890220 IsEntityWieldingMeleeWeapon;
  - 00890280 IsEntityAbleToAttack;
  - 00890690 EntityAttachToScript (inline SetSerialisationSectionName).
- **Named CCharString local, then the virtual EntitySetStategroupEnabled (+0x938) (3):** 0088f7d0, 0088f810, 0088f850.
  - Retail builds the string BEFORE the arguments, so it is a named local, not a temporary (a temporary graded DIFFER 55v59).
  - 0088f810 had a baked `kName_0125fd40` constant at HEAD; it now spells the literal.
- **Faction pair (3):** 00890870 / 008908c0 / 00890910.
  - Each looks up GetPFaction twice and relates both ways.
  - FableWin's DoErrorMessage blocks are debug-only and are left out.
- **Display engine (2):** 0088e4d0 IsScreenFadingOut, 008907e0 FadeScreenOutUntilNextCallToFadeScreenIn (virtual +0x5D4, then an inline lock flag).
- **CWorld accessors (4):** 0088f450 uses the existing BED_USE_PASS28 IsHeroSleepingEnabled. The other three use a new opt-in.

Names come from two sources each:
- the self: its RTTI vftable placement, plus the Ego_r / FableWin name sets;
- every callee and its argument order: FableWin's /Od body (`tools/fw.py`), checked against retail's call / jmp target (`tools/rdis.py`).

## Header opt-ins (add-only; base = the current repo headers; originals in `orig/hdr_*.h`)
NF431's five opt-ins landed in this tree during the session, at 15:15. Every `orig/hdr_*.h` was re-compared with rebuild/include before assembly, and none is stale.

| header | opt-in | what it adds |
|---|---|---|
| CDisplayEngine.h | _ENDLETTERBOXMODE | `void EndLetterBoxMode()`, 0x004349D0 |
| CDisplayEngine.h | _ISSCREENFADEOUTOPENING | inline `ScreenFadeOutInfo.IsOpening()` |
| CDisplayEngine.h | _SETSCREENFADEOUTLOCKFLAG | inline, byte +0xD8 |
| CFaction.h | _SCRIPTRELATIONS | SetAsAlliedWithFaction / SetAsNeutralToFaction / SetAsEnemyOfFaction, 0x006B94E0 / 9510 / 9530 |
| CPlayerGui.h | _GETQUESTINFO | inline `*PDrawQuestInfo` |
| CQuestManager.h | _CREATUREGENERATIONDURINGSCRIPT | 0x004B3810 |
| CScriptConversationManager.h | _REMOVECONVERSATION | 0x006E5990 |
| CThingAICreature.h | _SETALLSTATEGROUPSENABLED | 0x008302E0 |
| CThingPhysical.h | _SCRIPTHITSETTERS | three inline bit setters, FableWin.pdb only plus their /Od bodies |
| CThingPlayerCreature.h | _LEARNABILITY | 0x006AC690 |
| CWorld.h | _SCRIPTCONVERSATIONMANAGER | inline accessor, +0x7C |
| CWorld.h | _GETFACTIONMANAGER | non-const inline accessor, +0x54 |
| CWorld.h | _SCRIPTFLAGACCESSORS | IsSavingEnabled / SetDontPopulateNextLoadedRegion / SetExperienceSpendingEnabled |
| CWorldMap.h | _SETREGIONSASCONNECTED | 0x004FC970 |
| NCreatureMode.h | _ISABLETOMOVE | 0x0085F3E0 |

hdrcheck2 (`evidence/hdrcheck.txt`): every macro already in the repo copies gives the same text. The only DIFF lines are under the new opt-in macros themselves.

## Gates (batch nf433_cgsi_b1; final run `bA`, 28 rows, one work path)
- **vland HEAD / new / mutant:**
  - grade kept 28/28, tests PASS 28/28;
  - 0 LITERAL lines;
  - new has 0 WARN lines (HEAD: 14 RELOC_NAME, 18 SHARED_HEADER_AVAILABLE);
  - leaf = the full mangled self.
- **Mutation control:** every one of the 28 mutants grades DIFFER and the test FAILs. None is a TCC or link fail.
- **name_gate --test:** 28/28 PASS. There are 2 DEBT lines on existing PClass split data (CQuestManager 0x013B89FC, CPlayerGui 0x013B8790).
- **objcmp:** 28/28 SELF_EQ.
  - The differing symbol pairs are callee respells to PDB names at the same targets.
  - The extra COMDATs are header-inline pick-any copies only, and no `??_7` / `??_R` is emitted.
- **pk_all:**
  - NOADDR copies (all leaf=n, reach=n: unreferenced inline copies): HEAD 5 (00892d70's CQuestManager Get / ??D), new 49;
  - 0 NEQ, 0 reach flips.
  - IS_THING_ALIVE<CThing*> / <const CThing*> copies are EQ.
- **/FORCE link (`evidence/linkcheck.txt`):**
  - duplicates: HEAD 0, new 0, control 1;
  - unresolved: 30 -> 30, with stand-in spellings swapped for PDB spellings.
  - Definers census (`evidence/census.txt`): no other landed definer of any new self. The 2 hits are comment mentions.
- **apply_table --dry:**
  - 9 adds and 14 deletes, 0 misses;
  - multi-target symbols 1 -> 1, none new.
  - 3 stand-in lines are kept (`evidence/TABLE_KEPT.tsv`) because other landed sources still spell them.
- **reserved_check** (RESERVED_REFS=""): 0.
- **equiv_check**, copy against copy (old = a copy of rebuild/include taken after NF431's landing, new = old plus hdr/): **0 of 57,160** landed rows differ (`evidence/equiv.log`, `equiv.tsv` is empty).
- **In-class census** (NF324 inclass_census.py, `evidence/inclass_census.txt`): 76 hits, all in other rows' self-contained TESTS (local CGameScriptInterface models); 0 in landed sources.
- **No `??_7` / `??_R`:** a direct symbol scan of all 56 new objects (rows + tests) finds none.
- **Mutants:** all 28 mutant test exes were built (`work/wd_bAm`), so every FAIL is a behaviour fail. The mutant sources are in `mut_b1/` at the agent root, not in the batch.
- **TYPED_MEMBERS arms** of the two new CWorld accessors (GetScriptConversationManager, GetFactionManager): a throwaway compile with FABLE_RETAIL_CWORLD_TYPED_MEMBERS (+ SCOPED_PTR) compiles, and 0x008907C0 still grades RELOCATION_MATCH (`probe/`).
- **Final rechecks, after equiv:** elig.sh on all 28 (`evidence/elig_final.tsv`: all locked NF433, 0 reserved, no Account B claim, one Account A naming-queue claim); every `orig/hdr_*` equals rebuild/include; apply_table --dry and reserved_check re-run, unchanged.
- **bytescan:** 0 hits in new files. The 4 hits are in HEAD copies (orig/, head/payload.json), which are already in the repo.
- **Tests:** they link the row object and call it qualified (`p->CGameScriptInterface::M`). Virtual callees are planted in raw vftables as `__fastcall` functions.
  - The 4 CWorld rows keep their landed HEAD tests unchanged.
  - EndLetterBox is protected, so its test calls it through a derived access helper that is never instantiated.

## T3 lines resolved (`evidence/T3_RESOLVED.tsv`, 23)
- nf324 port lines for 18 rows;
- 0x0088F810's nf320 / nf322 / nF325 / nf326 port lines;
- the nf431 family line "CWorld accessors | debt".

Release each one only for its listed address.

## Names resting on one PDB plus the /Od body
FableWin.pdb only (Ego_r has them inline / not listed), each backed by the FableWin /Od body matching the retail bytes in place:
- CThingPhysical::SetAsNegateAllHits / SetAsEvadeAllHits / SetAsAbleToBeEngagedInCombat;
- CDisplayEngine::IsScreenFadeOutOpening, SetScreenFadeOutLockFlag;
- CPlayerGui::GetQuestInfo;
- CWorld::GetScriptConversationManager, the non-const GetFactionManager, IsSavingEnabled, SetExperienceSpendingEnabled.

## NAME_FIXES.tsv (8 lines; for the coordinator to respell callers)
0x004349D0 EndLetterBoxMode, 0x004B3810 (delete the SetCreatureGeneratorState table line), 0x00654593 DisplayMiniGameInfo (enum mangling), 0x004FC970, 0x006A15A0, 0x006AC690, 0x0085F3E0 (stand-in spellings other landed callers still use), 0x00A36FE0 CClassFactory::Validate.

## Rulings
None. The 0x00A36FE0 question is settled by GOTCHAS nf380 (retail has no ICF): it is CClassFactory::Validate, and 0x006E72A0 is residue (research, portable next round).

## Judgment calls
- 0x0088F810 was on four T3 port lines (nf320, nf322, nF325, nf326). All four ask for exactly this port, so the row was taken and the lines are listed as resolved.
- The 4 CWorld rows were taken over from NF431. They were unlocked (no owner file) and are now locked NF433.
- TABLE_KEPT: ?SetCreatureGeneratorState@CQuestManager is kept only because the 0x004B3810 row's comment names it. NAME_FIXES asks to delete it.
- The repo headers changed during the session: NF431's landing at 15:15, then CEnginePrimitiveHandle.h / NPlayerGui_* at 15:49. The final vland run started after 15:49.
  - CPlayerGui users: 00891c60 defines FABLE_RETAIL_CENGINEPRIMITIVEHANDLE.
  - The lander's recheck covers this.

## Resume (next agent)
- Tools are in `tools/`, NF431's set repointed here, plus:
  - b2.sh: a dossier without HEAD;
  - rdis.py: retail disassembly at a VA;
  - rstr.py: a retail string;
  - hins.py: an anchored header insert;
  - census.py: the definers census;
  - elig.sh: lock / reserved / claim / T3 per address;
  - lock.sh;
  - linkcheck.sh, whose control is now a renamed object copy (LNK4006 counted).
- Next units, in order:
  1. The not_attempted small rows in RESIDUE.tsv, using the shapes above: 0088fc60 (GetMainPlayer, then IS_THING_ALIVE<CThingPlayerCreature>), 00891490, 00892810, 00892f80, 008902e0.
  2. The tc_family: one TC row first (0089a2d0 IsLockPickable, a PeekTC<const CTCDoor> expansion with lower_bound at 0x004365B0), with pk on its PeekTC copy, then its siblings.
  3. 00891c80 together with 0x006DF480 (the nf295 line).
- One recipe per row: `tools/b2.sh <a>`, then a spec block (D / I / E / M / body), `tools/mkrow.py`, `tools/qall.sh`, a tspec, `tools/mktest.py`, `tools/run431.sh`, ng / objcmp / pk / linkcheck / census, and `tools/assemble.py`. Then drop the unchanged CThing.h / CGameScriptInterface.h from the batch's hdr/.
