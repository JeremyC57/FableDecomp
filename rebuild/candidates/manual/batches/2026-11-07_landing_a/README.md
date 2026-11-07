# Landing 2026-11-07a: chunk 827 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows, sharing the lane with the chunk 826 agent. A container restart stopped the agent near the end; it
was resumed from its transcript and finished. All six rows were retries of game-logic parks with proven names. The landed row is
plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00684610 | CTCCreatureOpinionOfHero::OnSerialise | RELOCATION_MATCH PASS (122 lines at chunk 802, 4 at chunk 824) |

- **OnSerialise.**
  - Ego_r.pdb names a user copy ctor `??0COpinionTransientOffset@@QAE@ABV0@@Z`; the new opt-in
    `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY` declares it.
  - The row spells `GetDefaultVal<COpinionTransientOffset>() { COpinionTransientOffset value; return value; }`. The local is copied,
    not returned in place, so VC7.1 zeroes the hoisted default in memory order, as retail does.
  - Chunk 824's same-TU Transfer<long> / Transfer<COpinionTransientOffset> bodies (from the landed rows 0x00410BE0 and 0x00685F80)
    stay above the function under `#pragma auto_inline(off)`, the established same-TU callee pattern (GOTCHAS wave 25).
- **Headers:**
  - engine/CTCCreatureOpinionOfHero.h is copied in with two additive opt-ins: chunk 824's `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY`
    and this chunk's `_COPY`. No other landed file defines either.
  - The parked-only CDefPointer.h opt-in `FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD` stays in `pending/2026-11-07_linux_hand827/hdr/`.
- **vland and bindings:**
  - New bindings: the template Transfer<COpinionTransientOffset> (0x00685F80; the older non-template spelling was queued
    `land0611y`), Transfer<EOpinionAttitudeType> 0x00685CC0, and SerialiseFlagMask 0x00681BA0. No symbol is at a second address.
  - RELOC_NAME warnings come from catalog labels already queued for respelling: 0x00569FE0 TransferVector<float> (`cx12`, `w34`),
    0x00681BA0 SerialiseFlagMask (`land0611c`), and the two STL / CCharString labels.
- **Names:**
  - The three wrong caller spellings of 0x00410BE0 `??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z` are queued as `land0711a`
    (the right name was already queued under 574).
  - The agent's other fixes were already queued. Its two "catalog-only" stale names (0x00684610, 0x00833C00) are right in ghidra_out
    and the manifest.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`).
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-07_linux_hand827/`)
| Row | Grade | Residue |
|---|---|---|
| 0x007C4430 CTCHealLife::StartHealingEffect | DIFFER(1980v1952), 222 lines, 10 more attempts | the loop's float push_back is expanded where retail calls 0x00568850; no `/d2inl` setting (diagnostic) outlines just that site, so it is a source shape |
| 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering | DIFFER(2296v2300), 608 lines (was 684) | info.Engine cached in ebp where retail re-reads [ebp+34h]; frame slots one dword off |
| 0x0094AB00 CAIStateGroup_GoToPub::Update | DIFFER(2018v2067), 246 lines (was 743) | frame 0x188 v 0x190, one spill, edx v eax. **Review:** the gain uses row-local `__forceinline` list push_back and LowerBound helpers at the two sites retail expands (the 0x008DDAD0 per-site precedent). If those count as invented local copies, the fallback is chunk 800's 743-line source |
| 0x0066CF00 CRegionMinimap::PopulateRegionMinimap | DIFFER(3236v3248), 659 lines | x and the theme def pointer swap ebx / ebp; frame 8 bytes larger (new CDefPointer opt-in ASSIGN_REREAD) |
| 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams | DIFFER(2005v2073), 481 lines (was 643) | the sky block still calls LowerBound where retail expands it; frame packing (a do-while(0) with an Inline-split note and a visible ~CParticleUpdateParameters body) |
