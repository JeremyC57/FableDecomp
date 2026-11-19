# NF103 overlay (round 10, 2026-11-19)

Base copies: `orig/engine/` (repo headers at session start). Edited copies: `hdr/engine/CThingSearchTools.h`, `hdr/engine/NThingMap_CThingMap.h`.
engine/CTCWeapon.h is NOT overlaid: q11533 uses NF101's landed opt-in FABLE_RETAIL_CTCWEAPON_GETMISSILELOADCARRYSLOT unchanged.
NF102 / NF104 overlays hold none of these headers (checked: NF102 has four reverb headers, NF104 none).

## engine/CThingSearchTools.h
1. **Opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_STATICS`** (generated / non-retail branch, inside the class after `AreaMaps[0xc]`):
   `static unsigned long NoLookups,NoThingsScanned,SearchID;` (Ego_d.pdb ?NoLookups / ?NoThingsScanned / ?SearchID@CThingSearchTools@@2KA,
   0x013B89A8 / AC / B0). Public, as the class head is `public:`. Without the macro the default path is token-identical.
   Users: the 26 CThingSearchTools_Calls rows (t/stat_c.py) and 0x004162B5.
2. **Header claim (brief specifics 1-2, owned this round):** the ROUGHAREABODY body and the two TCSINROUGHAREA_BODY bodies spelled the stand-ins
   SearchCount_013b89a8 / SearchWork_013b89ac / SearchID_013b89b0 / NThingMap::g_NoMapwhoLevels / LayerInfo_0137dad8; they now spell
   NoLookups / NoThingsScanned / SearchID / NThingMap::MaxLayers / NThingMap::LayerInfo (21 body lines). The statics declaration gained an
   `#elif defined(ROUGHAREABODY) || defined(TCSINROUGHAREA_BODY)` branch (statics only: those blocks declare GetAllThingMapsInArea themselves).
## engine/NThingMap_CThingMap.h
3. **Header claim:** the two `#if defined(BESTROUGH_BODY) || defined(ORIGINAL_ROUGHAREA)` conditions (CRGBColour include; MaxLayers /
   CLayerInfo / LayerInfo declarations) also take ROUGHAREABODY / TCSINROUGHAREA_BODY; the `#else` branch's stand-in declarations
   (g_NoMapwhoLevels, global struct CLayerInfo, LayerInfo_0137dad8[], SearchCount_013b89a8, SearchWork_013b89ac, SearchID_013b89b0) are retired
   (a comment says so). Nothing in rebuild/src/compiled uses them through the header (grep: every user declares its own extern or only names
   them in comments).

## pin_asserts
`pin_asserts.py rebuild/include hdr engine/CThingSearchTools.h engine/NThingMap_CThingMap.h`: pinned 0 / 0 (the generated-branch asserts were
already FABLE_STATIC_ASSERT_AT; no other assert moved).

## Header users (gate)
`t/ppgate.py`: cpp -P of every landed row naming CThingSearchTools.h / NThingMap_CThingMap.h / CTCMapwho.h (355 rows), repo headers vs this overlay
(rows/ versions where present): 43 differ (work/ppgate1.tsv) = 26 Calls rows of mine (the STATICS opt-in line, graded) + 17 header users
(the ROUGHAREABODY / TCSINROUGHAREA_BODY users and 3 rows that only lose the unused `#else` stand-in declarations). All 17 were rechecked:
16 unchanged RELOCATION_MATCH PASS (work/b4.log); 0x00435530's test defined the retired stand-ins, so rows/00435530_test.cpp now defines the
PDB names (RELOCATION_MATCH PASS, work/b5.log). The other 312 includers preprocess identically. B's 0x0063F4B0 is unchanged (identical cpp -P).
All 94 rows (77 changed + 17 users) are in recheck_final.log.

## Later edits
- NThingMap_CThingMap.h lines 122-124: the stale comment about the landed stand-in spellings rewritten in place (same three lines;
  comments only, so cpp -P is unchanged).
- Test gate (t/ppgate_t.py): the 41 landed tests outside the 94 rechecked rows that name the three headers preprocess identically
  with this overlay (work/ppgate_tests.tsv, 0 differ).
