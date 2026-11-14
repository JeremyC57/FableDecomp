# acctC hand-back c42a (agentC42, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00A5CB90 | CMatrix4x4::AlignedTransformTransposedAssumeW | 847 | RELOCATION_MATCH (`/Zi`), grade `retail_asm` | PASS |
| 0x00C6F3E0 | libpk multi-tap delay effect, MMX slot 2 `Process` (descriptive; catalog: CAudioMixer_ProcessChannels_64bit) | 834 | RELOCATION_MATCH (`/GS`) | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`
(0x00A5CB90 prints WARN RELOC_UNCHECKED, multi-block TU: its call targets were checked by hand by the author).

OWNER SIGN-OFF NEEDED (ASM_RULES):
- Two new retail-asm kernels as shared-header macros with FABLE_PORTABLE twins and dual-path tests (`test_portable/`, to move
  to `rebuild/tests/portable/` at landing): `GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W` in the new
  `engine/CMatrix4x4TransformSSE.h`, and `LIBPK_MMX_COPY64` in `libpk/MixKernelsSIMD.h`. No asm in either row source.
- `// Flags: /Zi` on 0x00A5CB90 rests on byte evidence only: retail's realigned ebx frame copies the return address
  (`sub esp,8 ... mov ebp,[ebx+4]; mov [esp+4],ebp`), which VC7.1 emits only with debug info (832v847 without; `/Oy-` and
  `optimize("y",off)` do not produce it). Three landed rows carry `/Zi`; the landed same-TU sibling 0x00A5CEE0 has no flag
  (its frame cannot show the difference).
- 0x00C6F3E0: local class models and descriptive names, as every landed row of that libpk module; the test is a
  self-contained portable model (sibling convention), it does not link the row object.
- Labels: 0x00987700 is `??0CMatrix4x4@@QAE@XZ` (folded empty ctor); 0x00987670 needs an alias (also the empty C4DVector ctor).

`open/` (successor agentC43 unless noted):
- 00772bb0.cpp CTCShop::AddSaleItem DIFFER(2585v2585) and 007de710.cpp CTCWallMount::AttachObject DIFFER(2578v2578): one
  shared x87 term-order residue in the inlined inverse text (ours 120i/341b v FableWin 123i/345b).
- 00875690.cpp CGeneralCircle ctor DIFFER(726v762); 006d0830.cpp CTCWife::GlobalUpdate (port unfinished).
- 007db6a0.cpp CTCTavernGameCardBlackjack::CardsMainGame DIFFER(2556v2556), two register picks (parked).
