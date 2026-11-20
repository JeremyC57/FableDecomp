# NF181 notes (2026-11-20, ctor / dtor slice 1, classes C0..CH)

## Census method
- One resumable compile probe of every REFS row (2,582 row/target lines; `work/probe_small.tsv`, `work/probe_big.tsv`), summarised per
  target in `work/census.tsv` (RIGHT rows omitted; WRONG = binds another spelling; FAIL = the row does not grade plainly; SKIPPED = held by
  NF178 / NF178b / NF179 / NF180 / acctB, taken from the table only). Rows held by others were not compiled (`work/skip_rows.txt`).
- The binding table is not a census: three stand-in bindings had no table line (0x00CA73D0 `?Dtor_00ca7310@T@@QAEXXZ`, 0x00AA4150
  `?C3DAnimationManager_dtor@@YIX...`, 0x00953960 `?CCreatureAction_DragonTakeOffIntoFlying_dtor@@YIX...`), and several table lines are
  stale (0x00A4FE60 binds ??1CDataStream now, the table still shows ??1CDataInputStream).

## Pick-any / defined-symbol checks (every changed row: objdump defined/undefined diff against the HEAD probe object, `work/symchk.sh`)
- 61 access-only rows, 7 q11676 rows, 0x00951B20, 0x0064B6DA, 0x009211D0: only the intended undefined symbol changed.
- 0x008A8E40 / 0x0095B280: the temporary's implicit-dtor COMDAT changes name with the class / virtual-ness (no table owner).
- 0x00485CF1: new COMDATs ??1CDataInputStream / ??1CMemoryDataInputStream / ??1CPersistContext are the shared-header implicit dtors,
  cmpbody-identical to landed includer 0x00597228 (0x0047F58B, 0x004A21F0, 0x00A4FE60 define them at HEAD too). Their table lines map them to
  0x00993B10 / 0x009BBBF0 only through stand-in binders (0x009BBBF0 is CPersistContext::CheckUnknownSymbols: a wrong table association).
- 0x009568E0: ??0CCreatureAction_ThrowShotObject, ??_7, ??_G, ??2 placement new: cmpbody-identical to the owner 0x009566C0 HEAD object.
- 0x00485420: ??0CMemoryDataInputStream bytes equal to owner 0x00485400 (relocations differ only by the owner's
  stand-in base names); ??_7CMemoryDataInputStream@@6B@ (44 bytes) identical to the 11 HEAD header includers, while the owner's 4-byte
  stand-in vtable already differed at HEAD (stand-in DATA COMDAT rule).

## Coordinator decision needed (pick-any)
- 0x00485CF1 / 0x00485420 newly define shared-header COMDATs whose table lines point at other addresses: ??1CDataInputStream@@UAE@XZ /
  ??1CMemoryDataInputStream@@UAE@XZ (table: 0x00993B10), ??1CPersistContext@@QAE@XZ (table: 0x009BBBF0) and, for 0x00485420,
  ??0CMemoryDataInputStream@@QAE@XZ (table: 0x00485400). These were compared with HEAD header includers (identical), not with the table owners:
  the 0x00993B10 / 0x009BBBF0 owner rows define other functions under those addresses (the lines come from stand-in binders; that the
  0x009BBBF0 line is wrong rests on its catalog label CPersistContext::CheckUnknownSymbols only). 0x00485400's own ??0CMemoryDataInputStream
  is a strong out-of-line definition on a stand-in base whose bytes equal the new copy (relocations differ), so linking it beside any header
  includer gives LNK2005; five HEAD objects (0x0047F58B, 0x004A21F0, 0x00520670, 0x00597228, 0x006F9F90 ...) already define the same pick-any
  copy. Hold 0x00485420 / 0x00485CF1 if those HEAD copies are not accepted as licence.

## Tests
- 0x00686800 and 0x0080C250: their standalone tests model CBaseClassNonCopyable with a public ctor and failed to link after the access fix;
  the test-local ctor was made protected in place (rows/<addr>_test.cpp), PassPatterns unchanged. These tests remain standalone host models
  of a header class (pre-existing); not rewritten onto the headers this round.

## No casts, no new local models, no overlay
- No stand-in was cast to a real type. No class token was added to any row except through shared headers (008A8E40 now includes
  engine/CCombatAction_GenericResponseToDecapitation.h; 00485CF1 / 00485420 / 009568E0 moved onto headers). In-place respells only inside
  local models present at HEAD (0x00951B20, 0x0064B6DA, 0x0095B280, q11676 rows, 0x009211D0, 0x00982C70, 0x00693C70, the 61 access rows).
