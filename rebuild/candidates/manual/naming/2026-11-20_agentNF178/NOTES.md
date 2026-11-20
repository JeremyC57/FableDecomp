# NF178 notes (round 10): cut-scene cast-map family (q6368 q6493 q6504 q6509 q6510)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 5 entries, all `blocked` (port; partial, narrowing). Rows: 34 ported,
4 not-a-binder, 6 blocked (2 of them the self rows). recheck_final.log is one vland --recheck-landed run over the 34 ported rows:
34 RELOCATION_MATCH + PASS.
**NF178b update (see "NF178b follow-up" at the end): now 30 rows in rows/ and 4 held in held/ (0x00E12F20 0x00E1CD40 0x00E292C0
0x00E3E720); recheck_final.log is one vland --recheck-landed run over the 30 (30 RELOCATION_MATCH + PASS). pS/ holds NF178's objects (with
the _M_erase / clear copies): superseded, like accept_defs.log. The text below is NF178's and counts 34 where it says so.**

## What moved
- The 34 rows' local `struct CCutsceneCastMap` stand-in is replaced by the shared STLport map from `stlport/fable_map.h` (retail branch:
  FABLE_RETAIL_STLP_MAP + MAP_DTOR_DECL_ONLY + MAP_INDEX_DECL_ONLY, the 0x00D29880 set), instantiated over each row's EXISTING key / value
  classes (`CCharString`, `CScriptGameResourceObjectScriptedThingBase`; same tokens, so the same mangled names). No class token was added
  and no local model was renamed; the stand-ins `CCutsceneCastMap` / `CCutsceneFlagMap` / `CCutsceneArgMap` are gone from these rows.
- RunCutsceneMacro 0x00CBFB7D: each of the 34 rows now declares it with the Ego_r.pdb parameter types and binds exactly the self row's
  Leaf (`?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@...@_N4@Z`); the old `...AAUCCutsceneCastMap@@PAUCCutsceneFlagMap@@PAUCCutsceneArgMap@@_N4@Z`
  spelling is gone from them. The same rows also move 0x00CD3D2E operator[] onto `??A?$map<CCharString,...>` (the self row's spelling).
- Every new spelling was already bound by landed rows (narrowing only): ??0/??1/??A?$map (0x00E0A820, 0x00D29880, 0x00E12330, ...),
  RunCutsceneMacro (self Leaf + the fable_script_cutscene.h users), 0x00CD344A `?_M_erase@?$_Rb_tree<CCharString,...>` (reloc_bindings
  witness; 4 rows now reach it from the ~map COMDAT instead of `?Erase@CCutsceneCastMap@@...` / `?_M_erase@CCutsceneCastMap@@...`), 0x009AACE0
  `?clear@?$map<CCharString,CCharString>` (0x00D7C470). RENAMES section 2 has every changed pair (pdiff HEAD v new, 136 lines).
- Identity, second source: the ??0?$map and ~_Rb_tree COMDATs that the 0x00D29DB0 port emits grade RELOCATION_MATCH against 0x00CDBF70 and
  0x00CDBFB0, and the ??1?$map COMDAT that the 0x00E12F20 port emits grades RELOCATION_MATCH against 0x00CDBFB0 (q.py on the row file with
  those leaves); with Ego_r.pdb and the landed header binders that settles both names. Landed 0x00E137B0 (FABLE_RETAIL_STLP_MAP only) binds
  ??1?$map at 0x00CDBFB0 (compiled this pass), the shape the four expanded-ctor rows follow.
- Defined symbols (accept_defs.log, all 34 objects, no compile): every symbol a ported object adds against HEAD is a `std` header member
  (map / _Rb_tree / allocator / pair / vector members), every external definition is COMDAT, and the ??0?$map, ~_Rb_tree and (4 rows) ??1?$map
  COMDAT bytes are identical in all 34 objects (the graded ones above). Removed: the local stand-ins' members (??0/??1CCutsceneCastMap,
  ??0_Rb_tree_base, the local vector ctor / _Destroy). The row leaves are unchanged.
- _M_erase (0x00CD344A) COMDAT: the ported objects also emit the pick-any `?_M_erase@?$_Rb_tree<CCharString,...>` instance, built over each
  row's local key / handle models: 70, 125 or 59 bytes by row (accept_defs.log), none equal to retail's 123. This is existing header
  behaviour: landed 0x00D29880 emits the same 70-byte copy (its _M_erase graded against 0x00CD344A: DIFFER(70v123), this pass). The self row
  0x00CD344A defines it as a COMDAT too (123 bytes, RELOCATION_MATCH), so there is no LNK2005, but a link may pick any copy. Decision item 2.

