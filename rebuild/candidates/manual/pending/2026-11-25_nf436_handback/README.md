# NF436 hand-back (2026-11-25): script-class debt rows, round 2

NF436's own summary is `AGENT_README.md`; `NOTES.md` has every gate.

## Landed: 39 rows (`batches/2026-11-24_landing_nf436_{a48,a1,a1t,n1}`)
| Batch | Rows | What |
|---|---:|---|
| a48 | 5 | 0x48 Alloc rows whose class already has a header |
| a1 | 25 | Alloc rows of sizes 0x4C to 0x74 whose class already has a header |
| a1t | 1 | 0x00E28CA0 (R-NF436-2) |
| n1 | 8 | Alloc rows on 8 new class headers |

- **Headers.** Each `FABLE_RETAIL_<C>_ALLOC` block adds only what the class lacked: the user ctor, the static Alloc, and FableWin's public overrides. Size asserts are pinned.
- **Vftables.** Checked against today's table: 0 wrong slots. Every slot matches FableWin's `??_7` (78 / 78), and every `??_G` equals retail at its slot 0.
- **Coordinator equivalence check** with all 34 headers: 0 of 57,160 rows differ. No row NF437 landed in between includes any of these headers.
- **Effect on the tracker:** none. These rows already had the right names; the port clears shared-header debt.

## Rulings
- **R-NF436-2 (coordinator, 2026-11-25): granted.** a1t's opt-in wraps the header's `virtual ~CQS_GuardianTrophyDealerInfoScript();` in `#ifndef`, so the opt-in removes a declaration. That is allowed here because:
  - the default path stays byte-identical (hdrcheck, equiv 0);
  - FableWin.pdb marks the destructor compiler-generated;
  - no landed row defines it.
- **R-NF436-1: open.** 12 ctor rows. FableWin's base is `CLevelScriptNoMasterBase`, and modelling it emits a pick-any ctor copy that clashes with the strong 0x00CDD2F0. NF436 prefers making 0x00CDD2F0 inline first.

## Left
34 rows (nf436 round-3 line) and 8 name fixes (queued).
