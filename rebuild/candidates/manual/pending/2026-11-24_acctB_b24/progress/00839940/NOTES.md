# 0x00839940 CThingGameObject::OnHit -- B24B3 (triage only, not worked)
- Baseline regrade of the Codex pass21 draft (differ/00839940.cpp = base.cpp): DIFFER(5174v5195), 229 fg hunks, 547 raw lines (rawcmp on q_base.log).
- First divergence: `this` in ebp (retail `mov ebp, ecx` 0x0083994D) v ebx (ours); stack slots shifted by 4 (retail lea [esp+0x44] v ours 0x48,
  byte locals 0x58..0x5b v 0x5c..0x5f); frame 0xa0 equal. Register-role / slot-ranking residue over a 5 KB body (REGISTER_SWAP_SUMMARY class).
- Not worked: B24B3 spent its context on rows 1, 2 and 6; read pass21 PROBES.md (A01..A06) before any new idea.
## B24B4 (2026-11-24): regrade only -- DIFFER(5174v5195) 229 hunks (r0.cpp = differ/ unchanged). First hunks: retail `mov ebp,ecx` + `this`
spilled to [esp+0x24] (ours ebx, [esp+0x1c]); ebx/ebp role swap for the CGameScriptInterface pointer ([esi] load at 0x00839991); CRGBColour
byte locals 0x58..0x5b v ours 0x5c..0x5f, a float at 0x44 v 0x48. Not worked further: the coordinator raised item 5 (one-TU test) above it.
- First structural hunks (r0full.log): ours loads `this` back from its slot and keeps the constant 0x2000 in ebp (`mov ebp,0x2000` at
  0x00839BC3, no source literal 0x2000: a c2-materialised constant / flag mask) while retail keeps `this` in ebp throughout; at the
  MakeNoise(CCharString("SIMPLE_NOISE")) site retail builds the temp at [esp+0x14] and passes `lea eax,[esp+0x14]` (ours one slot lower and
  no separate lea). Next: find which source expression yields 0x2000 (a flag test such as `& 0x2000` inside an inlined accessor of
  CThing / CTCBase, or a bitfield) -- removing that repeated constant frees ebp for `this`; then recheck the 21-byte gap.
- Correction/lead (B24B4): the ebp=0x2000 constant comes from the TC-presence bitset tests (HasTC(TCI_DOOR) at [this+0x20] bit 13, and the
  [this+0x2c] bit 13 test at 0x00839DF2): ours `test dword ptr [reg+0x20], ebp` (constant hoisted into ebp, two uses), retail
  `mov eax,[ebp+0x20]; test ah,0x20` at both sites (word loaded to a register, byte test). So retail's HasTC / bitset::test body at those
  two sites yields a LOADED word (e.g. a by-value/`const bool&` result or `_M_w[n] & mask` read through a local), not a memory-operand test.
  Fixing that spelling should free ebp for `this` (the root of the ebx/ebp swap). CThing.h has three HasTC spellings (lines 366 / 378 / 380,
  one with `const bool& found = UsingComponents.test(type)`): try each per site class first.
- m1.cpp (overlay hdrm: FABLE_RETAIL_CTHING_HASTC_MATERIALISED + the ONHIT template copied into that branch; diagnostic): `this` moves to
  EBP as in retail, but every HasTC site materialises -> frame 0xa4, 5409v5195, 287 hunks. So the ebx/ebp swap IS the HasTC spelling;
  retail mixes forms (in-place `test byte ptr [ebp+0x24],0x40` for the template's inner test, `mov eax,[ebp+0x20]; test ah,0x20` for
  HasTC(TCI_DOOR) and the [ebp+0x2c] site, out-of-line bitset::test 0x00410DE0 late). Next: plain-inline (unforced) HasTC(type) with the
  forced template, and the STLport bitset::test body visible but noinline (Q32 lesson), so c2 decides per site as retail did.
- p1.cpp (overlay hdrp, plain-inline HasTC(type), template forced): 5170v5195, 230 hunks, this still ebx -- no.
