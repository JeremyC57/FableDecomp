# Cleanup pass 1 -- NOTES (resumed run, 2026-10-28)

Workspace: $S/w22/cleanup1 (W). Nothing here reaches the repo; the coordinator copies W/fix, W/new and the TSVs.
Written progressively (a second restart loses nothing that is recorded here).

## Stock-take of the first run (restart recovery)
- W/fix mirrors 1,193 landed sources + their tests (2,386 files); 1,152 sources differ from the repo, 0 tests differ.
- W/grades holds a grade log per mirrored source (q2.py: honours `// Leaf:` / `// Flags:`, prints the RELOC pairs and the
  reloc_identity verdict). After the restart: 1,179 MATCH/RELOCATION_MATCH, 1 SRC_FAIL (00811e20), 3 DIFFER
  (0083fce0, 00927c80, 00a2ea60); 10 mirrored sources had no grade and are byte-identical to the repo (untouched).
- Every grade log is newer than its source (no stale grade). Rows whose grade log I could not account for were regraded.
- The first run's scripts (W/tools) cover: issue 1 (respell_player.py, gsi_fix.py, edits_tableonly.py, constfix.py:
  0x00487DC0 / 0x00487DD0 callers, 441 rows), issue 2 (stl_access.py + leaffix.py over 624 rows with local STLport
  container models), issue 3 (alivebody.py; IS_THING_ALIVE rows), issue 4 / A / C (wave1.py, wave2*.py: 48 rows).
- Fixed on resume: 00811e20 (a stray quote left by wave2.py; now MATCH as `??$__find_if@U?$_List_iterator@...`),
  00a2ea60 (bool const HasDummyObject needs the `if (found) return true; return false;` form: RELOCATION_MATCH).
- Pre-existing, not caused by the cleanup: 0083fce0 grades DIFFER(661v660) from the repo copy too (int3-after-noreturn,
  landed against the N+1 span, batch 2026-10-24_linux_hand480 oracle_int3_rows); 00927c80 grades DIFFER(287v287)
  from the repo copy too (landed 2026-10-19 under the QFE-4035 lane; register choice esi/eax). Both are left as they
  are in the repo apart from the respelling (no byte change from the respelling: the repo copies differ identically).

## Issues (per-issue evidence and changes follow as each is closed)

### Coordinator item: 0x004C73D0 / 0x006A4D00 byte twins (CThing::GetPos vs CThingPhysical::PeekPos)
- Both bodies are `mov eax,[ecx+60h] ; add eax,0Ch ; ret`. Only one PDB name exists for a function of that body in the CThing
  region (`?GetPos@CThing@@QBEABVC3DVector@@XZ`, 0x004C73D0, sole PDB name). 0x006A4D00 sits in the CThingPhysical run and
  Ego_r.pdb donor order settles it: donor OnDie 0x00683D30 / PeekPos 0x00683D40 / PeekLastPos 0x00683D50 / OnCreate
  0x00683D60 = retail Kill 0x006A4CF0 / 0x006A4D00 / 0x006A4D10 / OnCreate 0x006A4D20. Verdict (strong):
  0x004C73D0 = `?GetPos@CThing@@QBEABVC3DVector@@XZ`, 0x006A4D00 = `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ`,
  0x006A4D10 = `?PeekLastPos@CThingPhysical@@QBEABVC3DVector@@XZ` (20 byte-identical twins exist; identity is by run/order,
  the call-site feed (creature-typed receivers) agrees for every landed caller checked).
- Respelled every landed caller (text scan W/probe/callers_A.txt: 706 rows name 0x004C73D0, 64 name 0x006A4D00, 2 name
  0x006A4D10). Method: W/tools/respell2.py (declaration respell; when the declaration sat in another stand-in class the
  canonical class is inserted as an empty base of it, so the layout is unchanged; a polymorphic stand-in without a base
  hands its first virtual to the inserted base so the vptr stays at +0), W/tools/cfix.py (compile-driven: only the call
  sites the compiler attributes to the respelled class are renamed, pointer-returning stand-ins get `&`/deref fixes),
  W/tools/dual.py for the rows that modelled both twins under one name, W/tools/unwrap.py for the three rows whose model
  sat in an invented namespace (008d6860 NCreatureMode-wrapped CThing/C3DVector, 00935160 NCondition00935160, 006637d0
  X_006637d0: the engine types now mangle at global scope with the class key, e.g. `?GetPos@CThing@@QBEABVC3DVector@@XZ`
  instead of `?GetPosition@CThing@NCreatureMode@@QBEABUC3DVector@2@XZ`).