## Shapes
- Plain ctor/dtor rows (18 incl. 00EA8130 with a by-value flag-map member): transform only (work/csport.py).
- Ctor expanded in retail (rows not in the 0x00CDBF70 REFS): 0x00DD1AF0 matched with the plain set; 0x00E12F20 / 0x00E1CD40 /
  0x00E292C0 / 0x00E3E720 need MAP_DTOR_DECL_ONLY left out (the implicit ~map is then called as its COMDAT, as landed 0x00E137B0).
  0x00DD1AF0 / 0x00E1CD40 / 0x00E3E720: their dead global `_Rb_tree_node_base` / `_Rb_tree_base` stand-ins were removed.
- No-Leaf rows (9): a `// Leaf:` line was added from the symbol the HEAD object defines (plus a one-line note); 0x00E9A270's own leaf is
  `?WatchForHeroEnteringCave@CV_HeroDuelScript@@QAEXXZ` (no NScript qualifier), left as is (not checked against the PDBs).
- Construct()/Destroy() view rows (7 of 9): the cast map is now a real object; where retail destroys it before the hero view, an inner
  scope ends right there (same call order). 0x00DC8C80 / 0x00E1CFD0 / 0x00DF0B70 also dropped a row-local `namespace std { vector }`
  model for stlport/fable_containers.h (VECTOR_DTOR_FORCEINLINE + VECTOR_ERASE_RANGE_OUTOFLINE, resp. VECTOR_DTOR_OUTOFLINE).
- Inner scopes (0x00DBEB20 0x00DC8C80 0x00E0AD40 0x00E1CFD0) carry an `// Inferred:` note (EXTRA_RULES 2026-11-19).
- 0x00D22880 / 0x00D29DB0 / 0x00D2A180 / 0x00E292C0: an unused row-local C++-linkage `int __cdecl rand();` was dropped (stdlib.h, now
  included through the container header, declares it extern "C"; C2732 otherwise). No call, no binding change.

## Left (exact)
- 0x00CDBFB0: 0x00D246C0 ?Destroy@CCutsceneCastMap@@QAEXXZ, 0x00DDD030 ?Destroy@CCutsceneCastMap@@QAEXXZ, 0x00DC21F0 ??1CCutsceneCastMap@@QAE@XZ,
  0x00E2AB30 ?AutoTinyFirstMemberDestructor_00cdbfb0@@YIXPAX@Z, and the self row (??1CRetailTree_erase00cd344a@@QAE@XZ, no Leaf line).
  0x00D0F640 / 0x00D45060 bind ??1?$_Rb_tree<...> through the shared header: the nf104 ICF case, not debt.
- 0x00CDBF70: 0x00D246C0 / 0x00DDD030 ?Construct@CCutsceneCastMap@@QAEXXZ, 0x00DC21F0 ??0CCutsceneCastMap@@QAE@XZ, and the self row
  (??0_Rb_tree_T_node0x24_00cdbf70@std@@QAE@XZ).
- 0x00CBFB7D / 0x00CD3D2E: 0x00D246C0, 0x00DDD030, 0x00DC21F0, plus stand-in binders outside the ctor/dtor census (they expand the map ctor
  and dtor inline, so they are not in those REFS; not attempted, locks not claimed): 0x00D823A0, 0x00E2A320 (operator[] only), 0x00E2A900
  (`RunScriptedCutscene` descriptive spelling), 0x00E80D60, 0x00EE2DB0, 0x00F01480, 0x00F02210 (no Leaf), 0x00F0BB10 (no Leaf). Several also
  bind 0x00CD344A as `?Erase@CCutsceneCastMap@@...` / `?_M_erase@CCutsceneCastMap@Row00e80d60@@...`; 0x00CDBEB0 binds it as
  `?Erase@CRetailTree_erase00cd344a@@...` (reloc_bindings).
