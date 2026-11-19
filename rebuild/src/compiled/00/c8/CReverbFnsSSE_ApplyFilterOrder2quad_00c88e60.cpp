// CReverbFnsSSE::ApplyFilterOrder2_quad: GameCODA 2.10 library code, not Fable source (grade toolchain_library; owner rule
// 2026-11-19, EXTRA_RULES "rows byte-identical to the GameCODA SDK library").
// Toolchain-library: CODAApi.lib(.\Release\ReverbFns_SSE.obj) ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z -- 681 bytes verified against Fable.exe 0x00C88E60
//
// Retail 0x00C88E60..0x00C89108 (681 bytes, thiscall, `ret 14h`) is byte-identical to GameCODA 2.10 CODAApi.lib member
// ReverbFns_SSE.obj ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z with its 4 relocations
// masked; all 4 resolve to the same target (+0x3D, +0x99, +0xF3, +0x14D DIR32 __real@3f800000 = 0x0122DED8, retail's 1.0f).
// The member stamps @comp.id 0x0060 build 3077 (Utc1310_CPP), a Rich-header entry of retail (1369 objects). Retail linked a
// different SDK build than the one on disk (member offsets and channel counts differ elsewhere), so this records byte identity,
// not the library retail linked. The function is C++ around the SDK's hand-written SSE block (the CODAApiDbg.lib /Od body
// carries the same block). One intrinsic spelling was graded (an <xmmintrin.h> transcription, DIFFER 893v681); no further spellings
// were tried, the library route being the coordinator's decision (2026-11-19).
// Identity (two sources): the member's own bytes, and the only direct (E8) call in retail, 0x00C75D7F inside 0x00C75CE0, which is
// byte-identical to CODAApi.lib FloatShortReverb.obj ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBVCFloatCyclicBuffer@@PAMPAPAMI@Z
// (432 bytes) whose REL32 +0x0A0 names this symbol; CODAApiDbg.lib names the same function in its CodeView.
// Preconditions (CODAApiDbg /Od ASSERTs, compiled out of release): pfDest and pfSrc non-null and 16-byte aligned, uiSamples
// non-zero and a multiple of 4.
// Portable twin: FABLE_PORTABLE builds compile the plain-C++ definition in engine/ReverbFnsSSEFilterOrder2Quad.h (included
// below); its portability statement is in that header. Dual-path tests: rebuild/tests/portable/test_ReverbFnsSSEFilterOrder2Quad.cpp
// (retail side: the test links the CODAApi.lib member itself) and rebuild/tests/portable/rows/00c88e60_ApplyFilterOrder2_quad_driver.cpp.
// Proof: FABLE_LIBSIG_LIBDIRS=coda=<GameCODA 2.10 CODA/lib/Win32> python3 tools/decomp_pipeline/libsig/lib_verify.py --source <this file>
// -> MATCH, 16 masked relocation bytes, 4/4 relocations proven; docs/pipeline/LIBRARY_SIGNATURES.md "GameCODA 2.10".
#ifdef FABLE_PORTABLE
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
#include "engine/ReverbFnsSSEFilterOrder2Quad.h"
#endif
