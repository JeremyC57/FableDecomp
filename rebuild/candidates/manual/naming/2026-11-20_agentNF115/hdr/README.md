# NF115 header overlay

Every overlay header is a copy of the CURRENT repo header (base copy in ../orig/) plus `#ifdef` opt-ins only. Without the opt-in macros
each header is token-identical to the repo copy (the diff is wholly inside the new `#ifdef` blocks).

## engine/CThingSearchTools.h
- `FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST` (nested inside FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY's GetPBestThing body):
  the scan and type tests, then the element re-read as `tthing` and tested for null before the filter (retail 0x0065F776 `test edi,edi ; je`;
  0x004AC304 has the same test). Used by rows 0x0065F744 and 0x00659800. The `#else` branch is the repo text unchanged.
- pin_asserts.py: pinned 0 (the header's asserts are already FABLE_STATIC_ASSERT_AT with explicit lines).
- Landed includers of engine/CThingSearchTools.h: 334 rows (`grep -rlE 'CThingSearchTools\.h' rebuild/src/compiled`). None defines
  FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST (new name), so the opt-in changes none of them.

## engine/CThingCreatureBase.h
- `FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETPROPERTY` (flattened view, beside NF83's FLAT_CLEARCURRENTACTION): declares
  `unsigned long GetProperty() const;` (?GetProperty@CThingCreatureBase@@QBEKXZ, 0x00662010), the same declaration as the retail branch.
  Declaration only, no layout change. Used by rows 0x0065C781, 0x00936BA0, 0x00937D20.
- pin_asserts.py: pinned 0 (all asserts are FABLE_STATIC_ASSERT_AT).
- Landed includers of engine/CThingCreatureBase.h: 546 rows. None defines FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETPROPERTY (new name), so the
  opt-in changes none of them; without it the header is token-identical to the repo copy.
