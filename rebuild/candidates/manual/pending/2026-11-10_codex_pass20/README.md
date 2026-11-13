# Codex pass 20 (2026-11-10): reservation

Handed to an outside ChatGPT Codex session as `fabledecomp_codex_pass20_2026-11-10.zip` (built by the coordinator from HEAD; no retail
bytes). 40 rows, listed in RESERVED.tsv: keep in-house agents off them until the return lands.
- 5 large unworked rows (1,312-1,888 bytes): 0x006D18A0 CTCWife::Marry, 0x00785620 CTCVolumeContainmentTracker::Update,
  0x007677E0 CTCObjectAugmentations::AttachAugmentationParticles, 0x007F6200 CTCTargetingAI::UpdatePBestEnemyCreatureForAttack,
  0x0076EFA0 CTCWeaponTrail::UpdateWeaponTrails.
- 35 parked near misses, all same length as retail, picked by re-grading 70 parked drafts against HEAD headers (2-52 differing
  instruction lines after masking; the residue per row is in RESERVED.tsv).
The package carries include/ at HEAD (Codex pass 19 merged), the naming data (reloc_bindings, NAME_FIXES_QUEUE through land1011bb,
PDB name sets, labels), docs (HANDOFF, GOTCHAS, playbook, EXTRA_RULES, ASM_RULES ...), the newest hand-back lessons (chunks 1013-1020,
naming NF3-NF5, Codex passes 15-19) and README_CODEX.md / PROMPT_CODEX.md (copies here).

**Released 2026-11-13 (owner):** the Codex work for this pass has been handed back. `RESERVED.tsv` is renamed `RELEASED.tsv`, so the
reservation glob (`pending/*/RESERVED.tsv`) no longer holds these rows back. The rows still unlanded go to the normal parked pool with
their closest version on file: `../2026-11-13_codex_released/` (STATUS.tsv, differ/).
