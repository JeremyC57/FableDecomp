# Chunk 3013 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B9A4A0) A 1-byte frame node's ALIGNMENT, not its slot, can be the whole residue: c2 node byte +0x2C = log2(align)+1, the slot's
  alignment (slot record +0x14, capped at 4) rounds its offset (0x1077479d). An inlined bool parameter home fed by a raw member load
  (`f(material->Transparent)`) is align 4 (symbol +0x39 bit 0x20); fed by an inline accessor (`material->IsTransparent()`) or a computed bool
  it is align 1. Retail [esp+1Bh] v ours [esp+18h] closed by the FableWin accessor. Read it with tools/al.py on a phook trace (free).
- (0x00B9A4A0) c2's final frame slots are laid out by weight/size descending (ties: list order); a named bool local has weight +1 over the
  forwarded parameter home (3 v 2), which re-sorts the byte slots -- prefer the accessor / temporary when only the alignment must change.
- (phook traces) the `packend` slot bitsets hold indices into the packend node-index array (event[5]), not node ids: remap before reading.
- (0x007C6E50) a split piece's spill test is not the lever: a forced spill 0 at 0x10782a5e is deferred (flag +5 bit 0x40, re-insert
  0x1078a35c), re-costed (0x10789715) and coloured on the next round. Which pieces exist (the split pass, block groups) decides it.
- (0x007C6E50) a GetTC HasTC test re-reads a CSE temp of the UsingComponents word when an earlier lookup on the same object tested the same
  word (hero-stats word 0 at line 143 -> the absorb lookup's `test dword [eax+20h]`): the parent piece there then holds only the map copy and
  the push and is skipped -- the codegen-temp `mov eax,[esp+14h] ... mov edi,eax` shape.

Review at landing:
- 0x00B9A4A0 needs hdr/ (chunk 2028 opt-ins + the new FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS). vland warns RELOC_NAME for
  DELTA_NOTIONAL_ZERO (catalog EPSILON) and BeginLoadingTextures (catalog Nop_ret4_00a23c50, NAME_FIXES.tsv): non-blocking.
- 0x00B9A4A0 global spelling 0x01436EA0 (g_pEngineCamera v NEngineInternal::GPCamera in c2018 / c2024).
- 0x007C6E50 stays parked after pass 1 of this chunk (2 grades; passes 2-3 unused for context); differ/ source = chunk 2027's best, comments
  extended with this chunk's measurements.
