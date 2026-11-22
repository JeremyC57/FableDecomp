# NF256 notes (2026-11-22, round 21): L28, L57, the two sky builders done; L129 address proven + plan; L37 not started

Handed back early (context). 4 rows changed (0x0063E480, 0x00899DB0, 0x00B61CD0, 0x00B61FF0), all RELOCATION_MATCH + test PASS in one
`vland --recheck-landed` run (recheck_final.log, with the unchanged controls 0x0049BE90 / 0x0049C490 / 0x00B620A0: 7 / 7 RELOCATION_MATCH
PASS, 0 RELOC_CONFLICT; the WARN lines are RELOC_NAME label notes and 0x0049BE90's SHARED_HEADER_AVAILABLE, both at HEAD too). No header
change (no hdr/ / orig/). No new split: every touched address ends on the same or fewer spellings (census below). Link of the 4 new objects
+ HEAD 0x0049BE90 / 0x0049C490 / 0x009D0380: 0 LNK2005 / LNK4006 (control with a renamed duplicate object: 1 LNK4006, so the check is live).
Repo root (`ls -A`) before / after: identical. No git, no foreground sleep; no new CompilerFlags needed (none of the four rows has any).
Order: entries 1 (L28, L57), 3 (sky builders) and 4 (L129 proof) were done; entry 2 (L37) was SKIPPED for context, so 3 and 4 ran out of
order. **Slips:** (a) 0x00B620A0's HEAD source went into the first final recheck (as an unchanged control) before its clang precheck; the
precheck was run afterwards (clean). (b) The HEAD control compiles (hk.sh: 0x0063E480, 0x0049BE90, 0x0049C490, 0x009D0380) ran under q.py's
inner `timeout 300` (sc.sh), not an outer `timeout 900`; the last pair (0x00899DB0, 0x00B61CD0) ran under both. Every lock was NF256 and every
source was prechecked before its compile otherwise.

## L28 (done): 0x0063E480 CWorldMap::GetHeightZBelowPos
- Was: row-local CWorld / CMainGameComponent / CThingSearchTools / CPhysicsMeshObject / CTCPhysicsBase / C2DBoxI / CThingPtrVector models
  (CWorld and CMainGameComponent have headers), plus a same-TU strong stub `?GetAllThingsInRoughArea@CThingSearchTools@@QAEX...CThingFilter_All...`
  calling an undefined `?CollectThingsInRoughArea@@...` (UND in the HEAD object).
- Now: the 0x0049C490 self row's define block (ORIGINAL_ROUGHAREA + ROUGHAREA_EGOD + CLIPAREA + PEEKCELL_INLINE ...) with
  ROUGHAREA_NOINLINE_PASS27 (0x0079A890 does the same), CThingFilter_HasPhysicsMesh.h, CWorldMap.h HEIGHTZ, C2DBoxI ORIGINAL_FOUR_COORD_CTOR
  (the HEAD direct-store shape), the 0x0049BE90 CMainGameComponent / CWorld / CPhysicsMeshObject / CMeshIntersectionData set.
  Form 1 (filter as a named local): DIFFER 268v268, frame +4. Form 2 (filter as a temporary in the call, as 0x0049BE90): RELOCATION_MATCH.
- Filter: retail calls 0x0049C490, whose landed self row (MATCH) is the `<const CThing, vector<const CThing*>, CThingFilter_HasPhysicsMesh>`
  instantiation and whose other binder 0x0049BE90 binds that spelling: one address, one instantiation.
- Pick-any: the GetAllThingsInRoughArea copy is 583 = 583 bytes, bytes EQ 0x0049C490; pk.py prints NEQ:relocs only for
  `?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B@0137dadc`: the instruction carries addend 4 (tools/addend.py), and 0x0137DADC = LayerInfo
  0x0137DAD8 (table) + 4, so the relocation is right (pk.py does not model addends). Overflow copy EQ 0x0049C370; operator new EQ 0x00405C10.
  0x0049BE90's HEAD copy of the same template is NEQ (733 v 733, its ROUGHAREABODY body: HEAD debt, unchanged); ours adds no new variant.
  cmpobj against the define-block twin 0x0049C490's HEAD object: 44 common COMDATs, all SAME, 0 DIFF (covers the NO-OWNER copies).
- Census: 0x0049C490 2 -> 1 (table line for the stand-in, witness 0x0063E480, deleted); 0x00A12450 4 -> 4 (0x0063E480 moves from
  `QBE_NPBV...0PAV` to the PDB `QBE_NABV...0AAV` (both PDBs); 0x00875E60 keeps the pointer form, so its line stays).
- Debt seen, not touched: 0x0049BE90 still models CThingFilter_HasPhysicsMesh locally (the header exists; vland WARN).

## L57 (done): 0x00899DB0 CGameScriptInterface::IsPlayerCreatureFlourishEnabled
- Proofs asked for by the brief:
  - +0x91 bit 0 = CThing::Dead: Ego_r Dead at 0xA1 (uchar:1); retail = Ego_r - 0x10 (the four 16-byte Dinkumware containers before it are
    12-byte STLport ones: Components 0x48 -> 0x44 etc.); retail `test byte [eax+91h],1`; the shared CThing.h places it there (the row
    grades with the header layout; 0x008916A0's IS_THING_ALIVE reads the same bit).
  - +0x24 bit 16 = HasTC(TCI_COMBAT): UsingComponents bitset<274> at +0x20 (Ego_r 32, before any container, unshifted), TCI_COMBAT = 48
    (ETCInterfaceType.h; CTCCombat::StatGetInterfaceType) = word 1 bit 16.
  - The non-const GetTC path: CThing::GetTC(ETCInterfaceType) (non-const) -> Components.Find -> the non-const LowerBound, table line
    `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAEPAU?$pair@...@std@@ABW4ETCInterfaceType@@@Z` -> 0x0040F020
    (witness 0x00635080), and the row's own pick-any LowerBound copy is EQ 0x0040F020 (60 bytes). PeekTC's const LowerBound is 0x004365B0.
- Body: IS_THING_ALIVE (engine/ThingAlive.h, FableWin's IS_THING_ALIVE<T>) && HasTC(TCI_COMBAT), then
  `static_cast<CTCCombat&>(GetTC(TCI_COMBAT)).IsMeleeFlourishValid()` (CTCCombat.h WEAPONTRAIL declares it; needs
  engine/ECombatAbilityDataType.h included first). RELOCATION_MATCH first form. Pick-any EQ: LowerBound 0x0040F020,
  IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0, bitset<274>::test 0x00410DE0, HasTC 0x004118C0.
- Census: 0x00733EE0 2 -> 1 (the `@Sub_IsMeleeFlourishValid@4` line, witness 0x00899DB0, deleted; a grep of all of src/compiled finds
  `Sub_IsMeleeFlourishValid` only in 0x00899DB0, and the other six referencers (0x0064B1AC, 0x0064BE45, 0x0068ECA0, 0x0068F470, 0x00736760,
  0x00839100) declare the member); 0x0040F020 / 0x00449970 / 0x00487DC0:
  0x00899DB0 binds the spellings it bound at HEAD (count-neutral). 0x00899DB0 itself: no landed referencer (no slot-152 vtable emitter).
- Test: the HEAD test linked the free stand-in, so it would LINK_FAIL. Rewritten on the shared headers (rows/00899db0_test.cpp): the row's
  own define / include block, zeroed byte images reinterpret_cast to the header types, stubs for only the three imported out-of-line
  members, the row called qualified (no ctor, no vtable); five cases; PassPattern kept. (A first draft in the older 0x008916A0
  link-stand-in pattern modelled header classes locally, against the NF214 rule; it was replaced before hand-back.)

## E3 (done): 0x00B61CD0 BuildBaseBandMesh, 0x00B61FF0 BuildStarFieldVB (nf254 L120 follow-on)
- 0x00B61CD0: public `CArray<SSkyVertex>& / CArray<unsigned short>&` local model -> the header's private
  `?BuildBaseBandMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z` (both PDBs; the caller 0x00B620A0 binds it; the body
  reads each array through its first word) on BUILDBUFFERS_CALLEES + engine/CTVertexColTex1.h (the 0x00B61DD0 recipe). First form.
- 0x00B61FF0: `CSkyRendererMethods` stand-in over the flat sky header -> `?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ` on the retail sky
  branch (StarFieldVertexBuffer CVertexBufferWin32 +0x24, StarCount +0x150, StarVBFilePath CWideString +0x158), with the row-local
  CWideString / CAFile / CDiskFileWin32 models replaced by fable_string.h / engine/CAFile.h / engine/CDiskFileWin32.h (the 0x009D0380 set).
  First form; relocation list identical to HEAD; the only emitted vtable of the four new objects (objdump scan for `??_7`). vtcheck `??_7CDiskFileWin32@@6B@` v 0x0122D06C: 16 slots, 7 OK, 9 NOLINE (no table
  line), 0 conflicts (HEAD emitted a 3-slot stand-in vtable with `?Open@CDiskFileWin32@@UAEXXZ`). New pick-any `??1CAFile@@UAE@XZ`
  (5 B) is NEQ v its table owner 0x0099A300 (7 B), but byte-SAME as landed 0x009D0380's HEAD copy (cmpobj), so no new body variant
  enters the build (the nf254 D1 reasoning; ~CDiskFileWin32 EQ 0x00401630, as at HEAD: gr_s2.pkh.tsv). Neither the leaf nor the
  ~CDiskFileWin32 copy references `??1CAFile` (relocations: Close, ~CWideString, ~CBaseClassNonCopyable; the leaf's list equals HEAD's),
  so unlike D1 no owned copy's relocation depends on the NEQ one.
- Census: 0x00B61CD0 2 -> 1, 0x00B61FF0 2 -> 1 (each: the self row's old leaf + the PDB line; 0x00B620A0 is the only referencer).

## L129 (address proven; family plan, no row changed): 0x01436EA8
- Proof (two sources): Ego_r.pdb S_GDATA32 `NEngineInternal::GPLandscapeRenderer` 3:0007be04 and `GPResourceManager` 3:0007bdf4 (+0x10);
  FableWin.pdb 4:0008808c and 4:0008807c (+0x10); between them both PDBs list GPLightingManager, GPCamera, GPEngine at +4 / +8 / +0xC
  (tools/gdata.py raw record scan). Retail: the table already binds GPResourceManager 0x01436E98 (NF46), GPLightingManager 0x01436E9C,
  GPCamera 0x01436EA0, GPEngine 0x01436EA4 in that order, so GPLandscapeRenderer is 0x01436EA8. Spelling
  `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A` (7 landed rows, witness 0x00BEAE50). NAME_FIXES line.
- Plan (unchanged from nf254 apart from step 1 being done): declare it in engine/CEngineLandscapeRenderer.h behind an add-only opt-in
  (next to the class, `namespace NEngineInternal { extern CEngineLandscapeRenderer* GPLandscapeRenderer; }`, after checking no landed row
  already declares it with another type: grep `GPLandscapeRenderer` in src/compiled first); then port the 84 stand-in referencers per
  pointee family (nf254 NOTES list), 20-30 per payload, each family's stand-in pointee replaced by CEngineLandscapeRenderer members (a
  port, not a rename); land the family whole so the 17 other spellings go at once.

## L37 (not started: context)
- nf254's census stands (8 stand-in binders, 7 spellings). New finding for the plan: **0x00ADC310's catalog name is wrong.** Retail
  0x00ADC310 copy-constructs a 12-byte first member with 0x00535470 (vector<unsigned long> copy ctor) and a second member at +0xC with
  0x00A96E40 (`std::map::map`, table `??0?$map@KJ...` = map<unsigned long,long>), so it is the _Cons_val / construct of a
  `pair<const X, map<unsigned long,long> >` with X a 12-byte vector<unsigned long> (or CArray<unsigned long>), not
  `pair<CCharString const, CArray<unsigned long> >` (that is its twin 0x009D3CA0: 0x0099EC30 on +0, 0x00535470 on +4). The key type
  (vector v CArray) needs its map's users before the catalog can be fixed. Both rows are July rows without a Leaf line (NF44 round-1
  lesson): their port needs the leaf settled first, which is a self rename.
- Recommended order for the family pass: the four header-shaped ports first (0x006250B0 CULongVector, 0x009B0AC0 CRetailVector,
  0x009D4810 / 0x00ADCE10 one-parameter vector<K>, each onto stlport/fable_containers.h), then 0x00AD9B20 / 0x00ADA240 (CGroupArray onto
  engine/CArray.h) as a pair, then the two _Cons_val rows with their leaves.

## HEAD objects
- Every changed row's HEAD object was built (work/hc/): the RENAMES old leaves of 0x00899DB0 / 0x00B61CD0 / 0x00B61FF0 are read from
  them, and 0x00899DB0's HEAD object binds the same LowerBound / GetMainPlayer / GetPControlledCreature spellings as the new one (only
  0x00733EE0 changes). Defined-symbol diffs v HEAD: only the leaf, shared-header inline / pick-any COMDATs and the dropped local-model
  members (sym_pH_* / sym_pS_* in work/).
- recheck_final.log is the second run (after the header-based 0x00899DB0 test): 7 / 7 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Decisions for the coordinator
- D2: the `??1CAFile@@UAE@XZ` copy 0x00B61FF0 now defines (5 B, NEQ v 0x0099A300, SAME as landed 0x009D0380's): land as debt, as nf254 D1.

## Flags / funclets / tests / descriptive names / locks
- Flags: none of the four rows has CompilerFlags; none needs one. Funclets: no file under src/compiled, tests or include names any of the
  four source basenames (grep of each, the 01/21 / 01/22 Unwind rows included).
- Tests: 0x0063E480 / 0x00B61CD0 / 0x00B61FF0 keep their HEAD tests (PASS); 0x00899DB0's is new, on the shared headers.
- Descriptive names: none added. Locks (rows.txt, NF256): 0063e480 0049c490 0049be90 00899db0 00733ee0 00b61cd0 00b61ff0 00b620a0
  00b61dd0 009d0380 (4 changed; the rest are controls / unchanged).
