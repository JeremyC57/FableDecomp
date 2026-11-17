# Ego_d.pdb findings (2026-11-17)

Research only: no row, header, catalog or binding was changed. Ego_d.pdb itself is not in the repo (270 MB, SHA-256
`d7aa26ef34d4c1857e7cebf035f4a897b77751b9d5ca308535a618062efbeae1`, GUID `{B5DFE434-B3F5-441D-97D9-7DDCE5DF8227}`, age 4).
Two lookup tables carved from it are committed next to the other PDB tables (see "Lookup tables" at the end), so later work
does not need the PDB.

## What Ego_d is (read this before trusting any spelling from it)
- **Compiler: VC10, not VC7.1.** 1,239 compiland records say `Microsoft (R) Optimizing Compiler 16.00.40219` (VS 2010 SP1),
  the linker is 10.00.40219 and the live import is `MSVCR100D.dll`. So Ego_d's STL is VC10 Dinkumware
  (`_Tree`, `_Tree_val`, `_Tmap_traits`, `_List_val`), and its code generation, inlining and COMDAT layout say nothing
  direct about retail VC7.1 + STLport. Game classes, members, statics, enums and function names do carry over.
  So does the type of any `std::map` / `set` / `vector` / `list` as a whole: STLport's `map<K, V, less<K>, allocator<pair<const K, V> > >`
  has the same template arguments and so mangles the same. Only the internal tree and list spellings differ.
- **Live streams:** 144 public symbols (Ego_d.exe is a stub); the type stream is complete: 47,014 class/struct
  records (22,601 full definitions + 24,413 forward references) and 1,695 enums.
- **Old symbol data:** the MSF still holds the previous directory as stream 0. That directory is itself mostly gone: of
  its 1,376 streams only 7 are live, and the directory before it lost its first 21 KB. Its stream-size table went with
  them, so old streams can not be rebuilt by directory. They were carved instead: a byte scan for CodeView records
  (`u16 len, u16 kind, body`) whose name ends exactly at the record end. A module's function record is kept as
  "verified" only when its `S_END` sits at the offset the record claims, counted from the preceding `S_OBJNAME`, so the
  module stream is contiguous there.

## Answers

### 1. Global handler maps (8 rows) — RESOLVED, high confidence
The four maps are class statics, filled by two `StatRegisterClass` functions.

| Retail | Name (Ego_d decorated spelling; carries over unchanged to STLport, see above) |
|---|---|
| 0x013BAF2C | `?RegisteredHitLocationCodelets@CHitLocation@NHitLocations@@0V?$map@W4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@ZU?$less@W4EHitLocationCodelet@@@std@@V?$allocator@U?$pair@$$CBW4EHitLocationCodelet@@P6IXAAVCThing@@ABVCHitParameters@@@Z@std@@@5@@std@@A` (private static; typedef `NHitLocations::CHitLocation::TRegisteredHitLocationCodeletMap`) |
| 0x013BAFA8 | `?IsValidPredicates@CCombatSequenceBase@@1V?$map@W4ECombatSequenceIsValidType@@P6I_NABVCThingCreatureBase@@0@ZU?$less@W4ECombatSequenceIsValidType@@@std@@V?$allocator@U?$pair@$$CBW4ECombatSequenceIsValidType@@P6I_NABVCThingCreatureBase@@0@Z@std@@@4@@std@@A` (protected static; typedef `TIsValidPredicateContainer`) |
| 0x013BAFB4 | `?OnStartModules@CCombatSequenceBase@@1V?$map@W4ECombatSequenceOnStartModuleType@@P6IXAAVCThingCreatureBase@@0@ZU?$less@W4ECombatSequenceOnStartModuleType@@@std@@V?$allocator@U?$pair@$$CBW4ECombatSequenceOnStartModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@@4@@std@@A` (typedef `TOnStartModuleContainer`) |
| 0x013BAFC0 | `?OnStopModules@CCombatSequenceBase@@1V?$map@W4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@ZU?$less@W4ECombatSequenceOnStopModuleType@@@std@@V?$allocator@U?$pair@$$CBW4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@@4@@std@@A` (typedef `TOnStopModuleContainer`) |
| 0x008162F0 (catalog `_global::RegisterThingHandlers`, a made-up name) | `?StatRegisterClass@CHitLocation@NHitLocations@@SIXXZ` |
| 0x008B08B0 (catalog `CScreenEffectPriorityStack::Process`, a propagated label) | `?StatRegisterClass@CCombatSequenceBase@@SIXABV?$map@VCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@ZU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@Z@std@@@5@@std@@@Z` |

