# Hand chunk 716 (CTeamSpawn .. GVersion): 10 of 10 byte-exact

All ten rows were whole functions (each checked against the padding / the previous `ret`; the four 0x0121xxxx rows are
`.text$yc` dynamic initializers whose `.CRT$XCU` entries point at them). Every catalog label was a placeholder or wrong:
nine `_gapscan::sub_*` rows and one wrong class (`CActionPerformExpression`). Grades: 1 MATCH, 9 RELOCATION_MATCH; vland
(read-only) WINS 10, no DIFFER / RELOC_IDENTITY; tests PASS under g++ and VC7.1. Leaves confirmed with `objdump -t`;
every relocation target checked with calls.py.

## What matched (10)
- **NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::~CTeamSpawn 0x00DCF700** (implicit dtor). Identity: ??_G 0x00DCF6E0 ->
  vtable 0x012DB2D4 -> its only storer, the landed ctor 0x00DCD160 -> factory 0x00DCD0E0 (`push 40h`) -> bound in
  CQ_OrchardFarmRaidScript::Main 0x00DCC770 as "GuardTeamSpawn"; Ego_r / FableWin ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ.
  Shape: the landed CFisticuffsMember (0x00E839F0) TU recipe: real PDB ctor in the TU (emits vtable, ??_G and the implicit
  ??1), Ego_r members unshifted, inline `~CScriptThing() {}` over engine/CCountedPointer.h gives the expanded OtherSpawnPoint
  release; FABLE_RETAIL_CLEVELSCRIPTBASE (chunk 713) used. The same TU also grades RELOCATION_MATCH against the ??_G 0x00DCF6E0
  and the ctor 0x00DCD160 (both landed under wrong names: NAME_FIXES).
- **CActiveEntityScriptBase::OnPredicateFail 0x00CE1090** (MATCH): slot 5 of 0x012C3594 and of all 121 CActiveEntityScript<T>
  vtables; both PDBs. Repo headers as they are.
- **DirectShow strmbase AddRef triple** CEnumPins 0x00CA7AE0 (+0x14), CEnumMediaTypes 0x00CA7C10 (+0x10), CMediaSample
  0x00CA9060 (+0x44): byte near-twins, each named from its own vtable (QI IIDs read from the exe, Release slot offsets) and
  m_cRef offset; amfilter.cpp `return InterlockedIncrement(&m_cRef);` (0x0143FD34 = KERNEL32 InterlockedIncrement from the
  import table) tail-jumps through the IAT. Models reused from the landed Release rows.
- **CMediaPosition::GetIDsOfNames 0x00CAD2E0**: ctlutil.cpp forwarder to m_basedisp with IID_IMediaPosition (0x012AA064 =
  56a868b2-...), slot 5 of both pass-through IMediaPosition tables; landed GetTypeInfo model reused.
- **NPlayerGui::`dynamic initializer for 'VALUE_DISPLAY_DURATION'' 0x0121DF1C**: `(float)CMainGameComponent::GetConstantFPS()`;
  Ego_r/FableWin name, Ego_r order identical to retail (right before REACTION_MANAGER_DEF_INSTANTIATION_NAME's initializer).
- **`dynamic initializer for 'LHNetworkLib::LSocket::GlobalList'' 0x0121F850**: real STLport 4.6 `_List_base(const allocator_type&)`
  (sentinel from malloc, self-linked, stored last); the TU's `_$E2` also matches the atexit thunk 0x0122A200 and proves 0x008F9F50
  is `_List_base<LSocket*>::~_List_base`.
- **`dynamic initializer for 'gIME'' 0x01219E90**: inline `CIME() { ResetToDefaultSettings(); }` / `~CIME() { Shutdown(); }` via
  the new fable_ime.h FABLE_RETAIL_CIME branch (hdr/); `_$E2` also matches 0x012288B0.
- **`dynamic initializer for 'GVersion'' 0x01219F40**: `CVersion GVersion(25, 19, 5, 125);` over the out-of-line PDB ctor
  ??0CVersion@@QAE@EEKK@Z.

## Parked for Fable 5.1 (0)
None.

## Skipped
None (no row covers two functions; no fragment).

