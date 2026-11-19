# fixed_b11 (verifier, 2026-11-19): 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad, library-grade route with no asm anywhere
Replaces b11/alt_library + b11's asm helper. No file here holds an inline-asm keyword, an alignment declspec or the asm-helper tag (grep in NOTES_audit).
- src/00c88e60.cpp: body-less stub, `// Toolchain-library: CODAApi.lib(.\Release\ReverbFns_SSE.obj) ?ApplyFilterOrder2_quad@... -- 681 bytes
  verified against Fable.exe 0x00C88E60`; wording "byte-identical to GameCODA 2.10 CODAApi.lib member ReverbFns_SSE.obj <symbol>";
  under `#ifdef FABLE_PORTABLE` only: the two opt-in defines + includes (no braces in the file). purity.classify = toolchain_library;
  lib_verify --source MATCH, 16 masked bytes, 4/4 relocations [ok:lib]; @comp.id 0x0060/3077 = Rich entry (1369 objects).
  Landing path suggestion: rebuild/src/compiled/00/c8/CReverbFnsSSE_ApplyFilterOrder2quad_00c88e60.cpp (the rows driver includes that path).
- hdr/engine/ReverbFnsSSEFilterOrder2Quad.h: plain-C++ twin only (fable_portable::ApplyFilterOrder2QuadKernel under FABLE_PORTABLE or
  FABLE_DUAL_PATH_TEST; the member's portable definition under FABLE_PORTABLE, included only by the stub). Fixes against b11's twin:
  reads all four lanes before writing any (b11's twin stored lane l before loading lane l+1: wrong for pfDest = pfSrc + 128, which the
  SDK member handles; measured), and keeps VC7.1's per-lane association of c6 (retail +0x059/+0x0B5 (b0+b1)+b2, +0x10F (b1+b2)+b0,
  +0x169 (b2+b0)+b1). Portability statement in the header.
- hdr/engine/CFloatFilterParams.h: = b11's copy (b10 block + `friend class CReverbFnsSSE;` opt-in). The friend is still needed: the
  portable member reads m_fA[1..2] / m_fB[0..2] / m_fGain (no public getter for a1, a2, b1, b2; removing it is a g++ access error).
- hdr/engine/CReverbFnsSSE.h: = b11's copy (NEW).
- test/: empty. A library row has no behaviour test; the dual-path tests carry the retail side.
- tests_portable/test_ReverbFnsSSEFilterOrder2Quad.cpp (for rebuild/tests/portable/): retail side = the CODAApi.lib member, which the test
  links via `#pragma comment(lib, "CODAApi.lib")` (LIB must hold CODA/lib/Win32: FABLE_VC71_LIB). vc71-asm PASS, vc71-dual PASS (member
  and twin kernel on the same blocks), vc71-portable PASS, gxx-portable PASS, gxx-m32-compile SKIPPED (no 32-bit glibc).
- tests_portable/rows/00c88e60_ApplyFilterOrder2_quad_driver.cpp (for rebuild/tests/portable/rows/): as-is = SDK member, /DFABLE_PORTABLE =
  twin member; vc71-row-dual PASS (1002 lines identical), vc71-row-portable OK. Exact data only (VC7.1's portable build is x87).
- Twin mutants (g++ portable): 13/13 non-equivalent killed (incl. b11's per-lane kernel, lane-2 / lane-3 c6 association, c2 / w / y
  re-association); 2 single-sided overlap mutants are equivalent alone; the fb commutation control survives.
- Host build gap (REVIEW): the stub's FABLE_PORTABLE build compiles under VC7.1 (vc71-row-portable OK); under g++ x86-64 it needs
  -DFABLE_RETAIL_PASS23_HOST_TEST_ABI (rebuild_abi.h fable_require_ptr32 and CReverbFns.h `operator new(unsigned int)` otherwise fail;
  stub_gxx_hostgap.log). That gap is in the landed shared headers, not in the twin.
- Gather vs x87 precision control (REVIEW): bit-identity of the twin's c2 / c6 holds for retail under PC_24 only; which PC retail's
  audio code runs under is not established. Alternative if PC_53 is shown: compute the gather sums in double and store to float.
