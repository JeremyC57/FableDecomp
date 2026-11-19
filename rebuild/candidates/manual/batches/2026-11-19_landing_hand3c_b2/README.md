# Landing hand3c b2 (2026-11-19): 2 not-plain-C++ rows (281 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (`logs/vland_land.log`); tests also PASS on g++ with FABLE_PORTABLE. No header changes.

| row | notes |
|---|---|
| 0x00C6AC90 CSoundMixer::MixBlockRamped (38, /GS) | retail_asm through the same-TU copy of the libpk MMX ramp kernel (0x00C6AAE0, shared LIBPK_MMX_SCALE_ACCUMULATE16_RAMP, `// Retail-asm:` 0x00C6AB43 `pmulhw mm0,mm7`); the row's own bytes are compiled C++. Member names m_bMixed (+8) / m_pAccumulator (+0x21C) are descriptive |
| 0x00A9D480 CMatrix4x4::AlignedPreScale (243) | retail_asm through the existing GF_SSE_MATRIX_PRESCALE helper; x87 fallback written through `CMatrix4x4& m = *this; m(r,c) *= scale.X` (FableWin shape; the accessor fixes the x87 operand order). Existing opt-ins only |

Naming debt: the kernel's landed name CUnprovenMixKernel_00c6aae0 carries the banned "Unproven" placeholder (`NAME_FIXES.tsv`), queued for the naming agent.