## Names and evidence (all proven from two sources; none descriptive except noted)
- 0x00DCF700 ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ -- Ego_r 0x00E04400, FableWin 0x0385D470, vtable/ctor/factory chain.
- 0x00CE1090 ?OnPredicateFail@CActiveEntityScriptBase@@UAEXXZ -- Ego_r 0x00CC45A0, FableWin 0x03B3A170, vtable slot 5.
- 0x00CA7AE0 / 0x00CA7C10 / 0x00CA9060 ?AddRef@{CEnumPins,CEnumMediaTypes,CMediaSample}@@UAGKXZ -- vtable slot 1 of each class's
  vtable (QI IID + Release offset), DX9 SDK amfilter.cpp text, landed sibling rows' spellings (UAG).
- 0x00CAD2E0 ?GetIDsOfNames@CMediaPosition@@UAGJABU_GUID@@PAPAGIKPAJ@Z -- slot 5 of both IMediaPosition tables, ctlutil.cpp text,
  the landed CBaseDispatch::GetIDsOfNames spelling.
- Dynamic initializers: PDB display names from Ego_r + FableWin (addresses in NAME_FIXES.tsv); their object symbols are the TU-local
  `_$E1` (Leaf). Line 1 of 0x0121F850 reads `LHNetworkLib::LSocket::`dynamic initializer for 'GlobalList''()` because finish.py
  splits on `::`; the PDB display name is `dynamic initializer for 'LHNetworkLib::LSocket::GlobalList'' (global scope).
- Globals: ?VALUE_DISPLAY_DURATION@NPlayerGui@@3MA (0x013BA860), ?GlobalList@LSocket@LHNetworkLib@@2V?$list@...@std@@A (0x013BB000,
  already bound), ?gIME@@3VCIME@NUISystem@@A (0x013B7CD8), ?GVersion@@3VCVersion@@A (0x013B85E4).
- Descriptive only: CVersion ctor parameter names (day, month, year, build) -- the PDB carries types only.

## Label corrections
All ten rows (see NAME_FIXES.tsv, 27 rows). Highlights: 0x00DCF700 is not CActionPerformExpression (NTCScriptedControl engine action)
but the Orchard Farm raid quest's CTeamSpawn entity script; 0x00DCF6E0 / 0x00DCD160 / 0x00DCD1C0 / 0x00DCD1D0 / 0x00DCD350 respelled
into NScript::CQ_OrchardFarmRaidScript::CTeamSpawn; 0x008F9F50 is ~_List_base<LSocket*> (bound as ~list, landed as
CGuiControlTreePane::SortTreeRecursively); 0x0122A200 / 0x012288B0 are `dynamic atexit destructor` thunks (landed as
GlobalMethodTailThunk stand-ins with a Leaf that matches no symbol); 0x013B7CD8 / 0x013B85E4 / 0x013BA860 / 0x013BB000 carry
stand-in global spellings; 0x009EA080 landed with `long` parameters (EEJJ) where the PDB says EEKK; 0x004051C0 also bound as
?Reset@NativeCIME; 0x00F35B40 also bound as ??1CFrontEndScreen.

## Decisions for the coordinator
1. **Linkage of VALUE_DISPLAY_DURATION (0x013BA860) and gIME (0x013B7CD8).** The PDB name sets prove the names but not whether
   retail wrote `const float` / `static`. Both forms give identical bytes (probe 0121df1c_p2: `const` makes the symbol internal,
   `_VALUE_DISPLAY_DURATION`). I used external, non-const spellings so the landed readers (AddDamageChange 0x006472B2, CIME rows)
   can bind them; data-symbol spellings are gated by the 2026-11-01 owner decision, so keep these gated until a PDB data dump
   (S_GDATA32 vs S_LDATA32) settles them.
2. **CCharString at 0x013B85E0 (0x01219F20, landed as g_CCharString_013b85e0 "Unknown")** sits between GGame and GVersion; the
   PDBs list GDebugString and GVersionString there, retail has one string: probably GVersionString (unproven; not renamed).

## Header debt
- fable_ime.h: the opt-in FABLE_RETAIL_CIME branch is new (hdr/README.md). The landed CIME rows (0x00404A80 Get, 0x004051C0 Shutdown,
  0x00404A90 ResetToDefaultSettings, 0x00405650 Initialise, 0x00405350 LoadIMESettings x2 files) still use local models / stand-in
  globals and could move onto it.
- No local base model sits beside an engine header in this chunk; the strmbase rows keep the landed DirectShow model (its
  combase.h CBaseObject is not engine/CBaseObject.h; `// Layout:` line added).
