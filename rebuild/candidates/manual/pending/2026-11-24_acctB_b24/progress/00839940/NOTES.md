# 0x00839940 CThingGameObject::OnHit -- B24B3 (triage only, not worked)
- Baseline regrade of the Codex pass21 draft (differ/00839940.cpp = base.cpp): DIFFER(5174v5195), 229 fg hunks, 547 raw lines (rawcmp on q_base.log).
- First divergence: `this` in ebp (retail `mov ebp, ecx` 0x0083994D) v ebx (ours); stack slots shifted by 4 (retail lea [esp+0x44] v ours 0x48,
  byte locals 0x58..0x5b v 0x5c..0x5f); frame 0xa0 equal. Register-role / slot-ranking residue over a 5 KB body (REGISTER_SWAP_SUMMARY class).
- Not worked: B24B3 spent its context on rows 1, 2 and 6; read pass21 PROBES.md (A01..A06) before any new idea.
