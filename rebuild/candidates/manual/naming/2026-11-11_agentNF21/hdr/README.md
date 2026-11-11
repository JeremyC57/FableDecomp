# NF21 header opt-ins

Full copies of the current repo headers (`rebuild/include/engine/`) with `#ifdef` opt-ins added. Nothing changes for a
TU that does not define the macro. Graded with `FABLE_HDR_OVERLAY=$A/hdr`.

| Header | Opt-in macro | What it adds | Used by |
|---|---|---|---|
| `engine/CTCBuyableHouse.h` | `FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE` | Declares `SetWifeLivingHere(CThing*)` (also the existing `FABLE_RETAIL_CTCWIFE_MARRY` block). Splits the opaque pad so that `CIntelligentPointer<CThing> WifeLivingHere` sits at +0x38 (Ego_r.pdb member). Includes `engine/CIntelligentPointer.h`. sizeof is unchanged. | 006d3c40 |
| `engine/CDataOutputStream.h` | `FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITEUBYTE` (nested under `FABLE_RETAIL_CDATAOUTPUTSTREAM`) | Declares `void WriteUBYTE(unsigned char)` at 0x00993E50 (Ego_r.pdb 0x00AFBE90 / FableWin.pdb 0x03082760 `?WriteUBYTE@CDataOutputStream@@QAEXE@Z`). Declaration only. | 00a8e6c0, 00aedaf0 |
| `engine/N3DPrimitive_CStaticBlock.h` | `FABLE_RETAIL_CBASEBLOCK_SAVEBINARY` (default branch) | Declares `N3DPrimitive::CBaseBlock::SaveBinary(CDataOutputStream&)` at 0x00A8E6C0 (Ego_r.pdb 0x00BD0F70 / FableWin.pdb 0x032FECF0). Declaration only. | 00a8e6c0 |

Header debt I noticed but did not change (the coordinator can decide):
- `engine/CMessageEventManager.h` uses `FABLE_STATIC_ASSERT` but does not include `rebuild_abi.h`. Rows that include it
  first have to include `rebuild_abi.h` themselves (004ff390, 006909a0, 00706950, 00842f70, 00890470, 008904e0, 007b0950).
- `engine/CThingFilter_IsNot.h` needs `CThing` declared but does not forward-declare it (006a76a0 adds `class CThing;`).
