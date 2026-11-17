# NF49 header overlay (round 7)

Both files are copies of the repo headers at the time of the round with ONE `#ifdef` opt-in block added each. Without the opt-in macro
each file is token-identical to the repo header (only the added `#ifdef ... #endif` lines differ; `diff` shows the insertion only).

| overlay file | copied from | opt-in added | used by |
|---|---|---|---|
| `engine/CAIStateGroupBase.h` | `rebuild/include/engine/CAIStateGroupBase.h` | `FABLE_RETAIL_CAISTATEGROUPBASE_CAIFILTERBASE_RESTORE_PROTECTED`: a `protected:` access label right after the nested `CAIFilterBase` block (`#if FILTER \|\| FINDAGUARD`), whose `public:` otherwise leaves `AIAction_PerformActionAndWait` / `_LookAtThing` / `_MoveToThingProximity` public (QAE). Ego_r.pdb + FableWin.pdb spell all three protected (IAE). Access label only, no layout change. | row 0x009378F0 (also fixes landed 0x009810B0 once it opts in; NAME_FIXES) |
| `engine/CMaterialDef.h` | `rebuild/include/engine/CMaterialDef.h` | `FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT`: declaration `long GetRandomSlideFootParticleEffect() const;` = Ego_r.pdb `?GetRandomSlideFootParticleEffect@CMaterialDef@@QBEJXZ` (retail 0x008EE5A0, landed row; labels_familypass6 L093). Declaration only, no layout change. | row 0x008F7950 |

Landing: diff each against the repo header (merge if NF48/NF50 moved it), copy into rebuild/include, recheck the includers. Both opt-ins are
off by default, so no existing includer changes.
