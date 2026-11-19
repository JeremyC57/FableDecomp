# Landing hand3c2 b1 / b3 / b4 / b6 / b7 (Account B, 2026-11-19)

Five rows from Account B's hand3c chunk-2 author, all RELOCATION_MATCH with tests passing under strict vland `--land`, after an independent
audit (`audit_REPORT.md`, which also covers b2 and b5; those two are held, see below).

| batch | address | row | notes |
|---|---|---|---|
| b1 | 0x00AAD340 | CWaveFile::ConvertSamplingRate | retail_asm (floor / nearest helpers); Ego_d lines 55 / 69 `// Unreproduced:`; audit added test cases 12 and 14 |
| b3 | 0x00D6C830 | NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus | `/d2inlT62` (window 62..71, re-measured; inline-budget rule 2026-11-08); shared engine/fable_script_distance.h |
| b4 | 0x00AABF60 | CSoundEmitterLH::PlaySample | audit fix: shared CSoundManagerLH.h / CSoundSystem.h instead of local models; new nested opt-ins CSOUNDMANAGERLH_HEADPOSITION (descriptive member name: no PDB names CSoundManagerLH's members) and LIBPK_CSOUNDEVENTMANAGER_PLAYSLOTS; one `// Inferred:` condition polarity |
| b6 | 0x00A567A0 | CCardinalSpline::GetSplinePosition | same-TU strong copy of GFGetHermiteSplinePosition 0x00A56670, verbatim with its Fidelity line (audit fix: the line was missing); retail_asm |
| b7 | 0x00766660 | CTCObjectAugmentations::AttemptToStartParticle | audit fix: redundant local extern removed (C3DVector.h declares it) |

Mutants (audit plus author): b1 42/46 (four equivalent), b3 28/28, b4 55/55, b6 50/52 (two equivalent), b7 48/48.
Headers are additive and opt-in only (hdrcheck2 default=same; asserts pinned); hdr_diffs/. The four headers are unchanged since
2026-11-15, so the audit's includer regrade (all MATCH / RELOCATION_MATCH, both with and without the overlay) stands.

## Held
- **b2 0x0064DD9A UpdateRegionExitNames:** waits for the respell of landed 0x007791F0 to `?GetRegionName@CTCDRegionExit@@QBEABVCCharString@@XZ`.
  The audit used the TU order across Ego_r, FableWin and retail: the bodies are byte-identical, and 0x007792D0 keeps GetNameOfRegionConnectingTo.
- **b5 0x00A793D0 / 0x00A79680:** the new engine/CNavConnectorNode.h first needs landed 0x00A780B0 and 0x00A78A90 re-ported onto it.

## For Account A (name_fixes/, one file per row, as authored)
The audit's agreed names:
- PeekTCDriver<const CTCDRegionExit> 0x00617D30;
- the `_List_base<const CThing*>` ctor / dtor 0x00615520 / 0x00615540;
- the b4 descriptive names (0x00C0A860, 0x00C0A880, 0x00C08750, 0x00C08780);
- `CCountedPointer<CNavConnectorNode>::DeleteData` 0x00A82FA0;
- DELTA_NOTIONAL_ZERO 0x0129BA3C;
- landed 0x00A7A650's first two calls go to 0x00A793D0 ConnectHorizantalMapEdge (both edge functions are private).

## Review items (owner)
- verify_and_land's shared-header regex misses classes declared with a base list (`class X : public B {`). About 20 landed CSoundEmitterLH
  rows keep local models for that reason, and libpk/LHAudioEventPosition.h has a second, conflicting CSoundEventManager.
- The register_scripts.obj rows use per-row /d2inlT values (WatchForBooty 57, CheckBanditFollowStatus 62). The windows overlap at 62..63.
