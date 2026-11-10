# agent1011b header overlay (export FABLE_HDR_OVERLAY=$A/hdr for every grade)

Copies of the repo headers at HEAD with one new opt-in each; the default path is byte-identical.

## engine/CTCInventoryMap.h -- FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW (new)
Rows 0x005FC2D0 / 0x005FC8A0 (re-authored landed rows, `rows/`). Declarations only, no layout change:
- `NUISystem::CComponent* ConstructQuestReviewList(NUISystem::CComponent* parent);` 0x005FC2D0,
  Ego_r.pdb 0x0064C6C0 / FableWin.pdb 0x02088CA0 `?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z` (private).
- `NUISystem::CComponent* ConstructQuestReviewListPC(NUISystem::CComponent* container, NUISystem::CComponent* parent);` 0x005FC8A0,
  Ego_r.pdb 0x0064CE90 / FableWin.pdb 0x020AEC70 `?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z` (private).
- The `namespace NUISystem { class CComponent; }` forward declaration sits under the same opt-in.

GivePrize 0x005EB590 and the differ 0x005FBAA0 need no overlay change (only committed opt-ins).