So the map types are:
- 0x013BAF2C: `map<EHitLocationCodelet, void (__fastcall*)(CThing&, const CHitParameters&)>`
- 0x013BAFA8: `map<ECombatSequenceIsValidType, bool (__fastcall*)(const CThingCreatureBase&, const CThingCreatureBase&)>`
- 0x013BAFB4 and 0x013BAFC0: `map<ECombatSequenceOnStartModuleType / ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>`

Every comparator is `less<key enum>`. The tree rows then follow: 0x00817D70 / 0x00818000 are `_M_insert` / `insert_unique` of the
EHitLocationCodelet tree. 0x008B29A0 + 0x008B2DD0, 0x008B2A60 + 0x008B2E90 and 0x008B2B20 + 0x008B2F50 are `_M_insert` +
`insert_unique` of the IsValid, OnStart and OnStop trees, in that order; retail confirms each insert_unique calls its own
`_M_insert` (0x008B2DD0 -> 0x008B29A0 and so on).

Evidence:
- **Names and types:** Ego_d publics (decorated names above) and Ego_d's type stream. In the type stream the class field
  lists carry `LF_STMEMBER` of exactly those maps (`RegisteredHitLocationCodelets` private,
  `IsValidPredicates` / `OnStartModules` / `OnStopModules` protected).
- **Second source for the names:** Ego_r.pdb and FableWin.pdb both have ``dynamic initializer for
  'CCombatSequenceBase::IsValidPredicates'`` (then OnStartModules, then OnStopModules, in that order; Ego_r 0x01013490 /
  0x01013510 / 0x01013590), the same for `NHitLocations::CHitLocation::RegisteredHitLocationCodelets`, and the matching
  atexit destructors. Retail's addresses A8 / B4 / C0 follow the same declaration order.
- **The registered ids match the enums exactly.**
  - The codelet function registers ids 1..4, and Ego_d's EHitLocationCodelet is CODELET_NULL = 0, TRAP_DEACTIVATE 1,
    AUTO_KILL 2, STRAW_DUMMY 3, DRAGON 4.
  - 0x008B08B0 registers ids 0..8 into 0x013BAFA8 (ECombatSequenceIsValidType has NULL plus 8 values), 0..1 into
    0x013BAFB4 (OnStart: NULL, CONTINUE_AIMING) and 0 into 0x013BAFC0 (OnStop: NULL only). The NULL entries get handler 0.
- **The handlers are the named functions** (Ego_r and FableWin names, Ego_d object files; signatures match):

  | Map | Handlers |
  |---|---|
  | 0x013BAF2C, codelets in `hit_location_codelets.obj` | 1 = 0x008F5D00 `?HLCTrapDeactivate@NHitLocationCodelets@@YIXAAVCThing@@ABVCHitParameters@@@Z`, 2 = 0x008F5CF0 `HLCAutoKill`, 3 = 0x008F5D40 `HLCStrawDummy`, 4 = 0x008F5E00 `HLCDragon` |
  | 0x013BAFA8, `NCombatSequenceIsValidModules`, `YI_NABVCThingCreatureBase@@0@Z` | 1 = 0x00905500 `IsTargetBlocking` (body `xor al,al; ret`, which matches its 18-byte debug body), 2 = 0x00905690 `IsToLoadRangedWeapon`, 3 = 0x009056F0 `IsToFireRangedWeaponAtTarget`, 4 = 0x00905510 `IsTargetHealthOver75Percent`, 5 = 0x00905530 `IsMyHealthBelow30Percent`, 6 = 0x009055E0 `IsBalverineAbleToLunge`, 7 = 0x009059D0 `IsAbleToSummon`, 8 = 0x00905A20 `IsTargetInLineOfSight` |
  | 0x013BAFB4 | 1 = 0x00905C30 `?OnStartContinueAiming@NCombatSequenceOnStartModules@@YIXAAVCThingCreatureBase@@0@Z` |

  None of these 13 addresses is a catalog row start today; they are naming leads, not checked against rows.
