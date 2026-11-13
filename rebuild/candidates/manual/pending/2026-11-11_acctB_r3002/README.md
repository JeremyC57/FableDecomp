# 2026-11-11_acctB_r3002 -- acctB hand-back, retry chunk 3002 (2 previously parked rows: 1 matched, 1 still parked)

Authored by an Opus 5.5 agent starting from the parked sources of c2022 and c2006, re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007F26B0 CTCLightningOrb::Initialise | RELOCATION_MATCH 1802v1802, test PASS (g++ and VC7.1) | **supersedes `2026-11-11_acctB_c2022/rows/parked/007f26b0.cpp`** (was DIFFER 1793v1802, frame size 0x78 vs 0x8C). Plain C++, no device. Three source facts give retail's frame: GetEffectFromString's body visible (existing repo opt-in `FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY`), the two Fireheart control points as named block locals, and `difference /= distance; C3DVector direction = difference;`. 4 grades |
| 0x00550DC0 NUISystem::CTable::Draw | still parked, DIFFER 1399v1399 | 20 masked lines, frame slots only: retail 0x00550EFE `fstp [esp+74h]` (parent R) / 0x00550F53 `fstp [esp+3Ch]` (table R), ours 3Ch / 74h; retail 0x00550F59 `[esp+38h]` / 0x00550F63 `[esp+34h]`, ours 34h / 38h. Measured cause: an equal-weight tie in c2's frame packer (both colour temporaries weight 2; the table one is seen first in scheduled order). ~35 probe spellings, 1 grade; **passes 2 and 3 not used** (the agent found no code-identical spelling that moves the tie). Source + test in `rows/parked/` (same residue as c2006's copy; line 2 updated) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean. I also regraded the
parked row: DIFFER(1399v1399), as recorded.

Review items for landing:

- The 0x007F26B0 object also carries the noinline COMDAT `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z`
  (retail 0x00669D30, landed elsewhere): grade only the Initialise leaf.
- The locals `fireheart_approach` / `fireheart_rise` are descriptive names (no PDB local names available).
- vland RELOC_NAME warnings are the labels already filed in c2022's NAME_FIXES.tsv (three CTCLightningNode setters,
  ProcessPoints) and DELTA_NOTIONAL_ZERO (catalog label `EPSILON`).

To land: `hdr/` is the union of the c2022 and c2006 overlays, files unchanged (no file names overlap); 0x007F26B0 needs only
the c2022 part, which builds on c2007 / c2002. Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
`LESSONS.md` documents c2's frame packer (offline replay, the 0x70 dead-parameter-home rule, tie ordering) for GOTCHAS.
