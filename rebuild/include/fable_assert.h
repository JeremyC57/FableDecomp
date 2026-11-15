#ifndef FABLE_ASSERT_H
#define FABLE_ASSERT_H
// Lionhead's ASSERT as the retail (release) build compiled it -- NEW shared header (agentC6, 2026-11-13), opt-in only.
// FableWin.exe (the debug build, FableWin.pdb) expands every ASSERT(x) to
//     if (!(x)) { bool stop = GFGetSystemManager()->GetDebugManager()->DoErrorMessage(CCharString("x"), CCharString(""),
//                     CWideString(L"<file>.cpp"), __LINE__, <EErrorType 1>);  if (stop && IsDebuggerPresent()) int 3; }
// (ghidra_out/audio_runtime_trace.c, CTCSoundPlayer::PlayCollisionSound @ FableWin 0x017B7447: "pmaterial1.Get()", line 0x5E).
// Retail Fable.exe carries no assert code, but the release form is not empty: it leaves a dead arm the front end still turns
// into IL, which c2 deletes after its record ids are handed out. Those ids decide orderings in retail (measured on row 0x00BB3E80
// CEngineClothRenderer::RenderClothPrimitive: the bookmark test/or operand order of its 16 inline SetRenderState expansions,
// 0x00BB46E0..0x00BB4E80, matches only with the dead arms present; without them 3 expansions come out swapped).
// FABLE_RETAIL_ASSERT_DEAD_ARM: the release form -- the debug control flow kept behind `if (0)`, with the string arguments as
// plain pointers (class-typed temporaries in the dead arm change the live code's register allocation, measured: no such
// arm existed). Emits no bytes. GFAssertFailed is a DESCRIPTIVE name (the dead arm is never emitted, so retail holds no symbol
// for it; the debug build's callee is CDebugManager::DoErrorMessage, Ego_r.pdb 0x00B27E30 / FableWin.pdb 0x02F86100).
// Otherwise (and always under FABLE_PORTABLE): nothing.
#if defined(FABLE_RETAIL_ASSERT_DEAD_ARM) && !defined(FABLE_PORTABLE)
bool __fastcall GFAssertFailed(const char* expression, const char* message, const char* file, unsigned long line);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#define FABLE_RETAIL_ASSERT(c) do { if (0) { if (!(c)) { if (GFAssertFailed(#c, "", __FILE__, __LINE__) && IsDebuggerPresent()) __debugbreak(); } } } while (0)
// FABLE_RETAIL_ASSERT_ARM(c) (wfC1 G1, row 0x00CBFB7D RunCutsceneMacro): the same dead arm as a bare `if (0)` statement, without the
// do/while(0) statement wrapper: 16 IL tuples per expanded site instead of 18 (c2 inline traces of a four-site mini TU). FITTED form:
// the caller-size budget of 0x00CBFB7D bounds the dead IL of one CCharString::StartsWith expansion to 13..16 tuples (see fable_string.h).
#define FABLE_RETAIL_ASSERT_ARM(c) if (0) { if (!(c)) { if (GFAssertFailed(#c, "", __FILE__, __LINE__) && IsDebuggerPresent()) __debugbreak(); } }
#else
#define FABLE_RETAIL_ASSERT(c) ((void)0)
#define FABLE_RETAIL_ASSERT_ARM(c) ((void)0)
#endif
#endif
