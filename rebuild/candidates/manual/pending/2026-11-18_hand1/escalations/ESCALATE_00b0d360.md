# ESCALATE 0x00B0D360 NParticleEngine::CPSCUpdateNormal::DatabaseSerialise -- memory-operand fold v shared frame slot
Retail at 0x00B0DA8E..0x00B0DAB0 (RandomisePosDistVaryEnable bit 0 of byte +0x10, then RandomisePosDistVaryScale with default 0.0f):
`mov cl,[esi+10h]; xor cl,[esp+28h]; ...` (bool temp folded) AND the float default stored to the SAME word [esp+28h] (context home).
- Ours, float transfer outside the bool block (natural, Ego_d has no `defaultScale` local): slot right, but `mov dl,[esp+28h]; ...; xor cl,dl`
  (differ/00b0d360_outside.cpp, 2242v2240). Probe: removing the following float transfer restores the fold, so the following store into the
  shared slot blocks the fold in our build.
- Float transfer inside the bool block (codex draft / 2-arg / literal): fold right, default moves to [esp+14h] (2240v2240, 2 hunks).
- Tried: 2-arg Transfer with GetDefaultVal<float>/<bool>/<CCharString> specialisations (landed sibling 0x00B13530 idiom; no change), macro forms.
Guess: in retail the bool temp and the float default are separate symbols until the late frame packer; something in the original makes c2 keep them
apart at codegen (a different temp kind for the default, e.g. a by-value GetDefaultVal temporary created inside an inlined helper?).