- **Who calls the registration functions:**
  - 0x008B08B0 is reached only by `jmp` from the end of `CCombatManager::RegisterCombatSequences` (0x008B13BC) with
    `ecx = this + 4`. Ego_r's CCombatManager has its sequence `map<CCharString, CCombatSequenceBase* (__fastcall*)(...)>` at +4,
    which is exactly StatRegisterClass's parameter (unused in the TLC body).
  - In Ego_d, `CCombatSequenceBase::StatRegisterClass` and `CCombatManager::RegisterCombatSequences` sit in the same object,
    `FableLib\debug\combat_sequence.obj`. Retail's 0x008B08B0 also sits among `CCombatSequenceBase` / `CCombatSequenceGeneric`
    rows, right before 0x008B0D10.
  - 0x008162F0 is called once, from `CTCBase::RegisterTCs` (0x004EE286). Its retail neighbours are the hit-location rows,
    matching Ego_d's object `FableLib\debug\tc_hit_locations.obj`.
- **Readers of the maps:**
  - 0x013BAF2C is read by `CHitLocation::ProcessHitLocationCode` (0x00815E80), which in Ego_d is
    `ProcessHitLocationCode(CThing&, const CHitParameters&)`, the codelet signature.
  - 0x013BAFA8 is read by `CCombatSequenceGeneric::IsValid` (0x008B07E0).

### 2. Identical unreferenced pairs (4 rows) — NOT resolvable from Ego_d
What Ego_d shows:
- For list<CCountedPointer<CNavRegionConnection> > it emits a chain of separate destructors, all from
  `BBBLibrary\Debug\lib_nav_quad_tree.obj`: `list::~list` (72 bytes), `_List_val::~_List_val`, `_List_nod::~_List_nod` and
  `_List_nod::_Node::~_Node`, in that order within the object.
- For map<unsigned long, CCharString> it emits `map::~map`, then `_Tree::~_Tree`, then `_Tree_val::~_Tree_val`, all from
  `FableLib\debug\ui_def1.obj`.

So each container's destructor chain is several out-of-line functions in a single object. That fits retail's pairs being
one instance's two destructor levels:
- the list pair: `~list` and `~_List_base`;
- the map pair: `~map` and `~_Rb_tree`. STLport's `~_Rb_tree` inlines `clear()` plus the header free, and an implicit
  `~map` inlines that.

Both retail pairs also sit in the matching object's region: 0x00A81470 / 0x00A82230 among nav quad-tree COMDATs, and
0x006311D0 / 0x00631420 among `NUISystem::CUIDef` / ui_def COMDATs.

The pairs are still unresolved, because Ego_d can not say which address is which:
- **Different STL and compiler:** debug VC10 Dinkumware has different classes (`_List_val`, `_Tree_val`) and a different
  emission order.
- **The retail heuristics disagree:**
  - Ego_d lists the outer class first, which suggests 0x006311D0 = `~map`.
  - Adjacency suggests the opposite: 0x00631420 follows `map::map` (0x006313E0, a 24-byte node, the same instance), so it
    would be `~map`, with 0x006311D0 next to the `_Rb_tree` helpers as `~_Rb_tree`.
  - For the list pair, 0x00A82230 follows a `list::list` at 0x00A82210, but that constructor allocates a 16-byte node, so
    it may be a different instance.

One retail fact for the record: identical unreferenced functions survive at separate addresses, so this link folded
nothing (no ICF) and kept unreferenced COMDATs (no /OPT:REF). Two identical bodies at two addresses are therefore always
two different decorated names.

### 3. Identical set<long> finds — partly answered; Ego_d can not split them
Retail call graph (full scan of `.text`):

| Copy | Callers |
|---|---|
| 0x00616580 | 13 call sites in game code: 0x005FEB1E, 0x005FEB5B, 0x005FEECA, 0x00616B7B, 0x00637E51, 0x006D134D / 0x006D1467 / 0x006D1734 (`CTCWife::CheckAppearanceChanges`), 0x0075D8CF (`CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing`), 0x0082B1D7, 0x0082B1FB, 0x0082B378 |
| 0x00B2EB20 | 2 call sites, both engine: 0x00B2DB6E (`CEngineSurfaceCompositionManager::UpdateDiskCache`) and 0x00B2EBFB, inside an uncatalogued 19-byte out-of-line `set<long>::find` wrapper at 0x00B2EBF0 that forwards to it |