- Why blocked: 0x00D246C0 / 0x00DDD030 destroy hero, cast, scripted (and movie) by hand at early returns; a real map is destroyed after
  every explicit statement, so the order cannot be kept until the scripted-thing / movie handles are real objects on
  engine/CScriptGameResourceObjectScriptedThingBase.h / engine/CScriptGameResourceObjectMovieBase.h (port those first). 0x00DC21F0: 3 forms
  DIFFER (STATUS). 0x00E2AB30: is NScript::CQS_ScytheInfoScript::`scalar deleting destructor' (NAME_FIXES); needs a re-author as the
  compiler-generated ??_G. Self renames wait for all of these (no new split).

## Decisions for the coordinator
1. Partial family: this hand-back moves 0x00CDBF70 / 0x00CDBFB0 / 0x00CD3D2E / 0x00CBFB7D partly (34 of 38 dtor binders, 29 of 32 ctor
   binders), not "fully moved or untouched". Every move lands on a spelling landed rows (or the self Leaf) already bind, so each address's
   spelling set only shrinks (GOTCHAS 476 narrowing). Land it as a narrowing partial, or hold the family until 0x00D246C0 / 0x00DDD030 /
   0x00DC21F0 / 0x00E2AB30 move?
2. The shared map is instantiated over each row's pre-existing LOCAL models of CCharString and CScriptGameResourceObjectScriptedThingBase
   (both have shared headers: fable_string.h, engine/CScriptGameResourceObjectScriptedThingBase.h). No new model or token was added, but the
   header templates now generate pick-any COMDATs under retail names from those local models; _M_erase differs by row (see above), as it
   already does for landed 0x00D29880. Accept, or require the key / value classes ported onto their headers first?

## Header opt-ins
- stlport/fable_map.h: FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY (hdr/README.md); used by 0x00E3E720 only. (NF178b: 0x00E3E720 is now held; two more opt-ins,
  FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY and FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY, see hdr/README.md.)

## Tests
- All 34 landed tests are unchanged: they are self-contained host programs that do not link the row (some carry their own host-side
  CCutsceneCastMap struct, a test-local stand-in of a header-less name, pre-existing). All PASS in recheck_final.log.

## Descriptive names / funclets / Flags lines / casts
- No new names. No funclet includes any changed file (grep of rebuild/src/compiled/01/21). No Flags lines added. No casts added.

## Locks
- Claimed for the family and kept: the 33 changed rows outside rows.txt (0x00D29DB0 was already in rows.txt). Claimed and released (unchanged):
  0x00DDD030, 0x00DC21F0, 0x00CD344A (owner `released`).

## Rule slips
- STATUS.tsv / APPLIED.tsv were written once at the end of the pass, not after every row (the work files and grade logs carried the state).
- 0x00D246C0, 0x00DDD030 and 0x00E2AB30 are blocked with zero grades (no source form keeps the by-hand order; 0x00E2AB30 needs a re-author).
- The remaining-binder lists come from REFS.tsv / the supplied census, an exe scan (refs.py) of 0x00CBFB7D / 0x00CD3D2E / 0x00CD344A, and a
  source grep for the stand-in spellings; the rows outside my 34 were not compiled (no probe).

## NF178b follow-up (2026-11-20): no pick-any copies of 0x00CD344A / 0x00CD3699 / the tree destructor
Task: EXTRA_RULES 2026-11-20 (a new pick-any copy of a function with its own landed row must grade MATCH / RELOCATION_MATCH against the
owner, or be made declaration-only). NF178's objects emitted `?_M_erase@?$_Rb_tree<CCharString, pair<const CCharString,
CScriptGameResourceObjectScriptedThingBase>, ...>` (0x00CD344A) with 70 / 125 / 59-byte bodies against retail's 123. The decision items
2 and the `_M_erase` paragraph above are superseded by this section for these rows.

What changed
- Header (hdr/README.md): `FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY` (no out-of-line _M_erase template body in the TU) and
  `FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY` (tree destructor declared only). Add-only, default path token-identical, pin_asserts pinned 0.
- Rows: all 34 define MERASE_DECL_ONLY next to `#define FABLE_RETAIL_STLP_MAP` (one comment line + the define). The 30 rows that take
  MAP_DTOR_DECL_ONLY also define TREE_DTOR_DECL_ONLY. Pre-change copies: work/pre_nf178b/; MERASE-only state: work/snap_merase_only/.
- Why the second opt-in: step 3 found a second table-mapped new symbol in all 34 objects, `?clear@?$_Rb_tree<CCharString,...>`, which
  reloc_bindings maps to 0x00CD3699 (landed row CTCCarryable_OnKill_00cd3699, a stand-in for the tree clear). Graded from the 0x00D29DB0
  object it is DIFFER(51v41): retail's clear is the /Os body (`and [x],0`, no `mov ecx,esi` before the call). Nothing calls it (only
  .debug$F refers); VC7.1 emits ~_Rb_tree, clear and _M_erase because the map ctor instantiates the member destructor. The existing
  FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE would have turned the inlined clear in the ~_Rb_tree / ~map COMDATs (which match 0x00CDBFB0)
  into a call, so the destructor is declared only instead. Landed 0x00D29880 emits the same 51-byte clear and 70-byte _M_erase at HEAD
  (debt, not touched; queue item for the coordinator).

Grades (recheck_final.log = one vland --recheck-landed run over the 30 rows in rows/): 30 RELOCATION_MATCH + 30 PASS (WINS: 30).
work/f1.log is the earlier run over all 34 with MERASE_DECL_ONLY alone: 34 RELOCATION_MATCH + PASS.

Objdump checks (objects: work/objs_f2 = final vland objects, work/objs_head = HEAD rows compiled this pass, work/objs_f1 = MERASE-only)
- `_M_erase` (0x00CD344A spelling): defined in 0 of 30 objects; also no longer referenced in them (the tree destructor that called it is
  gone). In the MERASE-only run it was referenced (undefined) and not defined in all 34. No other `_M_erase` is defined anywhere.