- 0x0095B280: the local CCreatureActionBase's manual `void* __vftable` became `virtual ~CCreatureActionBase()` (same layout) so the
  CCombatActionBase dtor could be virtual.
- No Flags lines added. No EH funclet includes any changed row.

## Self renames (RENAMES s1) and the split gate
- 0x00982C70, 0x00693C70: every referencing row binds the new spelling in this hand-back.
- 0x009568E0, 0x00485420: no landed row references them (exe scan); vtable slots only. 0x009568E0's build entry Module reads
  CCombatAction_WillShieldInto (stale; catalog name is CCreatureAction_ThrowShotObject::`scalar_deleting_destructor').

## Locks
- Claimed: 0x00CA73D0 0x00953960 0x00AA4150 0x00842460 0x00843F50 0x00854610 0x00854660 0x008546C0 0x009B08C0 (was released) 0x00CA9D00
  0x00CAEFB0 0x00CAF480 0x00496070 0x00485420 0x0095B280 0x00485CF1 (late, see slip) and the 61 access rows (several were `released`).
- Rule slip: 0x00485CF1 was edited and graded before it was claimed; claimed afterwards (it was free) before hand-back.
- 0x00A5C570 is locked by NF182 (binds ??0CBaseClassNonCopyable@@QAE@XZ): deferred.

## q11677 rows left (bind the pointer form ...@@QAE@PAVCThingCreatureBase@@PAVCThing@@JABVCSoundPair@@@Z; self row 0x00857F30 too)
0x0066A690 0x00739A00 0x0074DAC0 0x00855AF0 0x0085C190 0x0087CF90 0x0087FD20 0x0087FDF0 0x008B7F80 0x008B8200 0x008B83E0 0x008B84A0 0x008B8500 0x008BB0B0 0x008BB130 0x008BCE50 0x008BCFB0 0x008BFA60 0x008BFAC0 0x008BFB20 0x008BFB80 0x008BFBE0 0x008BFC40 0x008BFCA0 0x008BFD00 0x008BFD60 0x008BFDA0 0x008C27B0 0x008C39A0 0x008C4EA0 0x008C4F50 0x008C7390 0x008C7400 0x008C9410 0x008C94D0 0x008C95B0 0x008C9CC0 0x008CA7C0 0x008CBCA0 0x008CBCF0 0x008CBD90 0x008CCB80 0x008CCC10 0x008CCC70 0x008CCCD0 0x008CCD30 0x008CE040 0x008CE290 0x008E45B0 0x008E4640 0x008E46D0 0x008E49C0 0x008E6A70 0x008E6BB0 0x008EDA90 0x008F4D00 0x009067B0 0x00906A80 0x00906B40 0x00908000 0x009080B0 0x009081F0 0x00908270 0x009082D0 0x00908340 0x009083A0 0x00908400 0x00908C80 0x00909E30 0x00909FD0 0x0090A1F0 0x0090A690 0x0090A700 0x00925330 0x00925440 0x009254C0 0x00925560 0x009255E0 0x00925700 0x00953120 0x009531C0 0x00953260 0x00953300 0x00953370 0x009535B0 0x00953630 0x00957170 0x0095CB80 0x009621F0 0x00962920 0x0096AF60 0x0096C010 0x0096DB60 0x0096DDB0 0x0096FF10 0x009702D0 0x00976F30 0x00976FC0 0x00977060 0x0097B3F0 0x0097C760 0x0097C830 0x0097C920 0x0097CF90 

## Finding (NAME_FIXES.tsv)
- 0x00C10940 and its vtable family 0x012B263C / 0x012B2644 (0x00C108E0, 0x00C109A0, 0x00C109B0, 0x00C109F0, 0x00C10A10) are VC7.1
  <stdexcept> logic_error-family code, not CCardPositionsDef. logic_error vs runtime_error is not separable from the bytes (one source: the
  catalog's std::logic_error::what label), so the spelling there is descriptive-by-library.

## Blocked by the TU-emitter rule (??_G rows whose class has no inline constructor)
- 0x00CA73D0 (CBaseVideoRenderer), 0x00953960 (CCreatureAction_DragonTakeOffIntoFlying), 0x00AA4150 (C3DAnimationManager), and the q4411
  ??_G stand-ins 0x00739CC0 0x007F05F0 0x00868350 0x008C9690 0x008CED90 0x008CEDD0 0x009704B0: the nf86 scaffolding works only where the
  shared header has an inline constructor (it did for ThrowShotObject and CMemoryDataInputStream).
