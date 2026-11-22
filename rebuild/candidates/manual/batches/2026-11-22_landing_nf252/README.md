# Landing nf252 (Account A, 2026-11-22): the anonymous-namespace CCreatureAction_ScreamerIntoAggressive unit (E3 0x01296F14 + L101-L103, 6 rows)

Agent NF252 (E3 done; the other lines handed on). Hand-back: `pending/2026-11-22_nf252_handback/`.
- **0x0096DB60** ctor: row-local models -> the shared header, signature `PAV,PAV` -> `AAVCThingCreatureBase@@AAVCThing@@` (FableWin + the body:
  `ret 8`, both arguments passed straight to the base ctor's references), RELOCATION_MATCH. **0x0096E510** Clone: `QAE` returning
  CCombatActionBase* -> `UBEPAVCCreatureActionBase@@XZ` (Ego_r + FableWin). **0x0096E3F0** (L103): free `__fastcall` stand-in -> the class's
  compiler-emitted `??_G`. **0x0096DBB0 / 0x0096DBA0** (L102 / L101): `?Get@T@@QAE_NXZ` / catalog `vfunc_73` / `vfunc_74` -> IsToFaceTarget
  (slot 73) / IsToFaceTargetAtStart (slot 74), MATCH. The three emit through one anonymous-namespace harness (nf250 pattern, marked
  scaffolding). **0x0096DDB0** OnReplace: 13 row-local models -> 0x0096DF50's header set + engine/campfire_types.h CBRAINOUTPUT_ONLY + CAIBrain
  SETOUTPUT, first form; its callees 0x0073A8A0 (`HasMode QAE_NJ` -> `IsModeActive QBE_NK`) and 0x00857F30 (PAV -> AAV) respelled
  count-neutrally (other binders keep the old spellings).
- Every changed row uses 0x0096DF50's `#define` / `#include` block: each vtable COMDAT is 0x0096DF50's slot for slot (vtcheck, 0 of 81
  differ); every pick-any copy byte- and relocation-identical to 0x0096DF50's, the three it lacks EQ their retail owners. Census (hash
  stripped): 0x01296F14 2 -> 1 (the global-namespace vtable spelling gone), 0x0096DB60 2 -> 1, 0x0096E510 3 -> 1, 0x0096E3F0 3 -> 1,
  0x0096DBA0 / 0x0096DBB0 2 -> 1. Link (6 new objects + 0x0096DF50): 0 LNK4006 / LNK2005.
- **Header:** engine/CCreatureAction_ScreamerIntoAggressive.h already wraps the class in `namespace { }` (Codex pass 27); one new add-only
  opt-in `FABLE_RETAIL_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_CTOR_OUTOFLINE` (one assert pinned); its one includer 0x0096DF50 does not
  define it; equiv_check over 57,125 rows: 0 differ.
- Catalog / Module / files respelled for 0x0096DB60, 0x0096DBA0, 0x0096DBB0, 0x0096E3F0, 0x0096E510 and 0x0096DBC0 (Initialise, which
  lacked the namespace its siblings carry). reloc_bindings: `??_7CCreatureAction_ScreamerIntoAggressive@@6B@` ->
  `??_7CCreatureAction_ScreamerIntoAggressive@?A0x6cb5dcbb@@6B@` (01296f14, witness 0x0096DB60).
- **Recheck** (`logs/recheck.log`, landed tree): the 6 changed rows, the TU-mates 0x0096DF50 / 0x0096DBC0 (renamed) and the respelled
  callees 0x0073A8A0 / 0x00857F30 (10 rows): 3 MATCH + 7 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF252's questions
1. **Hash: retail's `?A0x6cb5dcbb`.** Fable.exe's own RTTI names `.?AVCCreatureAction_ScreamerIntoAggressive@?A0x6cb5dcbb@@`; the PDBs
   (Ego_r / FableWin, other builds) say `?A0x4c815337`. The EXTRA_RULES line "a hash found in no PDB came from a rebuild object: correct it
   to the PDB's" guards against rebuild hashes; a hash the retail exe itself carries is retail's and is closer to retail than any PDB's,
   so the catalog, table and Names lines keep 6cb5dcbb (the PDB hash is cited beside it). Counting is unaffected (hash stripped).
2. Leaf lines ending at `?A0x` (prefix match, EXTRA_RULES line 14), the full spelling on each Names line: accepted.
3. Catalog renames: applied (section 1 + 0x0096DBC0).  4. The table line: replaced with the 6cb5dcbb spelling.
5. The shared emission harness `EmitScreamerIntoAggressiveVirtuals` (anonymous namespace, marked scaffolding): accepted; the STANDIN_LEAF
   warning on 0x0096E3F0 compares the catalog text and does not block.
Owed: the six rows' tests are still HEAD host-model tests (queued). Slip (0x0096DBC0 compiled before its lock was claimed): noted.