So both named callers use 0x00616580. Const-ness does not explain the split:
- `IsCombatSplineValidForAnimAndThing` is a const member (Ego_r `QBE`), so it searches a const set;
- `CheckAppearanceChanges` is non-const (`AAE`);
- yet both call 0x00616580.
- The engine copy is used by `UpdateDiskCache(..., const set<long>*)`, which is const as well.

The split follows game code versus engine code. That points to two different decorated names coming from the two
libraries, not to const versus non-const `find`. Ego_d (VC10 Dinkumware) can not show STLport's instance spelling, so the
exact names are not resolvable here.

The landed binding for the 0x0075D8B0 caller is a stand-in (`?find@CAnimSet@@QAE?AUCAnimSetIterator@@ABJ@Z`). The engine
copy is bound as the const `find<long>` from 0x00B2D9F0.

### 4. Merged-function rulings — both RESOLVED, and neither is a merge (retail has no ICF, see 2)
**a) 0x004E37BF is `CTCTargetingPlayer::~CTCTargetingPlayer`, not `CGlobalDef::~CGlobalDef`.** The callee name is right
and the landed row name is wrong.

Retail body:
- `CBaseIntelligentPointer` destructors at +0xEC, +0xE4, +0xDC and +0xD4;
- 0x004E1AAF at +0x74 and +0x68;
- `CBaseIntelligentPointer` at +0x4C;
- tail jump to 0x004DA8B7.

How that maps onto Ego_r's CTCTargetingPlayer (256 bytes):
- `TargetedPoint` (a `CTargetPoint`, whose first member is an intelligent pointer) at +0x4C;
- `Targets` and `RejectedTargetPoints` (both `vector<NPlayerTargeting::CTargetPoint>`) at +0x68 / +0x78;
- `PLastThreatenedThing`, `PProjectileTargetedThing`, `PTargetLockedThing` and `PTargetOnlyThing` (`CIntelligentPointer<CThing>`) at +0xDC..+0xF4.

Ego_r's Dinkumware vectors are 16 bytes and retail's STLport vectors 12. That accounts exactly for retail's +0x74 and
for the 8-byte shift of the four pointers (retail +0xD4..+0xEC).

0x004E1AAF is a real `vector<CTargetPoint>` destructor:
- It calls `__destroy_aux` 0x004DBE58, which steps 0x1C bytes per element (sizeof CTargetPoint = 28; Ego_r puts
  `TargetedPoint` at 76 and `Targets` at 104) and destroys each element's leading intelligent pointer at 0x00A01C10.
- 0x004E37BF is its only caller.

Sources: Ego_r struct layout plus the retail bytes. The catalog's original label was also `CTCTargetingPlayer::~CTCTargetingPlayer`.

To check at landing: the base destructor 0x004DA8B7 (catalog `CTCTargeted::~CTCTargeted`) should then be
CTCTargetingPlayer's base. In 0x004E37BF it is the landed stand-in `CDefBase_004da8b7`. This is a name fix for the
coordinator; I did not edit the row.

**b) 0x00CD3A16 is `map<int, CCharString>::operator[]`; the catalog's `long` is wrong.** Its name:
`??A?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@QAEAAVCCharString@@ABH@Z`.

- Ego_r (0x00CB5792), FableWin and Ego_d all define `operator[]` only for `map<int, CCharString>`; no PDB has a
  `map<long, CCharString>::operator[]`.
- The users are `PlayCutsceneSound(map<int, CCharString>&, int)` and `RunCutsceneMacro`, neighbours in all three PDBs.
- In retail, 0x00CD3A16 is called only from inside the `RunCutsceneMacro` row (0x00CC0032, 0x00CC00AA). It calls
  0x00CD36C2, `map<int, CCharString>::insert` in the catalog, and through it the int tree 0x00CD34C5 / 0x00CD300B.

Separate inconsistency, outside this question: the binding for 0x00CD36C2 currently reads
`?insert@?$map@JVCCharString@@...` (`long`) while its catalog name says `int`.

### 5. libpk — NOT in Ego_d
- **CItem, CCodaFrameStreamer:** Ego_d has no libpk at all. Its type stream has no `CCodaFrameStreamer`, no global `CItem`,
  no `PKC*` types and no `CSoundEventManager` / `CItemDispatcher`, and no carved object file is a libpk one. The only
  `CItem` is `NInventory::CItem`: a class, 20 bytes, no vtable, so not libpk's keyed item. libpk's CItem class/struct
  question and the base of vtable 0x012B80AC stay open.
