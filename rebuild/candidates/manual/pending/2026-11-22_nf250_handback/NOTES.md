# NF250 notes (2026-11-22, round 18): NF248's open CODEXP27 lines, E2 0x0049D800, E3 0x01296F14

Handed back at an entry boundary (context). 13 rows changed, all MATCH / RELOCATION_MATCH + test PASS in one `vland --recheck-landed`
run (recheck_final.log). One header pair changed (add-only opt-ins, hdr/ + orig/, equiv_check 0 of 57,125 rows differ). No new
split: every address touched ends on the same or fewer spellings. Link of the new objects: 0 LNK4006 / LNK2005 (work/fin_f1.lnk.txt).
Repo root listed before / after: identical. No git, no foreground sleep, every grade under timeout, every grade after a clean clang precheck.

## Done (12 lines + E2 self row; L86 held pending decision 2)
- **L71 0x008AA9B0:** explicit instantiation of CCountedPointer.h's converting member template (MATCH). FableWin S_PUB32 + retail call
  0x009394F3, the only E8/E9 site in the exe (xsites.py; 0x008F69B0 likewise has one, 0x008F620F).
- **L83 0x008F69B0:** STLport vector<CHeightMapCell>(size_type). Settled: the element is CHeightMapCell (FableWin call from
  CHeightMap::Resize; the landed Resize 0x008F61A0's own pick-any copy of this ctor is EQ retail at 116 B). Needs `// Flags: /Gr` (Flags
  lines below): at the default convention uninitialized_fill_n stays out of line (82 v 116).
- **L86 0x0091C080:** CTCNymph::ClearReadyToAttackFlags on CTCNymph.h with the caller 0x0091C6A0's prelude + CINTELLIGENTPOINTER_ASSIGN.
- **L104 0x0096E4A0:** STLport std::sort<CThing**, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > via SORT_FASTCALL + _BODY
  (the 0x00A84160 pattern). Its three callees now bind the STLport spellings their own landed rows define, so 0x0096E410 / 0x0096D980 /
  0x0096D780 each go 2 -> 1 (TABLE_CHANGES).
- **L115 0x00AB15F0:** nested const C3DAnimationBuilder::CBoneInfo::FindChildIndex on C3DAnimationBuilder.h with the binder's prelude.
  The binder 0x00AB1740 defines the same function as a same-TU strong copy (identical body); linked together they give one LNK2005 with
  identical bytes (decision 3). Measured: the HEAD 0x00AB1740 object's copy is EQ 0x00AB15F0 (109 B, pk.py).
- **L118 0x00B209C0:** QAE -> UAE as an in-place respell of the HEAD local CPSCSplineBase model (decision 4). No binder: the three vtable
  emitters (dref.py) model at most two slots.
- **L119:** binder 0x00B3E580 ported (explicit (T*) ctor of CCountedPointer<CMeshBankStateBlock>; its own leaf becomes the spelling
  0x00B3BC90 already binds at 0x00B3E580); the stored deleter is now DeleteData<CMeshBankStateBlock> and the object's DeleteData copy is
  EQ retail 0x00B3E1A0. NAME_FIXES line for the catalog-only 0x00B3E1A0; table line replaced.
