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
