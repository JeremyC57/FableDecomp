# Chunk 834 header overlay

## engine/CV_DemonDoorsScript.h (opt-in branch, default path unchanged)
- What: inside class CDF_ThreeSuits, after `virtual void Main();`, an `#ifdef FABLE_RETAIL_CDF_THREESUITS` block declaring the three
  private non-virtual outfit tests `bool DressedAsBandit(); bool DressedAsGoodKnight(); bool DressedAsEvilMage();`. No layout change
  (non-virtual members), no new forward declarations outside the `#ifdef`, no reflowed lines.
- Evidence: Ego_r.pdb and FableWin.pdb `?DressedAsBandit@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@AAE_NXZ` (0x00E72F00),
  `?DressedAsGoodKnight@...@@AAE_NXZ` (0x00E730A0), `?DressedAsEvilMage@...@@AAE_NXZ` (0x00E731F0); retail CDF_ThreeSuits::Main
  0x00E72630 calls all three with ecx = this (esi). The two landed siblings 0x00E730A0 / 0x00E731F0 already use these Leafs.
- Used by: parked row differ/00e72630.cpp (`#define FABLE_RETAIL_CDF_THREESUITS` before the include).
- Freshness: copied from rebuild/include at edit time; re-diffed before hand-back: the only difference is the 8-line opt-in block.
- Includers rechecked (read-only, recheck.py + `vland.py --recheck-landed`, FABLE_HDR_OVERLAY set to this hdr/): all 19 direct includers in
  INCLUDERS_CV_DemonDoorsScript.txt are MATCH / RELOCATION_MATCH + PASS (SIBLINGS.tsv). Indirect includers through
  engine/CQ_PrisonRaceScript.h and engine/CQ_BanditCampBossBattleScript.h resolve the repo copy (same-directory lookup) and see the default
  path either way.
