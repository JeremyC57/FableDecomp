# NF238 notes (2026-11-22, round 12): the nf236 follow-ups

Base: the repo tree with nf236 landed. NF237's hand-back was being landed during my run (reloc_bindings.tsv, build_candidates.ps1,
the manifest changed); before hand-back I checked that none of my rows' sources, tests or the five headers I build on changed after I
claimed the locks (mtime check, nothing newer). Rebase the rows only if the nf237 landing touches them after this.

Entries: q12668 partly (2 of 27 lines done, 25 carried with NF236's blockers), q12669 half (0x006F2C00 done, 0x01296F14 deferred to
NF237's queue), q12670 done, q12671 done, q12672 not started, q12659 not started.
Changed rows: 9 (rows.txt): 9 sources, 5 new or changed tests. Header overlay: one add-only opt-in (hdr/README.md).
Locks: claimed with plain mkdir + `NF238` for all nine rows (six in work/locks.log; 0x006F3530, 0x004C7E30, 0x007D7C10 claimed later
the same way); none was held. Released at hand-back: no (the coordinator
releases at landing). NF239 was running beside me (its cl.exe in the process list). I read its BRIEF, rows.txt and hdr/ (read-only): none of its rows is one of
mine, and its overlay holds only two NPlayerGui headers (not NCreatureMode_CModeSitFloor.h).
Off limits respected: NF237 rows / queue lines, the 25 Codex pass 28 rows, 0x00B4D370 / 0x00BD3070, auth2* / c2t*.
Tools: NF236's set, re-pathed, plus tools/gpdm.py (the 0x01436E28 respell), tools/vl.sh (vland from the repo cwd, work dir under
wd/, cleaned after), tools/tcc.py (compile a test the way behaviour_of does and print cl's errors), tools/incchk.py (include walk).
Flags lines added: none. Descriptive names added: none (the descriptive g_pEngineDefinitionManager is replaced by the PDB name).

## Entry 1 (q12668): the 27 held p27 lines (29 minus q12669's two) -- 2 done, 25 carried
### 0x01436E28 ?GPDefinitionManager@NEngineInternal@@3PBVCDefinitionManager@@B -- done, 2 -> 1
- Name: the full spelling is in Ego_r.pdb, FableWin.pdb and Ego_d.pdb publics. `PBV...B` is a non-const pointer to a const manager,
  which is why CEngine::Initialise can store it.
- The p27 witness 0x00B412A0 declares `namespace NEngineInternal { extern const CDefinitionManager* GPDefinitionManager; }`. The five
  old binders take that declaration verbatim: 0x00B26360 (CEngine::Initialise, the store and six GetDef calls), 0x00B40260, 0x00B40BD0,
  0x00B625E0 and 0x00BEAB40. Every use is a const member (GetDef, GetDefClassIndexFromGlobalIndex).
- Census: absrefs finds 25 sites in 7 rows plus 3 sites at 0x00B40A41.. outside any landed row. 0x00B687D0 (BuildThemes) is a
  non-faithful candidate: it compiles DIFFER 327 v 1476 and binds nothing at 0x01436E28 (bindat2), so it carries no spelling.
- Each row: sc.sh HEAD v new gives the same grade, and the only change is the UND data symbol (work/sc_*.log).
- Tests: 0x00B40260's test defined the old global, so its copy defines the new one (rows/00b40260_test.cpp). 0x00BEAB40's test is a
  host model with a static of its own and does not link the row, so it is unchanged. The other three tests do not name the global.
- The "descriptive (no PDB data symbol)" comments in 0x00B26360 / 0x00B625E0 / 0x00BEAB40 were corrected by hand.
### 0x00740040 ??0CModeSitBench@NCreatureMode@@QAE@AAVCThing@@@Z -- done, 3 -> 2
- 0x00742790 (CModeSitBench::Alloc) is ported off its row-local `struct CModeSitBench { _body[0x38]; CModeSitBench(CThing*); }` onto
  engine/NCreatureMode_CModeSitFloor.h. It uses the existing PASS27 class plus the new opt-in CMODESITBENCH_ALLOC, in the landed
  sibling shape 0x00742460 (CModeBasic::Alloc).
- Its self leaf goes from the free-function stand-in to the PDB `?Alloc@...SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z`
  (three PDBs), the spelling its only binder 0x0073B130 already binds, so 0x00742790 is 2 -> 1.
- The ctor binding becomes the PDB one (three PDBs; 0x00938380 binds it).
- The table line `??0CClass_00740040@@...` was stale at HEAD: its witness already bound `??0CModeSitBench@@QAE@PAVCThing@@@Z`.
- The self row 0x00740040 still emits `??0CModeSitBench@@QAE@PAVCThing@@@Z` as its own leaf. It is a 116-byte ctor with local
  CModeSitBench / CModeSitBase / CAnimFilterOfGroup / CCharString / CCRC models and it emits the class vtable, so a leaf-only respell
  is not in-place-safe. A PASS27 header port is needed (NAME_FIXES). That leaves 0x00740040 at 2 spellings, not 1.
- New header test (rows/00742790_test.cpp): links the row. The ctor is stubbed through a test-only /alternatename (thiscall -> __fastcall
  stub, ret 4) and operator new is replaced. Case 1: a successful allocation is 0x38 bytes, the ctor runs once with the owner, and the
  block is returned. Case 2: a failed allocation returns null and the ctor is not called.
### The other 25 lines -- 23 carried with NF236's blockers, 2 open (not attempted)
NF236 NOTES entry 5 gives each line's blocker, and none changed. The lines:
- Header ports of stand-in classes: 0x005BFA44, 0x00A3AA40, 0x009F32A0, 0x00700820, 0x008ED1F0, 0x00780400, 0x00592BCE, 0x00A97180,
  0x00660787, 0x005CBDFA, 0x00685C00, 0x008F00F0, 0x006B0C10, 0x0098E770, 0x00B92450, 0x0091C220, 0x00843A80.
- Access paths: 0x013D2838, 0x013B8774, 0x0126DEAC.
- Second-source gap: 0x00930D20.
- Body claim: 0x00B4B830.
- Re-authoring: 0x0143782C.
- Not attempted by NF236 (no blocker recorded), open: 0x0143B8D8 (private static, 7 binders; worth probing next) and 0x006A1620
  (9 rows, 6 spellings).
Observations from the census (none acted on):
- 0x006B0C10: 0x00726750 is a 3-line free-function stand-in (`CTCPhysicsNavigator_SetPosition`, a store at +0x89). Its port is a
  CTCPhysicsNavigator.h self rename with vtable-slot binders: authoring-sized.
- 0x0091C220: 0x0091C640 models CAIStateGroupBase / CTCNymph / CAIStateGroup_NymphAttack locally, and all three have headers.
- 0x006A1620: nine rows, CGameScriptInterface stand-ins (Entity, CThingNative). 0x005C2DF5 is an NF237 queue address.

## Entry 2 (q12669)
### 0x006F2C00 -- done, 2 -> 1
- 0x006F3530 (std::for_each) has no direct caller and no absolute reference (callers.py, absrefs.py). The witness
  AssignAttackersToRing 0x006EBF70 expands its for_each in place, so 0x006F3530 is the TU's out-of-line COMDAT copy.
- Its stand-ins were wrong. `CRotateThing` is a real, different PDB functor: `??RCRotateThing@@QBEXAAV?$CIntelligentPointer@VCThing@@@@@Z`
  over a list<CIntelligentPointer<CThing> > (FableWin / Ego_d for_each instantiations). So the respell is not of a folded alias
  (REJECTED.tsv).
- FableWin / Ego_d both have for_each over vector<CProng> with CGetBestFreeProngForAttacker. Their Dinkumware iterator is CProng* in
  retail's STLport. The sizes agree: functor 0x20 (`rep movsd` of 8 dwords), prong 0x1C (asserted in CDefendingCombatantInfo.h).
- Ported onto engine/CGetBestFreeProngForAttacker.h (PASS26) + CDefendingCombatantInfo.h + stlport/fable_algorithm.h FOR_EACH_BODY,
  with an explicit instantiation. RELOCATION_MATCH. The call binds `??RCGetBestFreeProngForAttacker@?A0x<TU hash>@@QAEXAAVCProng@...`,
  which is the witness's spelling with the hash stripped. The self row 0x006F2C00 defines
  ??RCGetBestFreeProngForAttacker@?A0xc371e781@@QAEXAAVCProng@CDefendingCombatantInfo@@@Z (objdump of its compiled object): the same name.
  Its `// Leaf:` line carries a full hash (`?A0xd8b789b9`), so q.py warns "matches no symbol" and grades the largest function
  (RELOCATION_MATCH). EXTRA_RULES says anonymous-namespace leaves end at `?A0x`; a one-line leaf fix for that row (NAME_FIXES).
- Form 1 hung cl.exe: CDefendingCombatantInfo.h alone does not compile, because it needs the CThing / fable_string prelude. The
  probe is in LESSONS.
- The self leaf changes (the for_each template arguments). It is free because nothing references the address.
- New self-contained header test: the same header template and functor, with operator() stubbed in the test's own anonymous namespace.
  The row object cannot be linked against a stub, because the stub's ?A0x name is per TU. Cases: an empty range; three prongs visited
  in order by reference; the returned copy carries PBestProng while the caller's functor stays untouched.
### 0x01296F14 -- deferred (NF237 overlap)
- Its old binders are 0x0096DB60 (the ctor, the vtable emitter), 0x0096DDB0 and 0x0096E510. They model
  CCreatureAction_ScreamerIntoAggressive at global scope.
- Moving that class into the anonymous namespace re-spells every slot function the vtable binds, and NF237's queue is renaming those
  slot functions now (0x0096DBA0 IsToFaceTargetAtStart, 0x0096DBB0 IsToFaceTarget, 0x0096E3F0 ??_G).
- Do it after NF237's landing, as one unit with those slots (decision 1).

## Entry 3 (q12670): 0x004C7E30 CThing::IsLocal -- done, 2 -> 1
Body proof (two sources and more):
- Ego_r thing.obj order: GetTypeName 0x004C9820 (27 bytes), then IsLocal 0x004C9840 (27 bytes). FableWin thing.obj has the same pair.
- Retail: GetTypeName at 0x004C7E10 (engine/CThing.h) returns `&record[type].Name` (lea +0x10 of the 20-byte thing-type record).
  0x004C7E30 sits 0x20 after it and returns the byte at +9 of the same record (`mov al, [edx+eax*4+9]`, a bool).
- Both landed binders (0x00637FC0, 0x00637A10) already call ?IsLocal@CThing@@QBE_NXZ: bindat2 on their objects shows exactly that
  symbol at 0x004C7E30 (work/islocal_bind.tsv); absrefs finds no absolute reference.
The change:
- In-place respell of the HEAD local model: same class token, `char GetTypeName()` becomes `bool IsLocal() const`, read through a
  `const bool*`. Bytes unchanged; the symbol diff is the leaf only.
- Catalog / files: RENAMES section 1. vland graded it with the payload name and oracle line patched to CThing::IsLocal.
- New test on engine/CThing.h (FABLE_RETAIL_CTHINGMAPMANAGER_PLACE_PASS27 + _MANAGERACCESS): links the row and defines the private static
  CThing::PCurrentThingManager. Four cases: local, non-local, neighbouring bytes, a flip.
- The vland WARN SHARED_HEADER_AVAILABLE (CThing, CThingManager) is the HEAD local model, as before.

## Entry 4 (q12671): 0x007D7C10 CTCTavernGameSpotTheAddition::GetWinPhrase -- done
Proof:
- The only reference to 0x007D7C10 is vtable 0x0126F814 slot 42 (absrefs).
- In all seven tavern-game vtables, the slot between Farewell (41) and GetBestScore (43) is GetWinPhrase. The base CTCTavernGame
  0x005EA100 is in five of them, and CTCTavernGameCardBlackjack::GetWinPhrase 0x007DA3A0 is in its own (work: vtable read).
  engine/CTCTavernGame.h's slot list agrees: 42 = GetWinPhrase.
- Ego_r.pdb 0x007D4EC0 / FableWin.pdb 0x02753360 ?GetWinPhrase@CTCTavernGameSpotTheAddition@@UAE?AVCWideString@@XZ. It is the only
  CWideString getter of the class not already at slots 43-46.
- Ego_r places it 0x70 after EndRound, and retail 0x007D7C10 sits 0x70 after EndRound 0x007D7BA0.
The change:
- Ported onto engine/CTCTavernGameSpotTheAddition.h (the FindTarget 0x007D7C40 prelude); the local CUserProfileManager model is gone.
  RELOCATION_MATCH; the symbol diff is the leaf only.
- No landed binder: the ctor 0x007D8DA0 emits a one-slot local vtable model (NAME_FIXES).
- The old leaf `?GetAutoSaveFileName@CUserProfileManager@@QAE...` is no longer emitted by any row. GetAutoSaveFileName's real address
  is 0x00406690 (nf236).
- Catalog, module and files: RENAMES section 1.
- New self-contained header test, the NF236 0x004065D0 pattern (row prelude + body, stubbed CWideString ctor / dtor). Two cases.

## Entry 5 (q12672): CEngine ctor + dtor -- not started
- The remaining context was not enough for a full header unit.
- Scope is unchanged from nf235 NOTES entry 3: a CEngine.h opt-in with the 83 overrides in PDB spelling, the typed members, and both
  rows' local models replaced, with the slot census of 0x012A0F3C.
- One more prerequisite I found: engine/CIEngine.h still carries descriptive `VirtualNN_<addr>` names for slots 8, 11, 12 and 13
  (its own header comment). The landed-code naming rule forbids those, so they need PDB names before the CEngine overrides are spelled.

## Entry 6 (q12659): 0x00418DCA -- not started (brief: only with the full 469-includer proof)

## Proof summary
- Recheck: recheck_final.log, one vland --recheck-landed run over all 9 rows. 9 / 9 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT.
  The WARNs are as at HEAD: RELOC_NAME catalog labels for three other NEngineInternal globals in 0x00B26360, and
  SHARED_HEADER_AVAILABLE on 0x004C7E30's in-place model.
- Symbol diffs (sc.sh, HEAD v new): only each row's own leaf, where renamed, and the UND symbol of the respelled callee / global. No
  row defines a new function copy, so pk.py has nothing new to compare.
- Link: 9 HEAD objects and 9 new objects, each linked with 11 co-owners (0x00B412A0 0x00740040 0x0073B130 0x00938380 0x006F2C00
  0x006EBF70 0x00637FC0 0x00637A10 0x00406690 0x007D8DA0 0x007D7C40). 0 LNK2005 / LNK4006 both ways (work/lnk/link.txt).
- Census, per touched address:

  | Address | Before | After | Note |
  |---|---|---|---|
  | 0x01436E28 | 2 | 1 | |
  | 0x00740040 | 3 | 2 | the self leaf remains |
  | 0x00742790 | 2 | 1 | |
  | 0x006F2C00 | 2 | 1 | hash-stripped |
  | 0x006F3530 | 1 | 1 | self only, renamed |
  | 0x004C7E30 | 2 | 1 | |
  | 0x007D7C10 | 1 | 1 | self only, renamed |

  No address grows.
- Header: equivalence over the 21 includers is 0 differing after pin_asserts; assert_names_check (orig v hdr copy): 0 mismatches.
- Includers of changed row sources (EH funclets etc.): none. No landed source or test names any of the nine row files.

## Rule slips
- One `rm -rf $A/eq/old $A/eq/new` on variable paths, before the equivalence copies. The directories did not exist yet, so nothing was
  removed. Every later cleanup went through the guarded tools/clean.py.
- The first vland run used a scratch cwd and failed on import. vland and equiv_check need the repo cwd: I ran them there and checked
  that the repo root listing did not change (work/root_*.txt).
- Two `timeout 600` foreground waits on a hung cl.exe (0x006F3530 form 1) before I diagnosed the include prelude.
- STATUS lines were written per entry, not after every row.

## Decisions for the coordinator
1. 0x01296F14: respell 0x0096DB60 / 0x0096DDB0 / 0x0096E510 into the anonymous namespace only after NF237's ScreamerIntoAggressive
   slot renames land, as one unit. Who takes it?
2. 0x00740040's self row: queue a PASS27 header port (it emits the vtable, so a leaf-only respell is not in-place-safe), or leave it
   at 2 spellings?
3. 0x004C7E30 and 0x007D7C10 rename their catalog entries, modules and files (RENAMES section 1). vland graded them under the new names
   with patched payload / oracle lines, as nf236 did for 0x004065D0.
4. CIEngine.h's VirtualNN_ placeholders: the declarations themselves are `Virtual08_00b27d90`, `Virtual11_00b23cf0`,
   `Virtual12_00b24400` and `Virtual13_00b242d0` (lines 61-82). Queue their PDB names as a prerequisite of the CEngine unit?
