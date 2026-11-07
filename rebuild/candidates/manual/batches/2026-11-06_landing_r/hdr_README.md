# Chunk 817 header overlay

Every header here is either a new header behind `FABLE_RETAIL_NSCRIPT` (no `#error` branch) or a repo header with one new
opt-in `#ifdef` block. The default path of every repo header is textually unchanged (checked with plain `diff` against
rebuild/include on 2026-11-07), so no landed includer can change bytes and no includer recheck was needed.

| Header | Kind | Change | Used by | Evidence |
|---|---|---|---|---|
| engine/CQ_TraderEscortScript.h | opt-in `FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_SETBRAINSTATE` (new, chunk 817) | adds `void SetBrainState(long);` to CDarkwoodTrader (private in the PDB; declared in the existing access region) | 0x00E07640 | Ego_r.pdb + FableWin.pdb `?SetBrainState@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@AAEXJ@Z`; the body 0x00E0A510 stores its argument at +0x20 (BrainState in struct_layouts_egor.tsv) |
| engine/CV_FisticuffsClubScript.h | new header (chunk 813 draft, byte-identical copy) | CV_FisticuffsClubScript layout / predicates for the club | 0x00E87030 | see chunk 813 hdr/README.md; CTyler itself is already declared in the repo's engine/NScript_CV_FisticuffsClubScript.h |
| engine/CQ_GuildTrainingWoodsDepartureScript.h | new header (chunk 814 draft, byte-identical copy) | CFinalMaze entity model | 0x00D647F0 (parked) | see chunk 814 hdr/README.md |
| engine/CQ_TraderConflictGoodScript.h | opt-in `FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_TRADERTORESCUE` (chunk 815 draft, identical) | CTraderToRescue entity class | 0x00DFE0F0 (parked) | see chunk 815 hdr/README.md |
| engine/CV_ChapelOfEvilScript.h | opt-in `FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL` (chunk 814 draft, identical) | CCOE_AcolyteEvil entity class | 0x00E6BC50 (parked) | see chunk 814 hdr/README.md |

If chunks 813 / 814 / 815 land first with these same drafts, only the CQ_TraderEscortScript.h SetBrainState opt-in is new
from this chunk. The other four files carry no chunk 817 change and can be dropped then.
Includers rechecked: none needed (no default path changed).
