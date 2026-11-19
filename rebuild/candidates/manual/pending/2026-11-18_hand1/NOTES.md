# hand1a notes (w23)

Tools: g.sh <addr> <abs src> [--flags ..] (preflight + q.py + masked diff), prio.sh (c2prio; PRIOARGS=--blocks),
inl.sh (inline trace), $S/w24/C/dag.sh <src> <func> '<pat json>' (scheduler regions), mkpayload.py <addr..> (payload.json +
oracle.tsv from src/ + test/). Absolute paths only.

## Matched
- 0x00B0D360 NParticleEngine::CPSCUpdateNormal::DatabaseSerialise: RELOCATION_MATCH + PASS (strict read-only vland,
  tmp/vland_00b0d360.log). Fable advisor c2_full_2arg: the CPSCLight 0x00B1A870 preamble makes CPersistContext::Transfer<T>'s
  body visible (retail still calls 0x004045C0 / 0x00410620), which lets VC7.1 fold the bool temp (xor cl,[esp+28h] at
  0x00B0DA91). 2-arg Transfer + GetDefaultVal bodies; Ego_d shape (no defaultScale local, float transfer outside the bool
  block). Test: test/00b0d360.cpp, standalone fixture over the same body (load / save / defaults), 11 mutants killed
  (member swaps, defaults 1000 / 1.0f, CRC, saving branch, dropped transfers, bitfield write-back).

## Escalated (tmp/ESCALATE_<addr>.md has the exact constraint each)
- 0x0066A9E0: Fable advisor; names 0x00415D70 / 0x00415DD0 likely swapped; differ/0066a9e0_fablewin_shape.cpp is the candidate.
- 0x00918A40, 0x008EEC70: Fable advisors assigned (coordinator).
- 0x006E89B0, 0x00B6DC40, 0x00AE3C30, 0x005AC10E, 0x00680050, 0x00F3ABF0: escalated with c2 constraints.

## Ego_d evidence applied
- 0x00F3ABF0: no `left` local -> differ/00f3abf0.cpp (probe f3_a) is the faithful spelling.
- 0x00AE3C30: no `data` local; 0x00B6DC40: no `empty_request` local; 0x005AC10E: all function-scope locals incl. pparent,
  pgold, block-scope pupper_menu_entry for the three PC_TEXT blocks.
