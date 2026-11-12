# Hand chunk 4003 (CScriptInfoManager::RegisterScripts): 1 of 1 byte-exact

Row 0x00CD52D0, 24,717 bytes (first instruction `sub esp,18h` at 0x00CD52D0, last `ret` at 0x00CDB35C, then three int3 to
0x00CDB360: the catalog start and length are right, no SPLITS.tsv). q.py, gall.sh and the read-only vland all say
RELOCATION_MATCH with no flags and no device; the behaviour test passes under g++ and VC7.1. Three full-function grades
(grades.log): g01 the first generated draft (matched), g02 / g03 re-grades of the same code after comment-level touches of
the overlay header and of the source's header comment.

IMPORTANT for whoever grades or lands it: the row needs the overlay (`export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent4003/hdr`):
it includes the new header engine/CScriptInfoManager.h and the opt-in FABLE_RETAIL_CSCRIPTINFO_CTOR of engine/CScriptInfo.h.

## What matched (1)
- CScriptInfoManager::RegisterScripts (0x00CD52D0): one basic block of 479 statements that fills the script table --
  6 progress texts, 5 master scripts (`AddScript(CScriptInfo(name, 1, true, <Script>::Alloc,
  <Script>::StatGetMasterScriptDataAlloc()), "")`) and 156 scripts with three statements each
  (`CheckSectionIsNotAlreadyRegistered(section); AddScript(CScriptInfo(name, 1, false, <Script>::Alloc, <master data allocator
  or NULL>), section); SetScriptActiveStatus(name, 0);`).
  Shape lessons (LESSONS.md): the FableWin.pdb constructor signature of CScriptInfo gives the store order; implicit
  const char* -> CCharString conversions; the by-value section; everything else (ebx = 0, edi = 1, ebp = the Sunnyvale data
  allocator, the eax / ecx / edx rotation of the two `lea` temporaries, the five statements whose four member stores stay ahead
  of the argument pushes) is c2's own doing and came out right at the first whole-function compile.

## Map (tools/map.py -> map/events.txt, map/units.txt; all from dis_all.txt)
- 479 statements = 6 PROG + 161 ADD + 156 CHECK + 156 STAT; call histogram of retail: 640 CCharString(const char*, long)
  0x0099EBF0, 484 ~CCharString 0x0099EAE0, 161 CCharString copy 0x0099EC30, 161 AddScript 0x00CB5C90, 156
  CheckSectionIsNotAlreadyRegistered 0x00CB5AC0, 156 SetScriptActiveStatus 0x00CBFAB8, 6 DisplayProgress 0x009E9F40.
- Sections (retail address of the progress text statement): "Registering Master Script" 0x00CD52D0 (1 master),
  "Registering Important Scripts" 0x00CD536F (7 scripts), "Registering Quest Scripts" 0x00CD57C8 (66),
  "Registering Vignette Scripts" 0x00CD7F64 (44), "Registering Global Quest Scripts" 0x00CD99DA (14),
  "Registering Test Scripts" 0x00CDA262 (4 masters + 25 scripts).
- Repeating unit (ADD, 25 instructions): name temporary at [esp+10h] (`push -1 ; push <name> ; lea ecx,[esp+18h] ; call ctor`),
  copy into CScriptInfo::Name at [esp+1Ch] (`lea r,[esp+10h] ; push r ; lea ecx,[esp+20h] ; call copy`), the section built in
  its argument slot (`push ecx ; mov ecx,esp ; push -1 ; push <section>`), the four stores ID [+20h] = edi, MasterScript
  [+24h] = bl, AllocFunc [+14h] = imm, MasterScriptDataAlloc [+18h] = ebp, `call ctor`, `lea r,[esp+18h] ; push r ; mov
  ecx,esi ; call AddScript`, then ~Name and ~temporary.
- Deviations of the unit (map/units.txt lists all 21 shapes with counts and first addresses):
  the `lea` register of the two address temporaries rotates eax / ecx / edx (41 / 41 / 39 units and their variants);
  five units keep the four stores AHEAD of the section pushes (0x00CD5745 LadyGreyWifeManager, 0x00CD7AB2, 0x00CD7BE4,
  0x00CD7D16, 0x00CD7E48: scheduler region cuts); MasterScriptDataAlloc is ebx (NULL) in 12 units, an immediate in the 22
  test-script units whose master is not Q_SunnyvaleMaster; MasterScript is the immediate 1 in the 5 master units; the first
  unit carries `mov edi,1 ; mov ebp,<data alloc>`.