- **??_G rows (L29, L32, L50, L95, L96):** (L50 / L95 / L96 form 3: the #define / #include block of the landed row that already emits
  that vtable at HEAD, 0x00955620 / 0x00951FB0, so the vtable COMDAT is slot-for-slot that copy: vtcheck diff 0 of 82 slots; a minimal
  prelude gave a 71-slot vtable with base-class spellings.) free __fastcall stand-ins -> the compiler-emitted ??_G of the shared header class, through an
  emission harness in an anonymous namespace (`// Fidelity: scaffolding, not retail`; the 0x0069BAF0 / 0x0065E96D patterns). First form
  used global harness functions: 0x004A9710 / 0x004D6915 would both have defined ?EmitIntelligentPointerDeletingDestructor@@YAXXZ, which
  the landed 0x0069BAF0 also defines (LNK2005): moved into `namespace { }`, regraded. Identity of each: vtable slot 0 (scan) + RTTI
  (manifest rtti-vtables.tsv; for CIntelligentPointer<T> the T from the ctor rows' RTTI notes / the tabled vtable spelling).
- **E2 0x0049D800 (self):** ?M@C0049d800@@QAEJXZ -> ?GetLocalSeed@CWorld@@QBEPAKXZ (MATCH) on two add-only opt-ins (hdr/README.md).
  0x008EE720 still binds the CEngineRandomOwner stand-in: it needs a full row port (local CMaterialDef / CMainGameComponent models), open.

## Blocked
- **L72 0x008B83E0, L74 0x008CA600, L92 0x009535B0, L100 0x00956870** (CCombatActionBase-derived ctors): each ports cleanly onto its class
  header + engine/CCombatActionBase.h (RELOCATION_MATCH; both PDBs give the reference signatures; binders already right; the new ??_G copies
  EQ their owners), but the object then emits the class vtable (71 slots from the shared headers) and no landed row emits that vtable at
  HEAD (the binders 0x0091C6A0 / 0x00951FB0 / 0x00955620 call the out-of-line ctors). Its slots bind base-class spellings where retail
  holds the class's own overrides (vtcheck.py: slot 4 FinishAction, 11 Clone, 12 Initialise, 56 FrameUpdate, 68 OnAnimationLoaded ...),
  i.e. new vtable-slot splits (slot relocations are binders, nf226). Held; drafts in work/blocked/<addr>_f1.cpp. Fix: declare each class's
  overrides in its header (the JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27 pattern) from the PDBs / retail vtable, then these four land as drafted.
- **L89 0x00947B70** (PickUpTankard ctor): the header port grades RELOCATION_MATCH, but the object newly defines
  ??_GCCreatureAction_PickUpTankard@@UAEPAXI@Z at 30 B against its landed owner 0x0094B350 (43 B): retail inlines the implicit
  ~CCreatureAction_PickUpThingAtTable (the CIntelligentPointer member's ~CBaseIntelligentPointer + ~CCreatureActionBase) into the ??_G,
  VC7.1 here calls an out-of-line ??1CCreatureAction_PickUpThingAtTable (22 B). Held under the pick-any rule; draft in work/blocked/.
  Lock released. One form spent; a fix needs VC7.1 to inline the implicit base dtor into the ??_G (an inline-depth question).

## Open (not started, context): L4 / L5, L28, L57, L111, L114, L120, L37, L129, E3 (+ L101-L103). APPLIED keeps NF248's census for each.
- E3 plan: the unit is 7 rows, two of them large (0x0096DDB0 OnReplace 401 B and 0x0096DF50 Update 1150 B, the first on heavy local
  models). engine/CCreatureAction_ScreamerIntoAggressive.h defines the class in the global namespace; retail (Ego_r S_PUB32 ?A0x4c815337)
  has it in an anonymous namespace, so the header needs a new opt-in that wraps the class in `namespace { }` (and the catalog's
  ?A0x6cb5dcbb is a rebuild hash: correct it to the PDB's), then the ctor 0x0096DB60 / Clone 0x0096E510 / the two slot rows / ??_G
  0x0096E3F0 move onto it together with OnReplace's port. Authoring-scale: give it its own agent.
- L104 note for the coordinator: the four sort-family self rows (0x0096E410 / 0x0096D980 / 0x0096D780 / 0x0096E4A0) now agree; the two
  helper rows still model CThingCompare_Nearest / CAdaptThingCompareToSTLComparitor locally (headers exist): port queue.

## Decisions needed
1. Catalog / file / module renames (RENAMES section 1): 0x008AA9B0, 0x008F69B0, 0x0091C080, 0x00AB15F0 (C3DAnimationBuilder::CBoneInfo),
   0x0049D800 (CVertexBufferWin32::DoSizeof -> CWorld::GetLocalSeed), 0x00B3E580, 0x00B209C0, 0x0096E4A0; the others are leaf-only.
2. L86 0x0091C080: its object defines ??_G?$CIntelligentPointer@VCThingCreatureBase@@@@UAEPAXI@Z at 30 B (/O2) against owner 0x004D6915
   (28 B, /Os). At HEAD the same row defined the same function's copy under the stand-in name ??_GCIntelligentPointer@@UAEPAXI@Z, also
   30 B (vtable 0x0123E7D8 slot 0 is 0x004D6915 either way). I read this as the nf203 rule (a copy HEAD already defined and that already
   differed may keep differing: debt), not a new copy. Accept, or hold L86?
3. L115: accept the LNK2005 between 0x00AB15F0 and its binder 0x00AB1740's same-TU strong copy (identical bytes), or should the binder's
   copy become the owner's spelling in some other form?
4. L118: the in-place respell makes a member of the row-local CPSCSplineBase model virtual (engine/CPSCSplineBase.h exists; the HEAD row
   already modelled it locally, same class token, nothing new defined). Accept under the in-place rule (as nf248 ruling 1), or require a port?
5. L83 needs `// Flags: /Gr` (its build entry has none): the CHeightMap TU is /Gr (caller 0x008F61A0 builds /Gr and emits this ctor EQ).
6. The five ??_G rows use the established emission-harness scaffolding (anonymous namespace). OK? (vland prints a non-blocking
   STANDIN_LEAF warning for the three CCreatureAction ??_G leaves: "names another function" against the catalog's `scalar_deleting_destructor'
   text; it is the leaf the catalog row names.)

## Flags lines / funclets / tests / descriptive names
- Flags lines: 0x008F69B0 `// Flags: /Gr` (new).
- Funclets: none of the changed sources is included by an EH funclet row (grep of `#include "../src/compiled` for the changed files: none).
- Tests: all 13 keep their HEAD tests (host models or trivial PASS stubs; all PASS in recheck_final.log). Rewriting them on the shared
  headers is owed (as for NF248's nine).
- Descriptive names: 0x008F69B0 / 0x0096E4A0 STLport spellings are descriptive-by-library (the PDB name sets carry no STL instances);
  the five harness function names are scaffolding (anonymous namespace, never called).
