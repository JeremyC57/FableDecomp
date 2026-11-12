# acctB hand-back, Fable-lane chunk 4006 (1 of 2 rows matched; row 2 in work)

Two agents (Fable 5.2) worked this chunk in the same directory; the second closed row 1. Re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x005D1F0C `?ConstructPersonalityListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z` | RELOCATION_MATCH 17873v17873 (5617/5617 instructions), test PASS (g++ and VC7.1) | grade `retail_asm` (8 bare `fistp` sites through the shared helpers; table in `ASM_ATTEMPTS_TABLE.md`). No flags. About 86 whole-function grades |
| 0x005CE0E6 `?ConstructPersonalityList@CTCInventoryStats@@...` | NOT_ATTEMPTED | 15,910 bytes; a continuation agent is on it in the same directory with the same headers |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; five `WARN RELOC_NAME`, all catalog labels
that differ from the PDB names, listed in `LABELS.tsv`). Padded-struct grep clean. Grade the row with the Leaf as the
third q.py argument (the catalog name is `vfunc_89`).

## Review items

1. **`hdr/engine/GFPackFloat.h` changes a shared inline body and is NOT an opt-in.** One line:
   `return (GFFloatToLongNear(value * scale) / scale);` (the return expression parenthesised) in `GFRoundFloatNear`.
   Bytes of the helper are unchanged; the extra no-code tuple moves the scheduler's region cuts in this very long
   function (5611 -> 5616 of 5617). I regraded BOTH landed includers under the overlay:
   0x00AFBCE0 `CPSCRenderSprite::DatabaseSerialise` RELOCATION_MATCH and 0x00B027D0
   `CPSCRenderMesh::DatabaseSerialise` RELOCATION_MATCH (the agent had only found the first). The agent also graded
   the out-of-line `GFPackFloat` copy against 0x00AF8D50 (match). Pending rows of other lanes that include this header
   (`2026-11-04_linux_hand738/740`, `2026-11-03_linux_hand724`, `2026-11-11_linux_handH40f`) were NOT regraded.
   FITTED: `(float)(...)` gives the same bytes. Your call whether this goes in as the shared body or as an opt-in.
2. **`CTCHeroStats::GetNumberOfBuildingsOwned` is a made-up helper** (descriptive name, in no PDB):
   `return GetNumberOfHousesOwned() + GetNumberOfShopsOwned();`. Retail's `lea edx,[eax+ecx]` at 0x005D4698 (and
   0x005D00AB in row 2) needs one more inline level on the houses side; three forms give the bytes (this helper, the
   houses getter through an inner inline, a named local in the houses getter). FableWin lists plain 22-byte getters and
   no sum helper. FITTED.
3. **Same-TU body in the row file.** `CTCInventoryStats::ConvertMap` (0x005CD6B7) is defined ahead of the row over the
   shared headers (it grades RELOCATION_MATCH against 0x005CD6B7 itself); without it the loop entry at 0x005D2F77
   differs. The landed 0x005CD6B7 row uses local stand-in classes, so the two need pair-landing or a shared body.
4. **Fitted spellings:** `float biggestFish = GFRoundFloatNear(heroStats->GetBiggestFish(), 2);` as its own statement
   (frame-word evidence: measured packer weights), the three best times as `GFRoundFloatNear(x, 1)` (PROVEN by the
   helper being the first inline COMDAT after the function in FableWin and by the bytes), local names, and the two gap
   enumerators of the local `enum EOpinion` (values 0 / 2 / 4 are what retail pushes).
5. **Inline-budget evidence:** `GFRoundFloatNear` as plain `inline` is refused at all five sites (size 48, frequency
   13.5, size-TU limit 15); `CWideString::operator==` in ConvertMap likewise (size 40, frequency 25). Both are in the
   row's `// Inline-budget:` line.
6. **Test wide text:** the mock spells wide text `unsigned short`; two `const wchar_t*` overloads remain under
   `#ifndef _MSC_VER` for the g++ run only (same pattern as the landed test 0x00540980). The textual purity grep
   flags those two lines.
7. **Headers** (`hdr/README.md`): opt-in additions to `CTCInventoryStats.h`, `CTCHeroStats.h`, `CTCHero.h`,
   `CTCInventoryItem.h`, `COpinionReactionManager.h`, `NUISystem_CManager.h`; new `CTCHeroMorph.h`.
8. **Labels:** 27 proven names in `LABELS.tsv`, including 0x0057AA1B `?GetNumRegionsExplored@CTCHeroStats@@QBEJXZ`
   (landed stand-in spelling `GetNumberOfRegionsVisited`), 0x0057D23B `UpdateAppearanceStats`, 0x00429160
   `CCountedPointer<NUISystem::CAction>::ReleaseData`, and the two `std::_List_base` COMDATs.

## How to land

`rows/005d1f0c.cpp` + `rows/005d1f0c.test.cpp` with `hdr/engine/`, then `payload.json` through your usual vland. The
source is machine-generated from a statement map (agent's `tools/gen.py`); the generated file is what is handed in.
