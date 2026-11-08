# hdr/ -- header overlay for chunk 864 (FABLE_HDR_OVERLAY)

Every file is the repo's `rebuild/include/engine/<Class>.h` plus additive opt-ins (`#ifdef FABLE_RETAIL_...` blocks) that
the rows define; nothing outside an opt-in block changes (`diff` against the repo header shows only `>` lines). The lines
inherited from the parked chunks' overlays are listed in those chunks' hdr/README.md files
(`rebuild/candidates/manual/pending/2026-11-08_linux_hand856/hdr`, `.../hand860/hdr`).

| header | from | lines | what |
|---|---|---|---|
| CQ_EndGameBossBattleScript.h | hand856 | +22 | CTheRealGuildmaster layout opt-ins for 0x00D33730 |
| CQ_FireHeartScript.h | hand856 | +8 | CFH_Prisoner / parent layout for 0x00D3A4A0 |
| CThingAICreature.h, CThingVillage.h | hand856 | +5 each | StatGetType opt-ins (0x0050A650) |
| CPersistContextReadSection.h, CStringParser.h, CDataInputStream.h | hand856 | +8/+5/+10 | ReadLine / ReadPresized / ReadUHUGE declarations (0x005253F0) |
| CThingManager.h, NThingManager_CEntityRuntimePersistence.h | hand856 | +16/+8 | LoadGameState members (0x005253F0) |
| C2DCoordI.h, C2DBoxF.h, CNavigationPosition.h | hand856 | +13/+6/+6 | ToC2DCoordF, corners, ctors (0x0050A650) |
| CQ_PrisonRaceScript.h | hand856 | +12 | layout for 0x00DDAE40 |
| C3DMeshFileBoneChunk.h, C3DMeshFileSubMeshChunk.h | hand860 | +9/+7 | bone/sub-mesh chunk layouts (0x00A8B990) |
| CRenderStateManager.h | hand860 | +11 | bookmark members (0x009D8CF0 / 0x009D9C80) |
| CThingSearchTools.h | hand856 **+ chunk 864** | +60 | hand856's TCSINROUGHAREA declarations **plus the new nested opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY`** (retail's rough-area walk as the real templates; see below) |
| CThing.h, CTCDriverBase.h, CTCDRegionExit.h, CTCDNavigationSeed.h | **chunk 864** | +6/+4/+3/new | opt-ins the TCSINROUGHAREA_BODY templates need (friend, slot-28 `IsDriverType`, `StatGetDriverType`); see below |

## New in chunk 864: `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY` (CThingSearchTools.h)

Nested inside `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA`. Gives `GetAllTCsInRoughArea<T,Filter,Container>` and
`GetAllTCDsInRoughArea<T,Filter,Container>` (PDB template order) same-TU `__declspec(noinline)` bodies that are RETAIL'S OWN
ALGORITHM, translated from the landed 0x005185F0 / 0x00518900 rows (`rebuild/src/compiled/00/51/CThingSearchToolsCalls_GetAllTCDsInRoughArea_*.cpp`)
and the 0x00518340 instance: `GetAllThingMapsInArea(area, AreaMaps)`; `++SearchCount_013b89a8; SearchWork_013b89ac = 0; ++SearchID_013b89b0`;
per map `ClipAndConvertWorldMapAreaToLocalArea`; per layer (`NThingMap::g_NoMapwhoLevels`, `LayerInfo_0137dad8[layer].ScaleShift`) the
scaled cell box, skipped when empty; per cell the `CMapwhoNode` list; per node the `CTCMapwho` search-ID stamp
(`GetSearchID() < SearchID_013b89b0` -> `SetSearchID`), `GetParentThing()`, then the type test -- TCs: `CThing::GetTC<T>` (HasTC flag + Components.Find);
TCDs: `thing.HasTCDriver() && thing.GetTCDriver().IsDriverType(T::StatGetDriverType())` (slot-28 virtual) (0x00518A54 `mov ecx,[edi+5Ch] ; test ecx,ecx ... push 19h ; call [edx+70h]`) --
then `filter(thing)` and `result.push_back(...)`; finally `AreaMaps.clear()`. No local pad structs: `this` is the real header class.
The clip helper's declaration (`FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA` / `_ROUGHAREABODY` guard) is also enabled by this opt-in.

