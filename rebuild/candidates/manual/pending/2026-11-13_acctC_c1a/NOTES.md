# agentC1 (acctC lane) -- NOTES

Environment: as BRIEF.md. Overlay = hdr/ (README there). Overlay is rebuilt from scratch by `python3 tools/mkhdr.py tools/spec.py`
then `pin_asserts.py rebuild/include hdr <headers>` (see hdr/README.md); hdr_base/ is a pre-pin copy used for header probes.

## Row 1 0x005D1F0C CTCInventoryStats::ConstructPersonalityListPC -- RELOCATION_MATCH 17873, test PASS
## Row 2 0x005CE0E6 CTCInventoryStats::ConstructPersonalityList   -- RELOCATION_MATCH 15910, test PASS
- Port: the chunk-4006 drafts unchanged except: today's opt-ins FABLE_RETAIL_CTCINVENTORYSTATS_TIMESPENT (GetTimeSpentInGame),
  _CTCHEROSTATS_AGE, _CTCHEROSTATS_TROPHYWITNESSES (GetAge / GetBestTrophyWitnesses are already in the repo), FABLE_RETAIL_DEF_CLASS_BASE
  (today's CTCHeroMorph.h includes CDefPointer.h), the new opt-ins of hdr/README.md. No flags. Same-TU ConvertMap body kept (review
  item 3 of the f4006 README still applies: pair-landing with the landed stand-in 0x005CD6B7 or a shared body).
- GFRoundFloatNear: the stale overlay changed the SHARED default body; now a new opt-in FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_QUOTIENT.
  Measured: default body 10 hunks, existing GROUP_PRODUCT 3 hunks (0x005D52F9 `mov ecx,[ebp+60h]` one slot late), GROUP_QUOTIENT match.
- Twins regraded after every overlay change (both RELOCATION_MATCH; read-only vland: 2 wins, 0 RELOC_IDENTITY / CONFLICT / NOT_GENUINE,
  only the WARN RELOC_NAME catalogue labels of LABELS.tsv).

### The GetNumberOfBuildingsOwned question (brief item b) -- SETTLED: the helper did not exist; the rows match without it
Evidence that no sum helper existed (three independent sources, the first one retail itself):
1. RETAIL BYTES. The linker kept this TU's unreferenced inline-getter COMDATs: 0x005CC58E..0x005CC685 is a run of
   `mov eax,[ecx+off] ; ret` / `fld [ecx+off] ; ret` bodies in CTCHeroStats header order (+0x48 TotalMoneyEarned, +0x4C, +0x50, +0x30 Age,
   +0x9C, +0xA0, +0x80, +0x54, +0x178 .. +0x190, +0xD0 .. +0xDC, +0xE4 Houses 0x005CC5FD, +0xE0 Shops 0x005CC604, +0xE8, +0xEC, +0xF0,
   +0xF4, +0xFC, +0x100, +0x104, +0x108, +0x24, +0x110, +0x154, +0x140, +0x144, +0x148, +0x150, +0x13C, +0x98, +0x15C BiggestFish).
   VC7.1 emits a COMDAT for every used inline (our object has one for the helper when it is used, ?GetNumberOfBuildingsOwned@CTCHeroStats@@QBEJXZ),
   yet there is no sum body in the run nor anywhere in .text (byte search for e0/e4 loads followed by ret: only the two getters here,
   the getter pair 0x00BCE710/0x00BCE720 of another TU, and the inline sum inside CGameScriptInterface::GetNumHousesOwned 0x008A12A0).
2. FableWin.pdb: the debug COMDAT run after ConstructPersonalityList (0x021BC850..) is GFRoundFloatNear, GetMaxMoney, ... GetBestTrophyWitnesses,
   GetNumberOfHousesOwned, GetNumberOfShopsOwned, GetNumberOfBuildingsRented ... -- every inline it uses, no sum helper.
   No CTCHeroStats / CTCInventoryStats name containing "Building" + "Owned" in Ego_r / FableWin / Ego_d (Ego_d has no CTCHeroStats at all).
3. Anniversary Test PDB (third source): the full CTCHeroStats field list (LF_FIELDLIST 0x539D7) has GetNumberOfHousesOwned,
   SetNumberOfHousesOwned, GetNumberOfShopsOwned, SetNumberOfShopsOwned, Set/GetNumberOfBuildingsRented and no sum method; its
   ConstructPersonalityList locals (HeroStats, pTCHeroStats, Tattoos, grams, CreaturesKilledNames, items, ...) have no buildings local.
So the rows now spell NUMBUILDINGS `heroStats->GetNumberOfHousesOwned() + heroStats->GetNumberOfShopsOwned()` (the shape of the
script interface's GetNumHousesOwned too), and the helper is gone from header, rows and tests.

What still has to be FITTED: written plainly the sum gives `lea edx,[ecx+eax]` (retail 0x005D4698 / 0x005D00AB `lea edx,[eax+ecx]`).
c2 makes the record with the smaller id the SIB index; retail needs one more (freed) record on the houses side. Kept: the houses
getter's body through a named local, `long GetNumberOfHousesOwned() const { long houses = NumberOfHousesOwned; return houses; }` --
only PDB names, same /O1 out-of-line bytes as retail's COMDAT 0x005CC5FD. Against it: FableWin's /Od copy of the houses getter is 18
bytes / 8 instructions like the shops getter (a local would add instructions) -- FableWin is a later build (VS2010-era STL), so this is
the remaining FITTED item for the owner. Alternatives measured (all DIFFER by that one lea unless said): row-side H+S, S+H (loads swap),
named locals either order, `+=`, int local, casts (int / unsigned long / float / static_cast) on either side or the sum, a const
`pTCHeroStats` local (Anniversary name) for all const getters or only the sum, the getters declared in retail order, parenthesised
getter returns (houses or shops), `this->` in the getter, static_cast in the getter, the 3-argument CRGBFloatColour ctor (retail
COMDAT 0x005CC570 `ret 0Ch`, FableWin ??0CRGBFloatColour@@QAE@MMM@Z -- equally valid, not needed), row 2 compiled ahead of row 1 in
one TU, 1..7 dead outer locals, 1..24 discarded inline getter calls (record-wrap test: no flip at any count, so it is a free-list
effect, not a mod-N wrap). The prior lane's d4 / d8 (houses getter through an inner inline) also match but need another made-up name.
Decision for the coordinator: accept the fitted houses-getter body (no made-up name anywhere), or keep the rows parked.

### Names
Proven (two sources each): all callees in LABELS.tsv (carried from chunk 4006, re-checked by vland) plus the two getter COMDATs.
EOpinion enumerators now PROVEN by the Anniversary PDB (OPINION_MORALITY 0, RENOWN 1, SCARINESS 2, AGREEABLENESS 3, ATTRACTIVENESS 4,
LAST 5), no longer "gap names". Descriptive: local variable names (biggestFish, totalExperience, percentage, renownLevel, goodness, the
getter local `houses`). Anniversary locals that could replace some (third source only, rewritten function): HeroStats, HeroMorph,
Hero, Tattoos, grams, CreaturesKilledNames, items, Title -- not applied (local names do not change bytes; the coordinator may).
Tests: the f4006 tests with the helper removed (the embedded function text equals the row's); mutants without one sum term FAIL.
The tests keep two `const wchar_t*` overloads under `#ifndef _MSC_VER` (g++ run only; f4006 review item 6).
