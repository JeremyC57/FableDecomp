# w24A header overlay (grade with FABLE_HDR_OVERLAY=<this dir>)

Every change is behind an opt-in; the default (no opt-in) preprocessed text of each header is identical to the repo copy
(checked with `g++ -E -P` on a one-include TU, old tree v overlay). No FABLE_STATIC_ASSERT( line moves unpinned.

| header | opt-in | what | row |
|---|---|---|---|
| engine/CArray.h | FABLE_RETAIL_CARRAY_INDEX_ASSERT (new; first arm of the index-operator chain; includes fable_assert.h) | `operator[]` with the original's bounds ASSERT as the release dead arm (`FABLE_RETAIL_ASSERT(index < this->size())`, then vector::operator[]); the row defines FABLE_RETAIL_ASSERT_DEAD_ARM. Emits no bytes; makes the second access to one element number its index product again (`lea ecx,[eax+ecx+4]`, 0x004706C9). No assert lines in this header. | 0x0047042A |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_CURFEW (existing, Q15; no landed user) | lines 154 and 156 edited IN PLACE (no line added or moved): the opt-in now declares `bool IsCurfewTime() const;` (0x00658DB3) and `bool IsSociableTime() const;` (0x00658D5D). Q15 had bound IsCurfewTime to 0x00658D5D, which is the landed ?IsSociableTime@CTCVillage@@QBE_NXZ. | 0x0090C900 |
| engine/CThingObject.h | FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS (new, 6 lines before the class end) | `static unsigned long __fastcall StatGetTypeFlags() { return 0x807; }` (FableWin.pdb ?StatGetTypeFlags@CThingObject@@SIKXZ). The size assert that moved is pinned: `FABLE_STATIC_ASSERT_AT(104, sizeof(CThingObject)==0xe0)`. | 0x00959380, 0x0095C7C0 |
| engine/CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET (new, first arm of the cell expression inside FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA) | FITTED cell expression: the layer's level-info record addressed as bytes (the landed family K016 spelling). Needs `<stddef.h>` in the row. All asserts of this header are already `_AT` pinned. | 0x00959380, 0x0095C7C0 |
| engine/NCameraModes_CStringAndCageBase.h | FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_AUTOGOBEHIND (new) | declaration `void UpdateAutoGoBehind(C3DVector& go_to) const;` (Ego_r / FableWin, private const, 0x00872110) | 0x00872110 |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS (new; inactive when the STATIC_FOCAL + UPDATE_LIST chain already declares it) | declaration `C3DVector GetCameraFocalPos(float) const;` (FableWin name, 0x004C12D0) | 0x00872110 |

`../hdr_exp/` holds the experiment copies (macro hooks for the PeekCell / rough-area probes); not for landing.

Base copies the overlay files were made from (md5 of rebuild/include/<header> at hand-back; a different md5 at landing means the base moved):
- engine/CArray.h edba43ddd0e1e522cbcdd911ae872f1b
- engine/CTCVillage.h c9cb5984b7939c438fa9764fc6a0b128
- engine/CThingObject.h 215b603f6d7efecf66c5a558177b2387
- engine/CThingSearchTools.h cb3da18c37ff4b503b5fcac6ef525963
- engine/NCameraModes_CStringAndCageBase.h ab865e13a0cc7f2b4d55d205508c7d6b
- engine/NGraphicAppearance_CTCGraphicAppearance.h b74be9258cc6291484504c6b510aa81c
