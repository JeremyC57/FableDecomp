# NF95 header overlay (2026-11-19)

Each overlay header is a copy of the CURRENT repo header (base copy in `../orig/engine/`, checked byte-equal to the repo at hand-back), with
`#ifdef` opt-ins only. Without the opt-in macros the default path is token-identical to the repo header, except where pin_asserts.py
respelled a moved `FABLE_STATIC_ASSERT(` as `FABLE_STATIC_ASSERT_AT(<old line>, ...)`, which keeps the typedef name (`fable_static_assert_<old line>`)
and so the compiler input. No landed row defines any of the new macros (grep of rebuild/src/compiled: 0 hits each), so the opt-ins change
none of the landed includers; the includer lists are in `../work/users_<header>.txt`.

Read NF93 / NF94 overlays first: neither declares any member added here. (Both of them add `FABLE_RETAIL_CCATEGORY_REMOVEITEM` to
CCategory.h; that is theirs, not mine.)

| header | opt-in added | declares | used by | pin_asserts | landed includers |
|---|---|---|---|---|---|
| engine/NPlayerGui_CDrawQuestInfo.h | FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLPUSHBACK | public `void OracleSymbolPushBack(const CCharString&)` (0x0064A3A1), `long GetScriptSpriteBySymbol(const CCharString&) const` (0x0064A37B) | rows/0064a3a1.cpp | pinned 0 | 32 |
| engine/CCategory.h | FABLE_RETAIL_CCATEGORY_HIGHLIGHTPREVIOUSITEM | public `void HighlightPreviousItem()` (0x005BD0F7), in the PASS22 class after the UNIQUEITEMCOUNT block | rows/005bdcdc.cpp | pinned 0 (all asserts above) | 48 |
| engine/CTCInventoryClothing.h | FABLE_RETAIL_CTCINVENTORYCLOTHING_PROCESSBUTTONARELEASED | `virtual void ProcessButtonAReleased()` (0x005B34D5; needs ..._CALCULATE for the base) | rows/005b34d5.cpp | pinned 10 | 30 |
| engine/CTCInventoryWeapons.h | FABLE_RETAIL_CTCINVENTORYWEAPONS_PROCESSBUTTONARELEASED | `virtual void ProcessButtonAReleased()` (0x005C2E2A) | rows/005c2e2a.cpp | pinned 5 + 1 by hand (line 150, the assert with a trailing comment the tool will not pin) | 48 |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_CTCINVENTORYBASE_GETPDEF_INLINE (nested, `#ifndef` around the PASS22 block's out-of-line `GetPDef() const` declaration) | lets a PASS22 TU use FABLE_RETAIL_CTCINVENTORYBASE_PDEF's inline GetPDef (retail inlines it) | rows/005bd4ce.cpp | pinned 0 | 67 |
| engine/CTCBase.h | FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS | protected `static std::list<CTCBase*> GlobalUpdateTCs` (0x013BA8AC; Ego_d.pdb PUB32 `@@1`; needs stlport/fable_containers.h first) | rows/00686df0.cpp | pinned 0 | 1191 |

The virtual `ProcessButtonAReleased` overrides add no vtable slot (the base CTCInventoryBase already declares the virtual) and no data.

## Coordinator note (2026-11-19, after landing part 1 as nf95a)
Part 1's opt-ins are now in rebuild/include (merged with NF93's CCategory.h SELECTITEMOFTYPE / REMOVEITEM blocks). Every header in hdr/ and
orig/ was replaced with the CURRENT repo header (which contains your part-1 opt-ins), so hdr/ == orig/ == repo now. The pre-reset copies are
in ../part1_overlay_snapshot/. Add part-2 opt-ins to these copies only.

## Part 2 (2026-11-19, resumed NF95)
Same rules as above: base copies in `../orig/engine/` equal the current repo header; opt-ins are `#ifdef` only; the default path is
token-identical except for pin_asserts' `FABLE_STATIC_ASSERT_AT(<old line>, ...)` respelling. No landed row defines any new macro.

| header | opt-in added | declares | used by | pin_asserts | landed includers |
|---|---|---|---|---|---|
| engine/CIsHornTaskWithThisHorn.h | FABLE_RETAIL_CISHORNTASKWITHTHISHORN_CTOR_OUTOFLINE (`#ifdef` / `#else` around the inline constructor; the `#else` line is the unchanged original) | `CIsHornTaskWithThisHorn(CIntelligentPointer<CThing>)` declaration only (0x0065F0FA, FableWin.pdb 0x02160C17) | rows/0065f0fa.cpp | pinned 1 (line 18) | 1 (0x0065BE6C; probe-compiled with the overlay: unchanged, RELOCATION_MATCH, binds 0x0065F0FA under the new name) |
| engine/CGameEventDataPackage.h | FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE (nested in the FABLE_RETAIL_CGAMEEVENTDATAPACKAGE class) | `unsigned __int64 GetUHUGE()` declaration only (0x00628E80, FableWin.pdb 0x023D5E10) | rows/00628e80.cpp | pinned 1 | 0 |
| engine/CCodaVector.h | FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE (`#ifdef` / `#else` around the in-class OrthogonalRight body, in both class branches; the `#else` text is the unchanged original) | `void OrthogonalRight(const CCodaVector&, const CCodaVector&)` declaration only (0x00C40370, GameCODA CodaListener.obj COMDAT) | rows/00c40370.cpp | pinned 0 (no asserts) | 1 direct landed includer (0x00C40590 via engine/CCodaListener.h; 9 landed rows reach it through CCodaListener.h / CodaProcessingTypes.h / CSalPlatform.h / CCoda3DSource.h): default path unchanged, no landed row defines the macro |