- Checked with W/tools/checkpos.py over every grade log: no RELOC to 0x004C73D0 / 0x006A4D00 / 0x006A4D10 carries any other
  spelling (exception: 0066ff20 CTCCoopSpirit::SwapToHero is an asm_bake row whose `extern "C"` label cannot be respelled
  without converting the row).
- Also settled on the way: 0x00661CC0 (`fld [g] ; ret`, three invented spellings) = `?GetPersonalSpaceRadius@
  CThingCreatureBase@@SIMXZ` (Ego_r.pdb; donor 0x0053CA80 lies in the CThingCreatureBase run before GetPHeroCombatDef
  0x0053E2A0 / IsPosInFOV 0x0053F470 = retail 0x00663340 / 0x00665260); callers 0092e950, 0092ed40, 0097fbe0 respelled
  (static __fastcall; in 0092e950 the retail evaluates the creature's PeekPos before the static call, written as two
  statements). 0x007F1B70 in 00871250 is `__fastcall(const C3DVector&, long, long, unsigned char)` with `ret 8`: the
  two pushes the old model gave to an invented `CThing::GetBonePosition(long, unsigned char)` at 0x004C73D0 are its
  stack parameters; the row now calls CThing::GetPos. 006cba20 (catalog CTCCreatureNavigation::IsPosNavLineOfSightTo)
  modelled 0x006A4D00 as an invented `CNavHelper::Compute`; rewritten as the PDB member
  `?IsPosNavLineOfSightTo@CTCCreatureNavigation@@QBE_NABVC3DVector@@@Z` calling Owner->PeekPos() (test rewritten).
- The three ChatGPT pass-8 rows 0x0093CFE0 / 0x00959EE0 / 0x0095A270 (CAIStateGroup_*::Initialise, `StartPos =
  creature.PeekPos()`) are respelled to CThingPhysical::PeekPos: W/new/gpt3/, RELOCATION_MATCH, tests PASS (g++ and VC7.1).
- Pre-existing DIFFER rows met on the way (the repo copy grades the same, the respell changes no byte): 0083fce0,
  00927c80, 00871d30 (operand order of a float add under the current lane flags).

### Addition B: chunk 517 held rows (invented `*_Sink` stand-ins) -> W/new/b517
- 009112a0 (FindMostRecentMessage<Bind3>, retail 0x00911240), 0092edc0 and 0094bfc0 (<Bind2>, 0x008D46D0), 00958770
  (<Bind4>, 0x00959040): the stand-in template body that called an extern `FindMostRecentMessage_Sink` is replaced by the
  real instantiation body (the landed callee's STLport list walk with the filters' out-of-line operator() 0x006AF1C0 /
  0x006AF1E0 / 0x006AF2D0 / 0x006AF200 / 0x006AF1A0 and the Bind operator() chain). RELOCATION_MATCH, the relocation binds
  the real template symbol to the retail instantiation (q2 RELOC lines), tests PASS (g++, VC7.1).
- 00917050 / 009170b0 (GetPBestThing<CIsFreeLogPile / CIsFreeBonfire, CThingCompare_Nearest, CThingObject>, retail
  0x00916F30 / 0x00916FC0) and 009652c0 (<CThingFilter_HasTC, CThingCompare_Nearest, const CThingMarker>, 0x00965180):
  the `GetPBestThing_Sink` stand-in is replaced by the real walk (landed body: PeekClassList 0x0049C770, inline
  IsIncludedInThingScans, THING_TYPE mask, filter operator() 0x00916E30 / 0x00916E90 / 0x006A80A0). The real body alone
  grades DIFFER (VC7.1 then stores the comparator's position words before loading them); with the comparator's real
  template body (the landed 0x00916B10 / 0x00964160 CThingCompare_Nearest::operator()<T>) also visible in the TU, as
  in the retail header, the rows grade RELOCATION_MATCH. Tests PASS.
- 0099f3d0 (CCharString::Insert; callee CBasicString<char>::Insert 0x009A06B0): the real body (landed 0x009A06B0, plain
  and `__declspec(noinline)` under `#pragma auto_inline(off)`) grades DIFFER(44v42): once VC7.1 sees the callee capture the
  two-byte buffer it no longer homes it in the dead `c` slot. Two real-body forms tried; the row keeps its held same-TU
  specialisation (`PData[position] = *text;`, which is a placeholder body, not an extern sink) and is delivered as held
  (RELOCATION_MATCH, tests PASS) with this note. The declaration carries the PDB spelling
  `?Insert@?$CBasicString@D@@QAEXKPAD@Z`; the landed 0x009A06B0 body declares `const T* text` (symbol ...KPBD@Z) -- a
  labels.tsv entry records the PDB spelling for that row.
- 006cba20 (catalog CTCCreatureNavigation::IsPosNavLineOfSightTo; PDB `?IsPosNavLineOfSightTo@CTCCreatureNavigation@@
  QBE_NABVC3DVector@@@Z`, donor 0x00701490): its landed model binds 0x006A4D00 as an invented `CNavHelper::Compute`.
  Four forms with CThingPhysical::PeekPos were tried (PDB member calling the +0x0C object's slot 10 directly, through a
  local, through an inlined IsPosNavLineOfSightToPos helper, and the landed register-explicit __fastcall skeleton with
  the real callee): all DIFFER on retail's entry `mov edx,[esp+4]` of `pos` (the landed model obtains it only by
  passing `pos` as the dead fastcall register of its invented callee). Left as landed; recorded here and in labels.tsv
  (0x006CBA20 -> the PDB name, 0x006A4D00 -> PeekPos). Attempts kept in W/probe/src/006cba20_*.

