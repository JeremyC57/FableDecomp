# acctC lane, hand-back c1a (2026-11-13): 2 rows matched, 33,783 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x005D1F0C | CTCInventoryStats::ConstructPersonalityListPC | 17,873 | RELOCATION_MATCH | PASS |
| 0x005CE0E6 | CTCInventoryStats::ConstructPersonalityList | 15,910 | RELOCATION_MATCH | PASS |

Both are the acctB lane's f4006 rows, ported onto the headers of `FullDecomp` at 55262db83 and re-verified by the lane coordinator
(read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, overlay = the eight headers below only; `coordverify.log`). Lengths are real.
No flags. No oracle file here: cut it from retail on the landing machine.

## What changed against f4006
- **The made-up helper is gone.** `CTCHeroStats::GetNumberOfBuildingsOwned()` (the reason landing fy held the rows) did not exist:
  retail's own unreferenced getter COMDAT run of this TU (0x005CC58E..0x005CC685) has the houses and shops getters and no sum body,
  FableWin's COMDAT run after the function lists the same getters, and the Anniversary Test PDB's CTCHeroStats method list has no
  such member. The rows write `GetNumberOfHousesOwned() + GetNumberOfShopsOwned()`. Full evidence and every spelling tried: NOTES.md.
- **GFRoundFloatNear:** f4006 had changed the shared default body; here it is a new opt-in
  `FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_QUOTIENT` (default and GROUP_PRODUCT bodies untouched).

## OPEN, owner decision needed before landing
1. **Fitted getter body.** `long GetNumberOfHousesOwned() const { long houses = NumberOfHousesOwned; return houses; }` (inside the new
   CTCHEROSTATS_PERSONALITY opt-in) is what gives retail's `lea edx,[eax+ecx]` at 0x005D4698 / 0x005D00AB; the plain one-line getter
   gives `lea edx,[ecx+eax]` (one instruction, same length). Only PDB names are used, and the getter's out-of-line bytes equal
   retail's COMDAT 0x005CC5FD either way, but FableWin's debug copy of the getter is the size of the plain one-liner. FITTED.
2. **GROUP_QUOTIENT** is a no-code parenthesis fit of a shared inline (same class of item as the existing GROUP_PRODUCT opt-in).
3. f4006 review item carried over: the same-TU `ConvertMap` body (pair-landing with the landed 0x005CD6B7).

## Headers (`hdrdiff/`, unified diffs against rebuild/include at 55262db83; `hdrdiff/README.md` lists every opt-in)
Additions under NEW opt-ins only; no default path changed; moved `FABLE_STATIC_ASSERT`s are pinned with `FABLE_STATIC_ASSERT_AT`
(`lane_merge/pin_asserts.py`). The agent's `equiv_check.py` run over all landed rows reported 0 rows with changed preprocessed text
(the overlay then also held the headers of a row still in progress; rerun on the landing machine after applying the diffs).