- **NGameText::CGroupEntry** is game code, not libpk, and is answered:
  - It is a `class` with public base `CResource` at offset 0 (Ego_d `LF_BCLASS`), sizeof 60 in Ego_d; `CResource` in turn
    derives from `CIVCountedPointeeBase`.
  - Second source: Ego_r lays out CGroupEntry (56 bytes) with its first 28 bytes as padding, exactly `sizeof(CResource)` =
    28 in Ego_r, then `Entries` at +28. Ego_r also names `?UnloadResource@CGroupEntry@NGameText@@UAE_NXZ` as a virtual override.
  - This fits the RTTI note in `engine/NGameText_CDataBank.h` (`CIVCountedPointeeBase` is the root), with `CResource` as
    the direct base.

## What Ego_d has beyond the Ego_r / FableWin tables (counts)
Carved records, after requiring each name to end at its record's end:

| Record kind | Count |
|---|---|
| S_PUB32 | 217,326 |
| S_GPROC32 | 155,333 |
| S_LPROC32 | 3,188 |
| S_GDATA32 | 2,434 |
| S_LDATA32 | 11,586 |
| S_PROCREF | 155,850 |
| S_OBJNAME | 1,929 |
| S_COMPILE3 | 1,853 |
| S_BPREL32 + S_REGREL32 (named locals and parameters) | 404,334 |

Some names occur more than once (several incremental generations).

**Decorated names** (publics plus data), distinct:
- 213,760 in all; 26,872 are string literals (`??_C@`), leaving 186,888.
- Of those, 155,454 are in `fablewin_pdb_names.tsv` and 41,041 in `egor_pdb_names.tsv`.
- **30,852 are in neither:** 25,234 RTTI / vtable descriptors, 4,406 functions, 1,127 data symbols, 85 template helpers.
- The new functions are mostly scalar and vector deleting destructors and their adjustor thunks (about 3,300, which the
  older tables spell differently), plus debug-only code: an in-game console (`NGlobalConsole`), UI debug code, and the
  Perforce client API (`lib_p4api.obj`: `ClientApi`, `Rpc`, `StrOps`, `FileSys`, `P4Tag`).
- The user's earlier raw string scan found about 118,000 names; this record-structured count is lower because it drops
  literals, text outside records, and names cut by later overwrites.

**Function to object file** (new information; neither older table has it):
- 101,128 function / object pairs (98,909 distinct functions) in 1,277 object files, each with its order inside the object.
- 1,335 distinct object-file names in all (FableLib 871, BBBLibrary 235, FableEngine 100, ParticleSystem 20,
  FableScripting 9, plus VC10 CRT and SDK objects), under `E:\FableTLC\Fable TLC Build Repository\BBBProjects\Fable1_5MainPC\`.

**Type stream:** the live TPI also has the static-member lists (`LF_STMEMBER`) and nested typedefs (`LF_NESTTYPE`) that the
Ego_r struct-layout table does not export. Question 1 used them.

## Lookup tables (committed; regenerate only from the PDB)
- `ghidra_out/egod_carved_names.tsv`: the 30,852 decorated names absent from both older tables. Columns: name, record kinds,
  `demangles` (1 when `llvm-undname` accepts the name; 2 do not). Anything already in `egor_pdb_names.tsv` /
  `fablewin_pdb_names.tsv` is left out; look there first.
- `ghidra_out/egod_carved_functions.tsv`: 101,128 verified functions. Columns: object file, old Ego_d section offset
  (only meaningful as order within the object), length, G/L, undecorated name. Rows whose module stream was not contiguous
  (about 50,900, unverifiable) are left out.
- Not exported: the 404k local-variable records and the type stream itself. Run `llvm-pdbutil dump -types` on the PDB for
  those.

Method, for reproduction: the PDB is from the owner's Drive folder "debug tlc" (file id in the task brief).
1. Carve: scan the whole file for CodeView kinds 0x110E / 0x110D / 0x110C / 0x1110 / 0x110F / 0x1101 / 0x1125 / 0x113C /
   0x110B / 0x1111. Keep a record only if its name is printable and ends within 4 bytes of the record end.
2. Attribute each S_GPROC32 to the nearest preceding S_OBJNAME, and keep it only when the record's own `pEnd` lands on an
   `S_END` relative to that module start.
3. Read the retail call and data references from a full `.text` scan of the Steam Fable.exe (not committed).