### Issue 1: 0x00487DC0 / 0x00487DD0 (CPlayer::GetPControlledCreature / PeekPControlledCreature)
- 451 landed rows (W/probe/player_addrs.txt) carried 67 spellings of the two accessors; every one now declares
  `CThingPlayerCreature* GetPControlledCreature() const; // 0x00487DC0` / `const CThingPlayerCreature* PeekPControlledCreature()
  const; // 0x00487DD0` on a CPlayer model (engine/CPlayer.h through the layout-rename include where the row needs the
  layout). Tools: W/tools/respell_player.py (shape-classified rename), gsi_fix.py / edits_tableonly.py (the
  CGameScriptInterface table rows), constfix.py (the const cascade: a const pointer result forces const receivers).
  The bindings_edit.tsv rows with target 00487dc0 / 00487dd0 list every old spelling.
### Issue 2: STLport access letters
- 624 rows with local STLport container models (W/probe/stl_addrs.txt) were checked against the STLport 4.6 source
  ($S/stlport_dl2): vector `_M_insert_overflow` protected (IAE), `_M_fill_insert` / `_M_range_insert` / `insert` /
  `reserve` / `operator=` public (QAE); `_Rb_tree::_M_find` / `_M_insert` / `_M_copy` / `_M_erase` private (AAE/ABE),
  `_M_create_node` protected; `_List_iterator`, `_Const_traits` / `_Nonconst_traits` structs (U); `char_traits` a class
  (`?compare@?$char_traits@G@std@@SAHPBG0I@Z`); `list::push_front` public. stl_access.py rewrote the access specifiers
  and leaffix.py the `// Leaf:` letters (161 leaf lines changed, W/probe/leaf_changed.txt); every row regraded
  MATCH / RELOCATION_MATCH (the access letter changes the symbol only).
- Kept deliberately: the repo's non-template `?_M_find@?$_Rb_tree@...` spelling (retail is a 4.5.x-style STLport --
  `resize(size_type, const T&)` by const reference, proven by `ret 8` and the pushed reference in 0x0075C040 /
  0x00889440 -- so 4.6's `_STLP_MEMBER_TEMPLATES` member-template `_M_find` is not assumed).
### Issue 3: 0x0062CF90 (CWorld::EACreatureBlock) and its IS_THING_ALIVE twin
- Verdict (high-probable): the row calls 0x0043DBD0 = `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayer
  Creature@@@Z`. Evidence (W/probe/alive_feeds.txt, W/tools/feeds.py): retail holds 20 byte-identical 18-byte
  IS_THING_ALIVE<T*> bodies (distinct instantiations, no ICF); every one of the 26 retail feeds of 0x0043DBD0 is a
  CPlayer::GetPControlledCreature (0x00487DC0, non-const) result, every feed of 0x0043DBF0 is a PeekPControlledCreature
  (0x00487DD0, const) result or a const-converted temporary -- a const pointer cannot feed the non-const instantiation,
  so the pair is <CThingPlayerCreature*> / <const CThingPlayerCreature*>; 0x0062CF90 feeds 0x0043DBD0 from
  GetPControlledCreature. 0x0040DCE0 = <CThing*> (fed by CThing*-returning GetPItem / Create / GetThingInPrimarySlot),
  0x005C05C0 = <CThingCreatureBase*>.
