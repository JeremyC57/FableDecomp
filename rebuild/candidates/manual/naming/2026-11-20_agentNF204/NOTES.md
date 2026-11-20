# NF204 notes (round 10, remaining queue q4389 .. q12292; handed back for context after q6341 / q6342)

Counts (from APPLIED.tsv / STATUS.tsv): 49 entries: 1 done-already (q12292), 2 blocked: partial (q6332, q6341 = one unit), 3 blocked: port
(q4389, q4394, q5089), 1 blocked: identity / port (q4456), 2 blocked: family census only (q6333, q6342 = one unit), 2 deferred: held by
NF203 (q6315, q6319 = one unit), 38 open (not started; queue resumes at q6376). Rows: 1 ported (0x00433530), 0 not-a-binder, 29 blocked,
1 deferred (0x00833A70, NF203). Final recheck: recheck_final.log (0x00433530 RELOCATION_MATCH + PASS).

## q6332 / q6341 (0x00433530 vector<CCharString>::_M_insert_overflow)
- Self row moved onto fable_string.h + stlport/fable_containers.h; leaf-only self rename to the STLport protected spelling (RENAMES s1).
  Descriptive-by-library (STL instance; no PDB holds STL instance names).
- Inline control: the match relies on the EXISTING opt-in FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (NF41, row 0x00618420 precedent:
  retail expands all non-POD uninitialized copies of the overflow path). Plain inline measured DIFFER(280v316) (NF199 and here);
  the MALLOC_ALLOC allocate/deallocate layers changed nothing. No new FABLE_FORCEINLINE was added to any header.
- Split judgement (advisor): the probe (work/probe.tsv, all 39 REFS rows compiled at HEAD) shows no landed row binding the old self leaf
  `?_M_insert_overflow@?$vector@VCCharString@@@@QAE...` (global, one-parameter vector), so the rename narrows the address from 8 spellings
  to 7 and creates none. The older "every REFS row must end on the new spelling" wording is stricter: coordinator decision requested.
- Pick-any check: the object newly defines only shared-header inline COMDATs (vector<CCharString> _M_clear / _M_set / size,
  allocator<CCharString> allocate / deallocate, _Construct / _Destroy / __destroy / __destroy_aux, the cdecl `YA` spellings of
  __uninitialized_copy / __uninitialized_fill_n<CCharString*>, max<unsigned>, __false_type ctor). None has a reloc_bindings line or a landed
  Leaf; landed 0x0044B120 / 0x0044B150 define the __fastcall `YI` __uninitialized_* instances, a different symbol (no clash). The
  placement new ??2@YAPAXIPAX@Z (table 0x00405C10) was defined by the HEAD object too (unchanged).
- Stale table line: reloc_bindings.tsv lists 0x006FC8E0 as an IAE witness for 0x00433530, but the probe shows it binds the QAE spelling.
- Left binders (12) listed in APPLIED q6332. The five QAE rows each carry a same-TU _M_insert_overflow body in a row-local vector model:
  respelled to IAE they would newly define a differing pick-any copy of the symbol 0x00433530 owns, so they need declaration-only first.

## Findings for the coordinator
- NAME_FIXES 0x006E9310 = CCombatManager::IsAttackerOn (both PDBs + Ego_r order IsAttacked / IsAttacker / IsAttackerOn = retail
  0x006E9270 / 0x006E92C0 / 0x006E9310 + body). Its port (q4389) is a 2-row family with caller 0x0096FA90, whose local CCombatManager
  model declares IsCreatureFightingThing(const CThingCreatureBase&, const CThing*): the respell needs a CThing -> CThingCreatureBase
  downcast at the call (retail tests the creature type flag 8 just before), a cast on a local model, so it is the coordinator's call.
- q4394: the only wrong binder 0x009F03B0 is one family with q12191 (self rename to DeregisterSection), which sits in NF202's slice while
  the row lock is NF204's: give both to one agent.
- q4456: the three constructor rows 0x009BDE70 / 0x009CB910 / 0x00A16D50 are landed under non-constructor stand-in names, and their
  catalog classes are wrong by PDB size (CInputProcessManager 1784 bytes, CPhysicalPrimitiveInit 8 bytes): identity work, not naming.

## Descriptive names
- None new. 0x00433530 carries the library spelling.

## Flags lines
- None changed (0x00433530 keeps `/O2 /Oy /W3`, catalog flags unchanged).

## Rule slips
- One Bash call hung: a status-helper loop passed `-` (read stdin) as a field with no stdin; stopped it (TaskStop) and rewrote the lines
  with `n/a`. No file was half-written (the helper writes after reading).
