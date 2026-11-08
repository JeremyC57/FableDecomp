# Chunk 871 header overlay

Every change is additive. The default path (no opt-in macro defined) is byte-for-byte the current `rebuild/include`
header. Each overlay file is the current repo header plus the blocks below; `diff rebuild/include/<f> hdr/<f>` shows only them.

## engine/CThingTrackNode.h: `FABLE_RETAIL_CTHINGTRACKNODE_CTOR` (row 0x008383D0)
- Declares `CThingTrackNode();` (Ego_r.pdb `??0CThingTrackNode@@QAE@XZ`). It is out of line in retail at 0x00837C70, which is landed and stores vtable 0x01272CE4.
- `CThingTrackNode::Create` calls it through `new (false) CThingTrackNode`.
- It adds a declaration only, with no layout change.

## engine/CString.h: `FABLE_RETAIL_CSTRING_DEFAULT_CTOR` (row 0x00C4B640)
- Adds the inline default constructor from MFC afx.inl: `CString() { Init(); }`.
- Retail emits `mov eax,[0x0139C900] ; mov [local],eax` for it in every libpk trace message.
- The block is guarded with `!defined(FABLE_RETAIL_CSTRING_LEFT)`, because chunk 870's LEFT block declares the same constructor. Both opt-ins can therefore be defined in one TU.
- It needs `FABLE_RETAIL_CSTRING_ALLOCCOPY`, which supplies the inline `Init()`.
- Rebased on the repo header as of 2026-11-08, which already carries chunk 870's `FABLE_RETAIL_CSTRING_LEFT`.

## libpk/CpuIdentify.h: NEW shared retail-asm helper (row 0x00C52E40, DetectCpu)
- Contains eight macros, one per retail CPUID block of DetectCpu:
  - `LIBPK_CPUID_VENDOR`, `_SIGNATURE`, `_CACHE_DESCRIPTORS` and `_SERIAL`;
  - `LIBPK_CPUID_EXTENDED_MAX`, `_EXTENDED_FEATURES`, `_BRAND` and `_AMD_L2`.
- The retail blocks are at 0x00C52ED4, 0x00C52F5F, 0x00C52FB3, 0x00C53001, 0x00C5301E, 0x00C53039, 0x00C53055 and 0x00C530EB. The header comment cites each one instruction by instruction.
- Why asm is needed: VC7.1 has no `__cpuid` intrinsic and no C++ form that emits `cpuid`. This is family CPUID C1, as for the landed `engine/GFInitVectorMathCPUID.h`.
- The retail path is macro-wrapped `__asm` statements, so the row source contains no asm.
- The `FABLE_PORTABLE` twin is `fable_portable::LibpkCpuid`:
  - `__get_cpuid_count` on GCC/Clang, `__cpuidex` on MSVC 8 and later;
  - VC7.1's own `__asm` CPUID inside the twin, so the VC7.1 dual build compares two routes;
  - zeros on hosts that are not x86.
- Dual-path test: `hdr/tests/portable/test_CpuIdentify.cpp`, to land as `rebuild/tests/portable/test_CpuIdentify.cpp`. It passed in all three VC7.1 modes (asm, asm+portable with `/DFABLE_DUAL_PATH_TEST`, portable with `/DFABLE_PORTABLE`), and under g++ with `-DFABLE_PORTABLE`.
- Usage notes:
  - Write each macro call on its own line.
  - Operands must be plain locals or globals, never reference parameters, and must not be named `type`, which is a MASM operator.
