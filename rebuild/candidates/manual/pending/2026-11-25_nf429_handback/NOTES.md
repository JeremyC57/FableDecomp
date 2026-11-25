# NF429 hand-back (2026-11-25): AI state-group family (CAIStateGroup_* and CAIStateGroupBase), one generator per shape

Worked against the repo as found on 2026-11-25. No repo writes. No wineserver -k. I ran no git command myself, but the tracker the brief
asked for (tools/decomp_pipeline/naming_status.py) calls `git rev-parse` internally (run twice, once with --dump-rec for lib425's rec.json).
NF427's tools were copied and repointed (tools/, tools425/); new NF429 tools are named *429*.

## Result
**47 rows ACCEPT in 5 batches**, all locked NF429. 3 of them (c1p) wait on ruling q2. The HOLD batch `hold_g12` has RESULTS.tsv only: the 11 generated-shape holds, never locked. Every other non-accepted row has a reason in T3.txt.

| batch | rows | shape / generator | grade (HEAD = new) | notes |
|---|---:|---|---|---|
| c1 | 29 | G1 CONST: RET_TRUE / RET_FALSE / RET_ZERO / EMPTY / EMPTY_RET4 (gen429.py) | MATCH PASS | 7 empty bodies: no mutation (NF419 r1); 8 objgate `.drectve +libcp` only (STLport include of the CreatureUseCoverBase model; NF318 Q1 / NF427 precedent) |
| c1p | 3 | G1 CONST, same run as c1 (split out after gating) | MATCH PASS | **RULINGS q2**: the UsesSeat chain needs the landed switch FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27 |
| hold_g12 | 11 | RESULTS.tsv only (HOLD) | - | reasons as in T3.txt |
| c2 | 7 | G1 CONST round 2: EMPTY_RETN (`ret 8`), size-mode RET_LONG (`xor eax,eax; inc eax` = `return (EAIStateGroupType)1;`) | MATCH PASS | 3 empty bodies: no mutation; 2 objgate libcp only; HEAD `#pragma optimize("s",on)` carried |
| f1 | 2 | G1 CONST float: a spelled literal replaces a made-up data symbol (0.0f, 1.5f) | RELOCATION_MATCH PASS | NF427 q3 precedent (granted): objgate differs only by the added `__real@` COMDAT; bits read from Fable.exe at the HEAD symbol's VA |
| j1 | 6 | G2 JMP (4: Initialise forwards 5 args to CAIStateGroupBase::Initialise 0x008FCEC0) + G2b (2: tail jump to an own non-virtual member) | RELOCATION_MATCH PASS | callee respells from `_ForwardTarget_*`; TABLE_CHANGES: 1 add (StandUp at 0x00910570) |

Two c1 rows use an existing branch, not a new model: 0x008FCFE0 GetDebugText (CAIStateGroupBase's FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS,
slot 6 already declared) and 0x00918ED0 InsideAtNight (CAIStateGroup_Gaze's hand-written GAZE + GAZE_RTTI_DECLS, slot 15 declared, probed
by NF330 through its derived models). Headers unchanged for those.

## Family and counts (evidence/worklist.tsv, classify.tsv, shapes.tsv, dropped.tsv)
Tracker regenerated (trk/). Class key = RTTI spelling's class, then the PDB spelling's, then the tracker klass. 565 family rows in 231
classes: rtti_identified 446, split_fixable 82, split_unknown 31, standin 6. Without the 218 deleting destructors the counts are
228 / 82 / 31 / 6, which matches the brief's 228 rtti_identified (the brief left the ??_G / ??_E rows out). 33 rows were dropped as
not in the family: 18 data addresses (0x012Bxxxx vftable / RTTI), 14 `_00c...` Unwind stand-in classes, and CAIStateGroupInfo.

### Shape histogram (retail body from the catalog, tools/shape427.py + NF429 shapes)
| member group | shape | rows |
|---|---|---:|
| virtual / member | OTHER (body-specific) | 274 |
| ??_G | deleting destructor | 116 |
| ??_E (tracker alias of ??_G) | deleting destructor | 102 |
| virtual | RET_TRUE | 16 |
| virtual | RET_FALSE | 12 |
| virtual | JMP | 8 |
| virtual | RET_LONG (incl. NF429 size-mode `33 c0 40 c3` / `6a imm8 58 c3`) | 8 |
| virtual | EMPTY | 6 |
| virtual | NOBYTES (Codex-29 reserved AIAction_* rows) | 5 |
| virtual | GET_MEMBER | 4 |
| ??1 | OTHER | 4 |
| virtual | EMPTY_RETN (NF429: `ret 8`) | 4 |
| ??0 | OTHER | 2 |
| virtual | RET_FLOAT_GLOBAL | 2 |
| virtual | RET_ZERO | 1 |
| virtual | EMPTY_RET4 | 1 |

58 constant / jump rows were generated (the 4 GET_MEMBER rows were not run; the 5 NOBYTES rows are reserved for Codex). 47 ACCEPT, 11 HOLD with reasons (batches/hold_g12, T3.txt "nf429 G1 / G2 hold"):
the base has no header (CleanHouse <- Clean, MinionBehaviourSad), a template base with no header (WanderAroundShops, WorkAtSchool,
PerformAction), the hand-written HoverMovementBase (HoverWanderAroundThing, HoverHornetQueen x2), MeleeCombatNearbyBase's two competing
models, a stand-in jump target (SitInPub 0x0094B730), and the unit hold for CreatureRangedCombat2 (its `_Methods` stand-in class is shared with 0x0091E970).

## Model layer (32 headers, one add-only `FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-in each)
- tools/genhdr429.sh + tools/genmodel.py (the NF322 generator NF427 used). Families: AB (base CAIStateGroupBase: FABLE_RETAIL_CAISTATEGROUPBASE
  + _MEMBERS, sizeof 0x14), CH:<B> (chained onto a model made here, or a landed NF322 / NF325 model with no Needs line: the chain's
  switches are walked), HB:<B>:<size>:<needs> (a hand-written shared base: CAIStateGroup_UsesSeat with its VIRTUALS_PASS27 switch).
- genmodel extensions (all evidence-based, all probed):
  - `--pure N=<override>` (tools/pure429.py): a new pure virtual slot (retail `_purecall`) is declared `= 0` with FableWin.pdb's
    LF_ONEMETHOD `pure intro virtual` name and access at that vftable offset, and the type of the retail override in a derived class
    (EnemyCreatureBase slot 20 IsCombatStateGroup, UsesSeat slot 14 IsStateGroupStillValid: two sources each).
  - protected destructor: where retail's slot-0 label is `??_G<C>@@MAE` (CreatureUseCoverBase: FableWin LF_ONEMETHOD
    `~CAIStateGroup_CreatureUseCoverBase` protected virtual, not compiler-generated; ??1 ...@@MAE@XZ in both PDBs), the model declares
    `protected: virtual ~C();`.
  - `--extra <mangled>`: a non-virtual member a G2b row calls (CreatureWantsPlayersAttention ABE, StandUp IAE; both in Ego_r + FableWin).
  - STLport include when a slot signature names std::vector (CreatureUseCoverBase ProcessCoverThingList / GetCoverThingList).
