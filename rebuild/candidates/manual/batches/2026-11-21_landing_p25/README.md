# 2026-11-21 landing p25: Codex pass 25 hand-back (2 of 5 verified rows landed, 3 held, 3 open)

Owner: "here is codex's work, land the matched ones, then try an opus 5.5 agent to try to finish matching the remaining 3."
Package: `../../pending/2026-11-21_codex_pass25/` (8 reserved rows). The returned zip's SHA256SUMS verify (123 files). Codex graded five
rows VERIFIED_MATCH and left three in progress.

## Landed
| Row | Function | Bytes | Flags | Notes |
|---|---|---|---|---|
| 0x00B0DC20 | NParticleEngine::CPSCUpdateNormal::Update | 5602 | /d2inlT200 | Ego_d lists `old_rhs` (bprel -416), so the dead `ToRightHandedSet(old_rhs)` chain is source text |
| 0x00C4F600 | CEAXManager::SetEAXProperty (descriptive: no PDB has a CEAXManager symbol; label DB agrees since chunk 874) | 9204 | none | |

- Header patches: only the two these rows use, from Codex's hdr/PATCHES.json. These are CQuaternion.h
  (`FABLE_RETAIL_CQUATERNION_RHS_EARLY_PASS25`) and EAXNativeTypes.h (`FABLE_RETAIL_EAX30_LISTENER_ORIGINAL_TAG`). Each old_text occurs once
  at HEAD.
- Gates:
  - Exact-mode whole-tree equivalence against HEAD's include tree (`logs/equivalence_nonfunclet.txt`): only the six known `__FILE__` rows
    differ, besides funclet rows that cannot resolve their relative include from a scratch tree.
  - `assert_names_check`: 0 mismatches.
  - Strict-header vland dry run (`logs/vland_dry_strict.log`), then the same with `--land` (`logs/vland_land.log`): RELOCATION_MATCH + PASS.
  - Warnings: one RELOC_ALIAS (import thunk) and one RELOC_NAME (0x0129BA3C DELTA_NOTIONAL_ZERO, as in p24). No RELOC_IDENTITY or CONFLICT.
- Pick-any, compared with Fable.exe (`logs/pickany_vs_retail.txt`; the new-row form of the rule, EXTRA_RULES 2026-11-21): 0x00B0DC20's
  seven copies that the table maps elsewhere equal retail. The eighth, the 5-byte `~CIntelligentPointer<CParticleEmitter>` jump, is not
  referenced by the leaf. HEAD binds that spelling to ~CBaseIntelligentPointer 0x00A01C10 (a folded alias), so the split is queued.
  0x00C4F600 exports no table-mapped COMDAT.
- Purity: no `__asm`, `naked`, `_emit` or `register`; no `volatile`.

## Held (leaf matches; pick-any copies differ from retail)
These leaves grade RELOCATION_MATCH + PASS (`logs/five_row_first_pass/`), but each object defines pick-any copies that differ from
Fable.exe at the owners' addresses: 0x0064FDB4 DrawAt, 0x0078FD60 CTCDivineWrath::FrameUpdate and 0x00795300
CTCUnholyPower::FrameUpdate. They are parked landing-ready, with the evidence and the route to landing, in
`../../pending/2026-11-21_codex_pass25_open/held/HELD.md`; the prerequisites are queued (land1121p25).

The advisor ruling and the new-row pick-any procedure are in EXTRA_RULES (2026-11-21).

## Open
0x00F1EED0, 0x00D01630 and 0x00E20600 are parked with Codex's sources and notes in `../../pending/2026-11-21_codex_pass25_open/<addr>/`,
plus `PATCHES_OPEN.json`. They are reserved for one Account A Opus agent (owner's request).

## Names
Codex's 12 NAME_FIXES plus the DrawAt manifest label are queued under `land1121p25` (CODEXP25), with no respells; naming is paused.
0x00797740 (14 bytes) and 0x00797750 (11 bytes) are uncatalogued code after 0x00795300's leaf (FINDINGS_LOG).
