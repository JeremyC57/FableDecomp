// fable_test_mode.h -- the build-mode label every dual-path test prints (tools/decomp_pipeline/portable_twins_check.py
// looks for PASS on the last line): VC7.1 as-is = the asm path, /DFABLE_DUAL_PATH_TEST = asm and twin compared,
// /DFABLE_PORTABLE = the twin alone.
#ifndef FABLE_TEST_MODE_H
#define FABLE_TEST_MODE_H
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
#define FABLE_TEST_DUAL 1
static const char* const kFableTestMode = "asm+portable";
#elif defined(FABLE_PORTABLE)
static const char* const kFableTestMode = "portable";
#else
static const char* const kFableTestMode = "asm";
#endif
#if defined(_MSC_VER)
#define FABLE_TEST_ALIGN16 __declspec(align(16))
#else
#define FABLE_TEST_ALIGN16 __attribute__((aligned(16)))
#endif
#endif