- No object defines or references `??1?$_Rb_tree<...>` or `?clear@?$_Rb_tree<...>` any more; `??1?$pair<...>` (table: 0x00CD27B4) gone too.
- Every symbol a final object defines that its HEAD object does not, looked up in reloc_bindings.tsv (tools/newdefs.py, work/newdefs2.txt):
  only `??0?$map<CCharString,CScriptGameResourceObjectScriptedThingBase>` (0x00CDBF70, all 30) and
  `??0?$vector<CScriptThing>(allocator const&)` (0x008AC5A0, 0x00DC8C80 and 0x00DF0B70).
  - ??0?$map: graded from rows/00d29db0.cpp against 0x00CDBF70: RELOCATION_MATCH; all 30 objects byte- and relocation-identical to that
    object's copy (cmpbody, work/cmpbody2.txt).
  - ??0?$vector(alloc): graded from rows/00dc8c80.cpp against 0x008AC5A0 (an ICF fold, catalog CIndexBuffer::CIndexBuffer): MATCH; the
    0x00DF0B70 copy is identical (cmpbody).
  - The other new vector members in 0x00DC8C80 (14 vector-related symbols) and 0x00DF0B70 (10) are not in the binding table; 0x00E1CFD0
    defines no new vector symbol against HEAD (VECTOR_DTOR_OUTOFLINE; its vector members keep their HEAD names).
- Table-mapped symbols the HEAD object already defined (149 pairs, work/common_table.txt): all byte- and relocation-identical to HEAD
  except the row leaves themselves (relocation names only: the NF178 respellings).
- Every function symbol defined in both the MERASE-only and the final object (872) is byte- and relocation-identical, so the leaves and
  their binding pairs are unchanged from NF178's hand-back (RENAMES section 2 for the 30 rows stands).

Held (held/, locks kept as NF178b): 0x00E12F20, 0x00E1CD40, 0x00E292C0, 0x00E3E720. These are the four expanded-ctor rows that leave
MAP_DTOR_DECL_ONLY out: their object emits the implicit ~map COMDAT (RELOCATION_MATCH v 0x00CDBFB0), which inlines the tree destructor
and clear, so the tree destructor cannot be declaration-only there, and the object keeps a new 51-byte pick-any copy of 0x00CD3699
(v 41). They stay on their HEAD spellings; their 17 RENAMES section-2 lines are removed (copy of NF178's file: work/RENAMES_nf178.tsv).
Their remaining wrong bindings (HEAD spellings, from work/RENAMES_nf178.tsv):
- 0x00CDBFB0 `??1CCutsceneCastMap@@QAE@XZ`: all four. 0x00CD3D2E `??ACCutsceneCastMap@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z`: all four.
- 0x00CBFB7D `?RunCutsceneMacro@@YIXABVCCharString@@AAUCCutsceneCastMap@@PAUCCutsceneFlagMap@@PAUCCutsceneArgMap@@_N4@Z`: all four.
- 0x00CD344A `?Erase@CCutsceneCastMap@@QAEXPAUnode@1@@Z` (0x00E12F20, 0x00E292C0), `?_M_erase@CCutsceneCastMap@@QAEXPAU_Rb_tree_node_base@@@Z`
  (0x00E1CD40, 0x00E3E720).
- 0x009AACE0 `?clear@CCutsceneArgMap@@QAEXXZ` (0x00E3E720).
With them held, no kept row moves a 0x00CD344A binding (grep _M_erase RENAMES.tsv: 0); NOTES "What moved" claims about the 4 rows that
reached 0x00CD344A from the ~map COMDAT and the 0x009AACE0 move now apply to held rows only.
The held files are byte-identical to the MERASE-only versions graded in work/f1.log (cmp with work/snap_merase_only/rows/).
APPLIED: q6368 / q6493 / q6504 / q6509 now 30 binders respelled (was 34); q6510 unchanged (29). 0x00E3E720 was the only user of
FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY (kept in the overlay, no user in rows/ now).

Decision for the coordinator
3. The second opt-in (TREE_DTOR_DECL_ONLY) goes beyond "make _M_erase declaration-only": without it, all 30 kept rows still define a new
   pick-any 0x00CD3699 copy with a non-retail body (51v41), i.e. step 3 would hold every row. Accept the opt-in, or hold the family?
   For the 4 held rows: a way out would be a header form of the tree clear that VC7.1 compiles to retail's /Os body inside an /O2 TU,
   or landing them once the 0x00CD3699 debt is settled header-wide (landed 0x00D29880 has the same copy).

Rule slips (NF178b): none known. Compiles were one at a time inside timeout; the HEAD objects were rebuilt from the repo sources with
q.py into pH/ (no writes under the repo).