- 38 of the 156 scripts are checked and added with the empty section "" (retail does that: `push 0x0122D70E`), so
  AddScript files 118 code sections.
- Cross-checks of the map: refs/script_recovery/native_catalog.json (161 scripts from Ghidra's quest registry table) has the
  same names in the same order with the same section, master flag, id and data allocator for all 161; all 161 names are the
  script names of Aeon's ports where a port exists (19 of the 20 seed-corpus packages are registered names or their suffixes).

## Names and evidence (LABELS.tsv has one line per address)
- `?RegisterScripts@CScriptInfoManager@@IAEXXZ` (protected): Ego_r.pdb + FableWin.pdb; sole caller Init 0x00CB5D80.
- `?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z` 0x00CB5AC0: both PDBs; donor order (directly
  before GetPScriptInfo in both PDBs, retail 0x00CB5AC0 / 0x00CB5AD0); the retail body only destroys its by-value CCharString
  (the check is compiled out); 156 call sites. The landed row at that address carries a propagated STL label (NAME_FIXES.tsv).
- `??0CScriptInfo@@QAE@ABVCCharString@@J_NP6IPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@ZP6IPAV3@XZ@Z`:
  FableWin.pdb (the COMDAT right after RegisterScripts); parameter order name, id, master script, alloc, master data alloc =
  retail's store order Name, ID, MasterScript, AllocFunc, MasterScriptDataAlloc (0x00CD531E..0x00CD5348). Inline in retail.
- `?StatGetMasterScriptDataAlloc@<Script>@NScript@@SIP6IPAVCScriptDataBase@@XZXZ`: FableWin.pdb, exactly five instances
  (CQ_SunnyvaleMasterScript, CalextestScript, CEmptyScript, Cjoss_test_scriptsScript, CDanyalTestScriptsScript) = the five
  master scripts retail registers, and the five data allocators retail stores. Inline in retail (the address is a constant).
- 161 script allocators `?Alloc@C<Name>Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z`:
  for every registered name both PDB name sets have the class C<Name>Script with this static factory (161 of 161); retail
  stores the address next to that name; 119 of the addresses are landed rows whose Leaf agrees (map/allocs.tsv). The script
  classes have no RTTI in retail, so the vtable route is not available.
- 5 data allocators `?Alloc@<Data>@NScript@@SIPAVCScriptDataBase@@XZ` (0x00CDBD20, 0x00CD4A60, 0x00CD4AC0, 0x00CD4B20,
  0x00CD4B80): both PDBs, donor order, and the master scripts they are registered with.
- Relocation identity: tools/rel.py pairs all 459 relocation symbols of the object one-to-one with 459 retail targets (7 call
  targets, 161 + 5 allocator addresses, 286 string literals; map/rel_final.txt); each string symbol's text equals the retail
  text at its address. vland: no RELOC_IDENTITY; one WARN RELOC_NAME (0x00CB5AC0's propagated catalog label).

## Descriptive (made-up) names
- No function, class, member or global name is made up.
- Not recovered: the enumerators of EScriptActiveStatus (no enum dump of the PDBs on this machine, no other caller of
  Set / GetScriptActiveStatus in Fable.exe). The row declares the enum without enumerators and passes `(EScriptActiveStatus)0`,
  the value retail passes (`xor edx,edx`).
- Parameter names of the CScriptInfo constructor (name, id, master_script, alloc_func, master_script_data_alloc) and of the
  manager's methods are descriptive (PDB name sets carry no parameter names); the two typedef names
  CScriptInfo::tAllocFunc / tMasterScriptDataAllocFunc are descriptive (FSE calls the data allocator type tScriptAllocFunc).
- The two declaration macros FABLE_SCRIPT_CLASS / FABLE_MASTER_SCRIPT_CLASS in the row are mine (see Header debt).

## Header changes (overlay hdr/, documented in hdr/README.md)
- engine/CScriptInfo.h: opt-in FABLE_RETAIL_CSCRIPTINFO_CTOR (constructor + the real __fastcall types of the two allocator
  members); default text unchanged (two landed includers preprocess to identical text with and without the overlay).
- NEW engine/CScriptInfoManager.h (class model, declarations only).

## Header debt
- The 161 script classes: ~100 have an engine/C*Script.h header, none of which declares the static `Alloc` factory, ~60 have
  no header. Including and extending them all from one TU is out of this row's reach, so the row declares each class with its
  one static member through FABLE_SCRIPT_CLASS (no layout, no padding); the five master scripts also get their data class and
  the inline StatGetMasterScriptDataAlloc. When the script headers gain `Alloc`, the row can include them instead.
- The landed rows of CScriptInfoManager (0x00CB5AD0 GetPScriptInfo, 0x00CB5C70 ctor, 0x00CB5C90 AddScript, 0x00CB5D80 Init)
  carry local class models; retype them with engine/CScriptInfoManager.h (Init's local model declares RegisterScripts /
  RegisterDefs public; both are protected in the PDBs).
- SetScriptActiveStatus / EScriptActiveStatus have no shared header (the landed 0x00CBFAB8 / 0x00CBF9CF declare them locally,
  as this row does).

## The behaviour test (test/00cd52d0.cpp, generated by tools/mktest.py)
The row's body (copied from src/) runs against a recording model; the expected 479 entries (kind, texts, retail address of
the allocator and of the data allocator, id, master flag, status) are generated from the RETAIL disassembly in address order
(map/events.txt), not from the source, and every model Alloc carries the retail address it stands for. Checks: the call
sequence entry by entry; check / add / status triples use the same section and name; the table AddScript builds (161 unique
names and allocators, 118 unique non-empty sections, 5 masters, every data allocator owned by exactly one master); every
stored pointer called through the table returns its own tag; no CCharString temporary leaks and the number built is retail's
(6 + 156 + 156 + 3 x 161); the constructor copies the name before the other stores. Mutation-checked under g++ (11 single
edits, each FAILs): a wrong allocator, a dropped status, a wrong section, a master flag, a wrong / an added data allocator,
a wrong id, a wrong status, a wrong progress value, two swapped statements, a dropped check.

## Tools (tools/, adapted copies of agent4001's or own work)
- `python3 tools/map.py`: dis_all.txt -> map/events.txt (one line per statement) + map/units.txt (the 21 instruction shapes).
- `python3 tools/gen.py <out.cpp>`: map/events.txt + map/allocs.tsv (name -> PDB class) + work/head.inc + work/tail.inc -> the
  whole source (this is how src/00cd52d0.cpp was made).
- `python3 tools/cc.py <src> <tag>`: whole-function compile (timeout 600) to tmp/b/<tag>/x.obj; `python3 tools/pd.py <tag>
  [--sum]`: relocation-masked instruction diff; `python3 tools/rel.py <tag> [--all] [--strings]`: symbol pairing (pairs by
  index when both listings have the same instruction offsets: agent4001's text matcher skipped relocated `push 0`).
- `python3 tools/mktest.py`: the test.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections (details and evidence in NAME_FIXES.tsv)
- 0x00CB5AC0 is CScriptInfoManager::CheckSectionIsNotAlreadyRegistered(CCharString), not std::_Cons_val<...EHeroMorphType...>
  (landed as Allocator::Cons_val_pair_morph_i).
- 0x00CDBD20 is NScript::CQ_SunnyvaleMasterData::Alloc (landed as the global pSunnyvaleDataAlloc_func, the FSE name).
- 0x00E9C300 is NScript::CV_HeroDuelScript::Alloc (landed as the stand-in Alloc_CVtable_012ed0d4).
- 0x00CD52D0 RegisterScripts and 0x00F2A0F0 RegisterDefs are protected (`IAEXXZ`); the landed Init binds them public.
- Five landed script allocators (0x00DDACB0, 0x00DFA020, 0x00E0AF60, 0x00E0E760, 0x00E81EB0) spell the second parameter
  `CGameScriptInterface&`; both PDBs have `CGameScriptInterfaceBase&`.
- native_catalog.json's "allocator" strings (e.g. HeroBoasts -> NCreatureAction::CActionEventRemoveObject::Alloc) are
  propagated Ghidra labels; the allocators are the script classes' own Alloc.
