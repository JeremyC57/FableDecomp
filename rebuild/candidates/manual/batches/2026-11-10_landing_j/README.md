# Landing 2026-11-10j: chunk 897, 22 never-attempted rows hand-authored

The user asked for one Opus expert agent hand-authoring unworked rows (avoiding the 20 rows reserved for Codex pass 15). Chunk 897 had 26
never-attempted rows: 8 tiny named rows, 14 small gap-scan starts in game code and 4 large PC inventory rows. **22 landed** (12 MATCH +
10 RELOCATION_MATCH, every behaviour test PASS under VC7.1 `/Od` and g++ `-std=c++03`; no asm / volatile / bare `__forceinline`).
The coordinator re-ran vland read-only before landing (`logs/vland_dry.log`: 22 of 22).

| Address | Row | Lever / identity evidence |
|---|---|---|
| 0x00C0E2C0 / 0x00C0E310 | CString::SpanIncluding / SpanExcluding | MFC strcore with _MBCS: `_tcsspn` / `_tcscspn` inline wrappers over dllimport `_mbsspn` / `_mbscspn`, then `Left(n)`; new opt-in `FABLE_RETAIL_CSTRING_SPAN` in engine/CString.h (declarations only; hdrcheck2 default=same) |
| 0x00C28540 / 0x00C28560 | CHandleBasedMemoryPool_012b4098::InternalSetResource / InternalGetResource | Ego_r.pdb protected pair between Defragment and MoveArea, same order as retail, arities fit; catalog `Set/GetHandleAddress` labels were wrong. Dinkumware vector model as the landed 0x00C283A0 |
| 0x00DDE6F0 | CPrisonWarden::OnPredicateFail | vtable 0x012DCC50 slot 5; `Game->RemoveAllQuestInfoElements()` (slot 339, 0x00891C30) |
| 0x00E8BDE0 / 0x00EE94C0 / 0x00F00C60 | CGhostGrannyNecklace::Init / CExpression_FishScript::Init / COracleSpectator::Init | vtable slots; one member store each (script-class members at Ego_r offset - 0x14) |
| 0x007921B0 | CUnprovenRbTree_007921A0::end | sibling of the landed begin 0x007921A0 (owner unproven) |
| 0x00976E20 | CThing::GetTC<CTCJackPillar> | FableWin.pdb name; explicit instantiation of the shared GetTC (engine/CThing.h's "no out-of-line copy" comment is wrong) |
| 0x00976E70 | CUnprovenCombatAction_00976E70::~... | implicit dtor tail-jumping ~CCombatActionBase, emitted through a qualified destructor call (landed 0x008C36D0 pattern); class unproven (no reference) |
| 0x00BE42D0 | CLandscapeTesselatorBase::AddPoly(long,long,long) | Ego_r.pdb; three BridgeIndexBuffer.push_back on the shared header |
| 0x0083AD90 | NHeroStatModifierEvent::CBase::~CBase | RTTI of vtable 0x0127232C; FableWin public non-virtual dtor |
| 0x0083ADA0 .. 0x0083AE20 (7 rows) | STLport map COMDATs (iterator operator*, end, three iterator ctors, _S_value, _Select1st::operator()) | unreferenced COMDATs of the TU of the landed _Rb_tree::find 0x0083B120; placeholder key/value types `CUnproven*_0083ADA0` and STLport 4.6 text in the TU, as that landed sibling |
| 0x0083ADF0 | std::operator== (reverse_iterator) | POD one-pointer iterator with a member `==` compared by value gives retail's sete form |
| 0x005D7081 | CTCInventoryStats::ConstructExperienceListPC (3,882 bytes) | the landed console twin 0x005D6748 with the PC component names, mouse observe/ignore pairs, viewport scroll events and 303x53 sliders; last residue the order of two null-inits |

Not landed:
- 0x00F29FF0 CCutsceneDef::Copy: EXTENT_WRONG (5-byte `jmp ??4CCutsceneDef`; the row's 219 bytes swallow the separate 204-byte
  operator= at 0x00F2A000). Recorded in `BOUNDS_FIX.tsv`; the split (and a manifest row for 0x00F2A000) is a coordinator to-do.
- 0x005C9A95 / 0x005C8459 CTCInventoryWeapons::ConstructScreenPC / ConstructScreen and 0x005B8743 CTCInventoryClothing::ConstructScreenPC:
  not attempted (time); a console/PC pair, best done together.

Header debt (closed 2026-11-10, review follow-up): the seven STLport map COMDATs now include the shared `stlport/fable_map.h`
(0x0083AE10 through the new opt-in `FABLE_RETAIL_STLP_TREE_S_VALUE_CDECL`: STLport's plain cdecl `_S_value`, retail's unreferenced
COMDAT; the default `__fastcall` spelling the landed callers bind is unchanged), and the two pool accessors include the new
shared `libpk/CHandleBasedMemoryPool_012b4098.h` (the libpk pool layout with its VC7.1 Dinkumware `vector<CHeader*>`).
The 13 other landed libpk pool rows that still carry the same TU-local model are a follow-up rebase onto that header.
Names: catalog + manifest names for these 22 addresses (`ghidra_out/labels_landing_2026-11-10j.tsv`). Queued `land1011j` (PRIORITY):
0x00976E80 JackFlash ctor signature, 0x0083AE30 CHitThing ctor signature, 0x00F2A1D0 CCutsceneDef::Transfer is virtual.
All three fixed 2026-11-10: `CCreatureAction_JackFlash(CThingCreatureBase&, CThingCreatureBase&)`, `CHitThing(const CCreatureStatsDef&)`
(Ego_r member layout, new C++03 test) and `virtual void Transfer(CPersistContext&)` (vtable 0x012FB6E0 slot 18); all RELOCATION_MATCH.
The 12 changed rows re-grade from the repo with no overlay (`vland --recheck-landed`): 3 RELOCATION_MATCH, 9 MATCH, tests PASS.
Warning at landing: RELOC_NAME on 0x005D7081 (0x006D6FA0 is bound under its PDB name; the unlanded row's manifest label is stale).
`bindings_multi_check.py` prints 1 (CScriptThing::NullCoord), unchanged.