- Probe (tools/probe427.py + vtcmp.py; evidence/probe_p*.tsv, models/): every model compiled in a scratch TU that emits ??_7<C> and compared
  slot by slot with the retail vftable labels. **32/32 OK: emitted slot count equals retail and every slot is SAME** (exact RTTI-port label;
  `_purecall` and `??_E`-for-`??_G` (VC7.1's weak alias) are handled explicitly). Abstract classes use form 3: a scratch derived struct
  overriding every pure slot, compiled /Od so the class's own implicit constructor stores its vptr.
- hdrcheck.py: 32/32 SAME (dropping each opt-in gives the repo header back byte for byte); orig/hdr_<C>.h equal the repo.
- Unused models (generated and probed OK, rows held) kept in work/hdr_unused: SitInPub, CreatureRangedCombat2.

## Gates (per batch; batches/<b>/CHECKS.tsv and evidence/)
- vland HEAD/new at one LANDVERIFY_WORK path: grades equal, tests PASS; 0 LITERAL warnings; 0 new warn kinds (j1: 6 RELOC_NAME warnings removed).
- name_gate.py --test (overlay hdr/): 47/47 PASS on row and test. Tests include the shared header with the row's opt-ins. A protected
  member is reached through a scratch accessor struct derived from C (`struct NF429Access : C { static R call(C*) ... }`). It writes no body
  of C, instantiates nothing, and name_gate accepts it.
- Tests link the row object (qualified call `p->C::M()`; G2 / G2b define the callee in the test and check the call count, `this`
  and the arguments). Mutation: 37 rows FAIL their mutant. 3 rows needed a hand-written mutant (pointer `return 0` -> `(const CCharString*)4`;
  0.0f -> 1.0f, because -0.0f == 0.0f; work/vland_x1m.log). 10 empty bodies cannot be mutated: their leaf bytes equal HEAD's (GOTCHAS NF419 r1).
- Object section compare (gate425.py): ALL_EQ under the self rename (+ expected callee / literal renames), except `.drectve +libcp`
  (10 rows) and f1's added `__real@` COMDAT.
- vftable: no batch object emits a ??_7 / ??_G / ??_E (objdump of all 94 row + test objects), so vtx429 reports `none`, 0 WRONG. The model
  probes above cover every slot of every handed-back model.
- pk_all / pickany_vs_retail: copies 0 on both sides for every row; 0 NEQ, 0 reach flips.
- /FORCE link (linkgate425.py): duplicates HEAD 0 -> new 0 in every batch; every new self defined once; j1 callee renames
  unresolved -> resolved 6 (definers 0x008FCEC0, 0x0090F290, 0x00910570 compiled HEAD). The control (a copied object) gives LNK4006
  (evidence/linkgate_control.txt). Census: gen429 checks for no other out-of-line `C::M` definition (comment-stripped) and no other landed
  user of the old stand-in. inclass427.py: 47 selves, 0 in-class definers, 0 bound elsewhere.
- apply_table --dry: 0 misses, multi-target 1 -> 1 (j1: 1 add; others none).
- reserved_check (RESERVED_REFS=""): 0 reserved, 0 mentions, all 4 batches.
- equiv_check, copy against copy (old = cp of rebuild/include, new = cp + the 32 headers): **0 differing rows of 57,160**
  (evidence/equiv_check.tsv). Planted control: a typedef appended to the EnemyCreatureBase copy flags 7 of its 11 includer rows / tests
  (evidence/equiv_planted_control.tsv).
- Catalog: Module equals the new self's class for all 47 rows; source names keep the repo's `<ClassNoUnderscore>_<Member>_<addr>.cpp`
  convention. No RENAMES / CATALOG_CHANGES. Checked: no ACCEPT row has catalog CompilerFlags, and every row's `#pragma` lines equal HEAD's
  (gen429 carries HEAD optimize pragmas; the c2 rows had `("s",on)`).

## T3 lines resolved (evidence/t3_resolved.tsv, 40 lines; every T3 line any ACCEPT row is named in, checked against fam_excl.tsv)
nf324 port 30, nf324 held 5 (their "declares M only under a switch" reason is gone: the model declares it), nf324 rejected 1 (0x00933DA0:
the new self is UAE, as RTTI), nf319 held 4 (GetStateGroupType "declared only under a PASS switch": the model declares it). The lander can
release them with `--release-t3`.

## Residue (T3.txt, 553 lines; locks released / never taken)
- 218 deleting destructors: see RULINGS_NEEDED q1. NF424's G1 scaffold held 7 state-group rows because it adds CAIStateGroupBase's special-member
  COMDATs, and the other 211 are inferred to do the same (shared base). **No NF429 pilot was run.** The 32 models now solve the vftable half.
- 274 OTHER-shape overrides: real bodies on row-local models. 82 are an access-letter-only respell (QAE -> UAE/MAE), but every one sits on
  local CIntelligentPointer / CCharString / base models, so moving it to the shared header is a per-row port, not a generator shape.
  Of the rest, 34 differ in signature, 56 in stand-in class / member spelling, 100 have no RTTI spelling (split rows) and 2 have no self.
- GET_MEMBER 4 (not generated), ??0 2, ??1 4, NOBYTES 5 (Codex reserved), 11 G1 / G2 holds, 33 not in the family.
- Debt lines: made-up float data symbols still in other rows (NAME_FIXES); the unused models.

## Things to know
- The vtx checker copied from $S/vtx427.py wrote to $S/vtx_<batch>.tsv. I moved the c1 / f1 / j1 outputs into batches/<b>/evidence/vtx.tsv
  and repointed the tool. If files of those names existed before 13:48, they were overwritten.
- gen429's first census flagged 0x008FE0FB because a comment in 0x00A76320 names the member. The census now strips comments.
