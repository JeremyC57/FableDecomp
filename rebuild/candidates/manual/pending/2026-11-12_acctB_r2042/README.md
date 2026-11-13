# acctB hand-back, near-miss retry of chunk 2042 (1 previously parked row: matched)

The authoring agent parked this row at 1929v1942; a near-miss agent (Opus 5.5) continued in the same directory and
closed it. Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007F2DC0 `?FrameUpdate@CTCLightningNode@@UAEXXZ` | RELOCATION_MATCH 1942v1942, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2042/rows/parked/007f2dc0.cpp`**. 10 grades in all (7 authoring incl. one diagnostic, 3 near-miss). No header changed in the retry |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; nine `WARN RELOC_NAME`, all stand-in labels
listed in `NAME_FIXES.tsv`). Purity and padded-struct greps are clean; no flags, no pragma.

## What changed against the parked source (I diffed them: three lines)

```cpp
    long timer = Timer;                                   // now after the PDef null test
    const bool has_life_time = PDef->LifeTime > 0.0f;     // new
    if ((has_life_time && timer > PDef->LifeTime * CMainGameComponent::GetConstantFPS()) || ...
    ...
            float range = PDef->Range;                    // new
            C2DBoxI area(C2DCoordI((long)pos.X, (long)pos.Y), (long)range);
```

1. **The Timer copy.** VC7.1 folds a single-use copy of a member into its use unless a control-flow join lies between
   the copy and the use (agent's fragments; the same effect is in the landed CTCHeroExperience::IsToAutoBlock
   0x006D6CB0, where a clamp's join keeps `level` in a frame word). The materialised bool supplies the join and gives
   retail's prologue byte for byte (`fld [ecx+40h]` / `mov eax,[ebx+28h]` / `fcomp` / `mov [esp+18h],eax` / ... /
   `fild [esp+18h]`) and with it retail's whole frame. With a plain `bool` an x87 squaring order elsewhere flips;
   `const bool` gives retail's.
2. **The store order at 0x007F2E9A.** Reading `PDef->Range` as an expression after the C2DCoordI temporary's stores
   leaves a may-alias edge (the temporary's address goes to the out-of-line C2DBoxI constructor); reading it into a
   named value first removes the edge and the `fld [ecx+3Ch]` issues first, as in retail (agent's scheduler-hook trace).

**FITTED, to be clear:** both mechanisms are measured, but the spellings are chosen because they compile to the
measured mechanism. `has_life_time` and `range` are descriptive local names; no PDB helper exists for either, and they
are not evidence of what the original source said. (The coordinator's guess in `COORD_IDEAS.md`, an inline helper with a
by-value parameter, was not what closed it.)

## Review items (carried over from `2026-11-12_acctB_c2042/README.md`, still open)

- `CVectorMap::Insert` forced inline through `FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE`, with an `// Inline-budget:`
  line (plain inline scores 191 and is called; retail expands both inserts).
- Headers (`hdr/README.md`): `CTCLightningNode.h` (nested `CLightningBranch` with DESCRIPTIVE member names, no PDB
  layout record), `CTCDParticleEmitter.h`, `CHitParameters.h`, `CThingFilter_EnemyCreature.h`, `C2DBoxI.h`,
  `CVectorMap.h`; new `CLightningDef.h`, `CThingFilter_In3DRange.h`, `CTCElectrocutedEffect.h`,
  `NParticleEngine_CParticleUpdateParameters.h`.
- `NAME_FIXES.tsv`: the entries of the first hand-back plus 0x00686D90
  `?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ` (a landed binding spells it
  `?GetThingSearchTools@CTCShopKeeper`). 0x0129BA3C is labelled `EPSILON` while `C2DVector.h` spells it
  `DELTA_NOTIONAL_ZERO`; neither is confirmed in the PDB name lists.
- The `CCountedPointer<CLightningBranch>` destructor is called through 0x004DA6F7 (a `jmp` to 0x004D799A, landed as a
  free `_Forward` stand-in): the symbol for each address still needs a decision.

## How to land

`rows/007f2dc0.cpp` + `rows/007f2dc0.test.cpp` with `hdr/engine/`, then `payload.json` through your usual vland.
