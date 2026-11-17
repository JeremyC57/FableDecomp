# 2026-11-17 landing nf84: name-fix round 7, NF84 (16 rows: the nf83 follow-ups and the 0x006DC020 name collision)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF84 took the 12 entries queued at the nf83 landing (q11044..q11055) and 23 rows NF64 had left blocked that no later agent touched.
Brief, list and records: `naming/2026-11-17_agentNF84/`.

## Rows (STATUS.tsv, 51 = 45 locked + 6 claimed through the lock): 16 ported, 8 not-a-binder, 27 blocked
- **Self renames (8):** 0x00789358 `CHitParameters::~CHitParameters` (was `CHitParameters_00789358`), 0x004585BD `CArmourDef::~CArmourDef` (was
  `_unattributed::~CAggregate_004585bd`), the STLport `deque<NUISystem::CComponent *>` rows 0x0059B576 `_M_push_back_aux_v`, 0x0059AAE3 iterator
  `operator-` and 0x0059B4FE copy constructor, 0x0053CB9B the `map<unsigned long, NUISystem::CUIState>` copy constructor, 0x00981EA0
  AIAction_SpeakAISpeechToTarget on its Ego_r spelling, and 0x006DC020 (below).
- **q11054, the collision:** 0x00451AC6 keeps the EDamageAttribute `~_Rb_tree` (it has the callers). 0x006DC020 only destroys a
  `map<EDamageAttribute,float>` at +0, which fits `~map`, `~CComboMultiplierData` and `~CPersistTraits<CComboMultiplierData>` equally in bytes.
  It is now `CPersistTraits<CComboMultiplierData>::~CPersistTraits`: FableWin.pdb keeps the traits ctor / dtor adjacent in the
  tc_hero_experience TU (the other two names come from other objects), Ego_d.pdb's tc_hero_experience.obj emits them adjacent after
  GetDefaultVal, and retail has the same ctor / dtor pair right after TransferBinaryIn (and the same pattern for CHeroStatIncreaseData).
  Best-supported, not byte-proven; NOTES lists the alternatives.
- **Binder ports:** 0x0085FF10 (NCreatureMode GetUpFromSitting) and 0x009387F0 on the shared headers; six rows (0x0057B783, 0x006644F0,
  0x00928E20, 0x009621F0, 0x0097D8B0, 0x0097DB70) have IsFreeToPerformAction respelled to the PDB form inside their pre-existing local models
  (the in-place precedent of NF79 / NF83; the full ports stay open).
- **Blocked (27):** the five q11048 `??_G` rows and 0x004861C3 (a real scalar deleting destructor comes only with the class vtable, so each must
  be re-authored with its constructor row), 0x0074ACE0 (C2259, abstract CCreatureActionBase), and 20 NF64 rows that were only probed.
- **Corrections to earlier notes:** 0x005BEAD5 / 0x005BF30D are whole wrapper functions, not fragments (Ego_r's order names them
  `CTCInventoryBase::GetBoughtPrice` / `GetAmountOfItemsContainable`), and the landed 0x005BE8D3 catalogued as GetBoughtPrice is
  `GetPerItemDataForItem` (queued, not graded).

## Entries (APPLIED.tsv, 12): 4 done, 3 done-already, 5 blocked
q11055's second source is FableWin.pdb 0x02833100 (CBrainOutput::SetAsPerformAction is not Ego_d-only). The five blocked entries have their
self rows right; only binder rows remain (`outside_binders.tsv`, from one compile of every landed caller).

## Rows and checks
- **Section 1: 8 self renames (14 file moves).** Modules match their siblings' catalog spellings.
- **Headers (4 opt-ins):** engine/CArmourDef.h (`FABLE_RETAIL_CARMOURDEF_DTOR`), stlport/fable_deque.h (`_STLP_DEQUE_COPY_CTOR`), engine/CAIStateGroupBase.h
  (`_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT`: the 7-argument overload, a descriptive declaration: no PDB has it), stlport/fable_map.h
  (`_STLP_TREE_COPY_CTOR_SMINMAX`). Token-identical without the opt-ins for 10 includers (`tokid.log`); no assert moved;
  `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 16 rows graded from the repo after landing: 16 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 28 binding lines and added 27; bindings_multi_check is at its baseline (`NullCoord`). NF84 compiled every
  landed caller of its 8 renamed addresses: none binds an old self spelling.

## Name fixes queued (`land1117fu`, 23)
NF84's NAME_FIXES.tsv (the outside binders, the CTCInventoryBase wrapper names, the CPersistTraits ctor and the CHeroStatIncreaseData pair, ...)
plus one line for 0x00860150: 0x0085FF10 now carries a pick-any COMDAT of the CCreatureAction_GetUpFromSitting constructor, so that row's
re-author must not define a nodup copy.

## Rule slips
- Most NF64 rows ended after a probe only (budget); about 70 read-only probe compiles of outside rows were run for the binder scans.
- A scan script misparsed its own output once (re-parsed from the saved dumps, no regrade).
- Two compiles failed because `fable_compiler.h` does not define `FABLE_FASTCALL` (`rebuild_abi.h` does).