- Landed binding rows fixed: 0064a568 and 0065196c (mirrors in W/fix) now bind 0x0043DBD0 / 0x0043DBF0 under the template
  spellings; the four IS_THING_ALIVE body rows (0040dce0, 0043dbd0, 0043dbf0, 005c05c0) carry the template leaf and the
  catalog names in catalog_names.tsv.
- W/new/0062cf90/: RELOCATION_MATCH (q2: GetPlayer 0x0049D850, HasMode 0x00634160, GetPControlledCreature 0x00487DC0,
  IsMainPlayer, IS_THING_ALIVE 0x0043DBD0, CVectorMap LowerBound 0x0040F020 / 0x004365B0, CTCCombat IsBlocking /
  SetBlocking, CCreatureAction_StartBlocking ctor), test PASS (g++ and VC7.1); delivered in W/new/payload.json +
  W/new/oracle.tsv together with the three ChatGPT rows and the eight chunk-517 rows.

### Addition A/C items closed on the way (beyond the twin item)
- 0x0099A2F0 `??0CBaseClassNonCopyable@@IAE@XZ` (protected ctor) and 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ`
  (protected virtual dtor): 009ac4d0 and 004473a0 modelled the root base as an invented `CBase`, 00ab1660 and 0044a3b0
  declared the ctor public; all four now bind the protected ctor (RELOCATION_MATCH).
- Recheck warnings that are expected: `RELOC_CONFLICT ??$IS_THING_ALIVE@PAVCThingPlayerCreature... -> 0043dbd0 here,
  0043dbf0 in landed 0064a568` is the repo's reloc_bindings.tsv row from the old 0064a568 binding; the fixed 0064a568
  (W/fix) binds 0043dbd0 / 0043dbf0 to the two template spellings, so the table row changes with bindings_edit.tsv.
  `RELOC_CONFLICT ?ReadVar@CAFile@@QAEXPAHPAVCASuspendableProcess@@@Z -> 00516180 here, 00516d90 in landed 008269a0`
  (00500230) is pre-existing: the repo copy binds the same symbol (W/gradesold/00500230.txt); not touched by this pass.

### Addition C: 0x008FCFA0 / 0x008FCFC0 (CAIStateGroupBase::PeekWorld / GetWorld, protected, reference-returning)
- Identity: Ego_r.pdb donor order of the CAIStateGroupBase getter run (Terminate 0x008FCF40, PeekWorld 0x008FCFA0 `IBEABVCWorld`,
  GetWorld 0x008FCFC0 `IBEAAVCWorld`, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010);
  the landed stand-ins returned `CWorld*` under ~25 invented names (GetPWorld, GetPThingWorld, GetPFrameWorld, ...).
- W/tools/world_fix.py (declaration rewritten in place under `protected:`, or moved into the file's CAIStateGroupBase,
  or CAIStateGroupBase inserted as the stand-in's base; cfix.py --ptr turns `GetPWorld()->x` into `GetWorld().x` and
  pointer consumers into `&GetWorld()` compile-driven) over the 83 rows that name 0x008FCFA0 and the 50 that name
  0x008FCFC0: see W/probe/world_fix.log / world_fix2.log and W/probe/world_grades.log for the per-row outcome. Rows
  where the compile-driven pass could not finish were restored untouched (their reasons: a stand-in CWorld method
  called on the const PeekWorld result -- e.g. a non-const GetPMessageManager model -- or a CThingWorld-typed stand-in
  with members of its own, or a non-derived `*Methods` stand-in that cannot reach a protected member); they are listed
  in the final table as not reached.

### Addition C (second pass): the 24 world-accessor rows world_fix.py had restored
- W/tools/world_fix3.py (second chance, compile-driven): the stand-in world type the old accessor returned (CThingWorld,
  CWorldO) is renamed CWorld and a second `class CWorld` definition is folded into the first; `C3861 '<old>' not found`
  call sites are renamed (a `CWorld* x = ...` local becomes `const CWorld& x`); `C2662 const this` on a CWorld stand-in
  member makes that member const (inline models, and the bound 0x0049D810 GetPRandomSeed in 00943ae0: it is called on the
  const PeekWorld result, so the retail member is const); `C2664 'const CWorld' to 'CWorld *'` on a callee parameter
  respells that parameter `const CWorld&` (CTCVillage::IsTimeToGoToSchool takes ABVCWorld in Ego_r.pdb; the two rows
  that pass it, 0090fd20 / 00942a00, compiled through the address-of rule instead, so their stand-in still says CWorld*
  -- leftover). cfix.py's --ptr deref rule no longer eats a `*` that is a multiplication (`stagger * World()->x`).
- Hand edits: 0090fdb0 (fold repair, GFROR13 decl restored), 00929820 (COpinionManager definition moved before CWorld),
  00950a30 (`(CWorldSeed*)&GetWorld()`), 0096edb0 (`world = &GetWorld()` x3).
- The three `*_Methods` / `*Access` rows got the layout-rename include pattern: `#define CAIStateGroupBase
  CAIStateGroupBasePdb` around engine/CAIStateGroupBase.h, then a local `class CAIStateGroupBase : public
  CAIStateGroupBasePdb { protected: ... }` that carries the PDB accessors, and the method carrier derives from it:
  00914c60 (PeekWorld + PeekThingSearchTools 0x008FD000 `IBEABVCThingSearchTools`), 0096e700 (GetWorld; the
  CAIStateGroup_MeleeCombatNearby tail +0x14..+0x1f copied from the engine header), 0097d360 (the leaf itself moves from
  the invented CAIStateGroupBaseAccess into CAIStateGroupBase: `?AIAction_WaitForMessage@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EMessageEventType@@PBVCThingCreatureBase@@J@Z`
  per Ego_r.pdb, IsStopping 0x008FD010 `IBE_NXZ`, PeekWorld).
- Outcome: all 24 rows RELOCATION_MATCH, tests PASS under VC7.1 (g++ failures are the same MSVC-only tests as the repo
  copies: 00922600, 009226b0, 0093a080, 00930ee0, 0090fdb0, 00914c60, 0096e700). W/probe/world_probe3*.log,
  world_adopt*.log, world_tests.log. No world-accessor row is left on an invented spelling.

### Recheck (W/recheck.log) and the tests that had to follow the respells
- Dry `vland.py ... --recheck-landed` over every changed W/fix row in chunks (W/rc2: 12 chunks of 150; re-runs W/rc3 for
  rows edited later or whose payload row had been described wrongly; W/rc4 for the last 33), merged by
  W/tools/rc_merge.py; the payload/oracle delivered as W/recheck_payload.json + W/recheck_oracle.tsv are the latest
  row per address (W/tools/mk_final_payload.py). Payload rules learned: `pass_pattern` must be the test's own literal
  (recorded batch pattern, else the literal containing the address, else PASS, else `_OK`), `flags` a list,
  `name` the catalog name, `leaf` the `// Leaf:` line or the name's last component.
