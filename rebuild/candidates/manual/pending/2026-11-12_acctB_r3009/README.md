# acctB hand-back, near-miss retry of 0x007C6E50 (1 previously parked row: matched)

Chunk 2027 parked this row at 1804v1806; near-miss agents (Opus 5.5) continued on it in three passes and the third
closed it. Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007C6E50 `?FrameUpdate@CTCTurncoatSpell@@UAEXXZ` | RELOCATION_MATCH 1806v1806 (551/551 instructions), test PASS (g++ and VC7.1) | **supersedes `2026-11-11_acctB_c2027/rows/parked/007c6e50.cpp` and `2026-11-12_acctB_r3013/rows/parked/007c6e50.cpp`**. 30 full-row grades in the near-miss directory; no flags, no pragma |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one `WARN RELOC_NAME`, see below). Purity
and padded-struct greps are clean.

## What changed against the parked source

Only the XP block. The experience lookup moved behind a flag that lives in the not-turncoated arm:

```cpp
        if (!turncoat->IsTurncoated()) {
            bool give_xp = false;
            if (!IS_THING_ALIVE(PTarget) || ... ) Timer.SetCurrentButtonHeldState(...);
            else if (InRange) {
                ...
                if (--CurXPFrameCounter == 0) { CurXPFrameCounter = XPFrameCounter; give_xp = true; }
            }
            if (give_xp) {
                CTCHeroExperience* experience;
                if (parent->GetTC(&experience)) { ... AddTrainableStatExperience(...); }
            }
        }
        else { absorb ... }
```

VC7.1 threads the flag away (no flag bytes, same block layout), but the caster's live-range piece in the lookup block
now stays in memory, which is retail's `mov edx,[esp+14h]` hoisted above the counter store at 0x007C7420 and
`mov esi,edx` at 0x007C7432. An earlier what-if (only that piece kept in memory = 551/551) had already shown this was
the whole residue.

**FITTED, to be clear:** the flag is invisible in the bytes and `give_xp` is a descriptive name. It is the shape that
compiles to retail; it is not evidence of what the original source said. About 25 other spellings of the block are
logged in `NOTES.md` (none moved it); two control-flow probes (arms swapped; lookup after the whole if/else) each
reproduced the retail block prefix with the wrong layout, which pointed at this shape.

## Review items

- **0x007C76D0** is labelled `_unattributed::AssignTarget_007c76d0` in the repo; it is
  `?SetTurncoatOriginator@CTCTurncoat@@QAEXPAVCThing@@@Z` (Ego_r + FableWin; `LABELS.tsv`). This is the vland
  `RELOC_NAME` warning.
- **0x006D8300** is `?AddTrainableStatExperience@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z` (the only
  spelling in both PDBs). `reloc_bindings.tsv` has `...QAEXJJ@Z` from the landed 0x008F5C50 row (`NAME_FIXES.tsv`).
- `NAME_FIXES.tsv` / `LABELS.tsv` also carry the chunk 2027 entries (0x0072AC40 `FadeOrKillEffect`, 0x0072AB70
  `CreateParticleEffect`, 0x00669580 `GetSubDef`, 0x00669D30 `GetEffectFromString`, 0x00DCDA80
  `MakeTeamMemberComment`), unchanged.
- Headers: `hdr/engine/` is byte-identical to `2026-11-11_acctB_c2027/hdr/engine/` (no header changed in the retries),
  so no other row needs regrading. The opt-in blocks the row uses are listed at the top of `rows/007c6e50.cpp`.

## How to land

`rows/007c6e50.cpp` + `rows/007c6e50.test.cpp` with `hdr/engine/` (or the chunk 2027 headers if already landed), then
`payload.json` through your usual vland.
