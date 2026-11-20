# NF125 overlay (round 10)

## engine/CTCHeroExperience.h
Copy of the repo working-tree header (base copy in ../orig/engine/CTCHeroExperience.h) with three `#ifdef` opt-ins; nothing else changed.
Without the new macros the preprocessed text is token-identical to the repo header (the additions are three new `#ifdef` blocks and
one new `#elif defined(...)` branch placed before the existing STATEXPERIENCE branch, taken only when its macro is defined).

| macro | what it adds | users |
|---|---|---|
| `FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS` | a layout branch in the `#elif` chain: BLOCK_THRESHOLDS' `std::vector<long>` StatExperiencePoints / TrainableStatLevels, plus Ego_r.pdb ExperienceSpentOnStats as `std::vector<long>` (+0x44), WITHOUT BLOCK_THRESHOLDS' inline GetTrainableStatLevel / GetTrainableStatMaxLevel bodies (so the out-of-line definitions can be compiled); includes stlport/fable_containers.h | self rows 0x006D64A0, 0x006D6510, 0x006D6520 |
| `FABLE_RETAIL_CTCHEROEXPERIENCE_SPENTONSTAT` | declaration `long GetExperienceSpentOnStat(EHeroTrainableStatType) const;` (Ego_r 0x00587C10 / FableWin 0x01EEFBE0; retail 0x006D64A0) | 0x006D64A0, 0x005724CD |
| `FABLE_RETAIL_CTCHEROEXPERIENCE_FORCEEXPERIENCE` | declarations `void ForceSpendableExperienceTo(long);` (0x006D6470) and `void ForceTrainableStatExperienceTo(EHeroExperienceStatCategory,long);` (0x006D6830), Ego_r + FableWin QAE | 0x0056F4BA |

No member is declared twice: the repo header has no GetExperienceSpentOnStat / Force* declaration, and the NF123 / NF124 overlays
(read 2026-11-20) do not touch this header.

pin_asserts.py (`pin_asserts.py rebuild/include hdr engine/CTCHeroExperience.h`): pinned 0 (every assert in this header was already
`FABLE_STATIC_ASSERT_AT`).

Header users: 84 landed rows include engine/CTCHeroExperience.h (`grep -rlE 'engine/CTCHeroExperience\.h' rebuild/src/compiled`);
none defines any of the three new macros, so these opt-ins change none of them.