- Seven tests linked the source object and defined the old stand-in symbols; they now define the canonical ones:
  007a2f90 (CThing::GetPos), 00899c90 / 00899d50 / 00899db0 / 0089a070 / 0089a0e0 (`Sub_ResolveB` -> `CPlayer::GetPControlledCreature() const`),
  008916a0 (the LookupResult/Target model -> CPlayer / CThingPlayerCreature : CGSISheatheHeroWeapons_Target). All PASS
  (VC7.1, linked with the mirror's object; W/probe/t6.log). 0088fc60 and 00911f00 had only been reported FAIL by a wrong
  pattern (`GIVEHEROEXPRESSION_TEST_OK`, `CHICKENBEG_UPDATE_OK`): PASS with the right one.
- Two runners once shared one work dir on W/rc4 and produced a false LINK_FAIL and a false `["t"]` sweep tag; the
  chunk was re-run alone (both rows RELOCATION_MATCH PASS). 00927c80 grades RELOCATION_MATCH only under its landed
  `/Oa` extra flag (the lane finds it in its sweep; q2.py without the flag says DIFFER -- pre-existing, repo copy
  identical in that respect). 00871d30 (DIFFER 26v26 under q2) is an untouched mirror identical to the repo copy.

## Final table (cleanup pass 1)
| Item | Outcome | Evidence / where |
|---|---|---|
| Twin 0x004C73D0 / 0x006A4D00 (coordinator) | settled: 0x004C73D0 `?GetPos@CThing@@QBEABVC3DVector@@XZ`, 0x006A4D00 `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` (0x006A4D10 PeekLastPos, 0x006A4CF0 Kill); 560 + 61 bindings respelled; GPT rows 0093cfe0 / 00959ee0 / 0095a270 in W/new/gpt3 | Ego_r.pdb donor order (NOTES above), labels.tsv |
| Issue 1: 0x00487DC0 / 0x00487DD0 | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` / `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ`; 338 + 74 bindings respelled, 7 source-linked tests relinked | Ego_r.pdb |
| Issue 2: STLport access letters | protected `_M_*` members (IAE) across the STLport rows; real 4.5-style `resize(size_type, const T&)`; tests reach protected members through `ClearAccess<V>` | STLport 4.6 source, NOTES |
| Issue 3: 0x0062CF90 | CWorld::EACreatureBlock; calls 0x0043DBD0 = IS_THING_ALIVE<CThingPlayerCreature*> (high-probable: all 26 retail feeds of 0x0043DBD0 are GetPControlledCreature results, all feeds of 0x0043DBF0 PeekPControlledCreature results); 0064a568 / 0065196c rebound; row RELOCATION_MATCH + tests PASS in W/new/0062cf90 | W/probe/alive_feeds.txt |
| Addition B (chunk 517 held rows) | 7 of 8 with real callee bodies (RELOCATION_MATCH, tests PASS) in W/new/b517; 0099f3d0 keeps the held form (real CBasicString<char>::Insert body changes the leaf's frame: DIFFER 44v42 twice) | W/new/b517/*.grade.txt |
| Addition A / C items | 0x0099A2F0 / 0x0099A300 protected ctor/dtor (4 rows); 0x006AF290 / 0x006AF2B0 filter ctors; 0x008D46D0 FindMostRecentMessage<Bind2<IsOfType,IsTargettedAtThing>>; 0x008FCFA0 / 0x008FCFC0 PeekWorld / GetWorld (all 133 landed rows that name them, incl. the 24 second-pass rows and the three `_Methods` rows); 0x008FD000 / 0x008FD010 (two rows); 0x00661CC0 GetPersonalSpaceRadius (static __fastcall); 0x007F1B70 CreateCameraShake 4-parameter __fastcall | labels.tsv, renames.tsv |
| Recheck | 1,829 changed rows: 1,774 RELOCATION_MATCH PASS + 55 MATCH PASS, 0 other (W/recheck.log; payload W/recheck_payload.json, oracle W/recheck_oracle.tsv; W/lv* deleted) | rc2 / rc3 / rc4 chunk logs kept under W/rc2, W/rc3, W/rc4 |
| New rows | 12 in W/new/payload.json + oracle.tsv, dry vland: 12/12 RELOCATION_MATCH PASS (W/new/recheck.log) | |
| Tables | bindings_edit.tsv 1,568 rows; catalog_names.tsv 36 rows; labels.tsv 29 rows; renames.tsv 26 rows | regenerated after the last edit |

### Not fixed (and why)
- 006cba20 (0x006A4D00 under `?Compute@CNavHelper@@...`): every PDB-member form DIFFERs on the entry `mov edx,[esp+4]`
  (4 attempts, W/probe/src/006cba20_*); the repo model is kept; one BADSYM remains in W/tools/checkpos.py.
- 0066ff20: grade asm_bake (an oracle, not reconstruction); its `_CoopSpirit_GetThingPosition_004c73d0` extern is left.
- 0099f3d0: see Addition B.
- Pre-existing, untouched in this respect: 0083fce0 (q2 DIFFER 661v660 without the landed extra flags; lane PASS),
  00927c80 (needs its landed `/Oa`; lane PASS), 00871d30 (mirror identical to the repo; q2 DIFFER 26v26).
- 0x0049D810 CWorld::GetPRandomSeed: 17 distinct landed spellings (one row now const per the PeekWorld evidence); a
  family for a later pass. 0x00449970 (CPlayerManager::GetMainPlayer) still carries invented externs in a few
  CGameScriptInterface rows (`Sub_ResolveA`, `GSI_*_Helper_A`); 52 of its bindings were respelled on the way, the rest
  are a leftover. CTCVillage::IsTimeToGoToSchool's `const CWorld&` parameter (0090fd20 / 00942a00 stand-ins).
- Review lists not reached: hand512 / hand514 items, the 0x00529230 family, 0x00A11210 / 0x00A13490 bodies (labelled
  only), the 0x00A557E0 family, 0x013D28D8 / 0x013D28D4, 0x00912700 / 0x009130D0, 0x00403C60, 0x009E9900, 0x009E2C80,
  0x00519EF0, 0x004E1D20, 0x0040E170, the remaining 0x006AF290 / 00885ca0 / 00a31790 / 0075c040 / 00920ff0 / 008d46d0
  callers (0091cdc0 ...), 00811380, 009f9f70, 00497890 YIKK, 00a249a0, optional item 5 (hand44/45 parks).