The row opt-ins the bodies need, besides the usual CTCBASE / CTHING ones: `FABLE_RETAIL_CTCMAPWHO` + `FABLE_RETAIL_CTCMAPWHO_SEARCHID`
(CTCMapwho.h stamp accessors; forward-declare `class CTCMapwho;` before including it), `FABLE_RETAIL_CTHING_TCDRIVER`
(CThing.h, below), `FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE` (CTCDriverBase.h, below), `FABLE_RETAIL_CTCD_STATGETDRIVERTYPE`
(CTCDRegionExit.h / CTCDNavigationSeed.h, below); include NThingMap_CThingMap.h and the driver headers before CThingSearchTools.h.

Why the caller needs the bodies at all: with declarations only, 0x0050A650's vector locals were address-exposed and c2 coalesced the
free's `_M_start` reload with the loop iterator (md 6); with visible bodies the vectors are promoted locals whose web the allocator
splits, and the free reloads into eax as retail -- RELOCATION_MATCH (W1 with stand-in bodies, W2d with these faithful bodies, W3b with the PDB spellings IsDriverType(long) / HasTCDriver / GetTCDriver: same 2233 bytes; the template instances are byte-identical between W2d and W3b).
The three instances the TU emits are COMDATs of 748 / 748 / 778 bytes against retail's 597 / 597 / 683; they are the same algorithm but
not byte-verified in this lane (no retail bytes for 0x005185F0 / 0x00518900 / 0x00518340 here); the landed rows' own TUs define those addresses.

### Overlay copies added for the bodies (chunk 864)

| header | change |
|---|---|
| CThing.h (overlay copy of the repo header) | `#ifdef FABLE_RETAIL_CTHING_TCDRIVER` -> the PDB accessors `bool HasTCDriver() const` (?HasTCDriver@CThing@@QBE_NXZ 0x01CCC3F0), `CTCDriverBase& GetTCDriver()` (QAEAAV 0x01CCC420) and `const CTCDriverBase& GetTCDriver() const` (QBEABV 0x01CF9F90), inline: the `PTCDriver` (+0x5C) null test and the driver reference (0x00518A54 `mov ecx,[edi+5Ch] ; test ecx,ecx`). Placed outside the FABLE_RETAIL_CTHING method block. Replaces the first draft's `friend class CThingSearchTools` (coordinator 2026-11-08) |
| CTCDriverBase.h (overlay copy) | `#ifdef FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE` -> `virtual bool IsDriverType(long type) const;` slot 28 (+0x70): PDB name `?IsDriverType@CTCD<X>@@UBE_NJ@Z` for every driver (egor_pdb_names.tsv; J = long); CTCDRegionExit 0x00779940 `cmp ecx,19h ; sete al`, CTCDNavigationSeed 0x004D4222 `cmp [esp+4],15h ; sete al` |
| CTCDRegionExit.h (overlay copy) | `#ifdef FABLE_RETAIL_CTCD_STATGETDRIVERTYPE` -> `static unsigned long __fastcall StatGetDriverType() { return 25; }` (0x19 pushed at the 0x00518900 call) |
| CTCDNavigationSeed.h (new, no repo header) | `class CTCDNavigationSeed : public CTCDriverBase` (Ego_r size 0xc), `StatGetInterfaceType()` = TCI_NAVIGATION_SEED (88), and under the same opt-in `StatGetDriverType()` = 21 (0x15 pushed at the 0x005185F0 call) |

## New in chunk 864: `FABLE_RETAIL_CCHARSTRING_REPLACE_STRSTR_TEXT` (fable_string.h, overlay copy of the repo header)

Nested inside `FABLE_RETAIL_CCHARSTRING_REPLACE`: the Replace loop's second `strstr` spelled on `text` after the `text = found`
copy (retail 0x005257AA `mov edi,esi ; push edi`). Diagnostic only (0x005253F0 variants M4/M5: it raises `text`'s priority by
one loop-weight reference per expansion, which over-corrects the text/search_text pair and flips the function-level
this/context registers). No matched row uses it; kept for the hand-over so the M4/M5 attempts can be reproduced. The rest of
the file is the repo header verbatim.
