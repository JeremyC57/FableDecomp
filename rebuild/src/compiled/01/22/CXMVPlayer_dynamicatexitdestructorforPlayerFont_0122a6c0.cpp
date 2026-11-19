// `dynamic atexit destructor for 'CXMVPlayer::PlayerFont'' -- releases the private static CCountedPointer<CFontBank> @ 0x013CA7F0 at exit
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A6C0, 57 bytes (the compiler-generated `??__F` atexit destructor; VC7.1 names it
//   the TU-local `_$E2`, as the `_$E1` dynamic initializers: docs/pipeline/GOTCHAS.md).
// Name: Ego_d.pdb PUB32 ?PlayerFont@CXMVPlayer@@0V?$CCountedPointer@VCFontBank@@@@A (private static, egod_carved_names.tsv) and
//   FableWin.pdb 0x04012630 / Ego_r.pdb 0x0101E390 `dynamic atexit destructor for 'CXMVPlayer::PlayerFont''; engine/CXMVPlayer.h
//   (GETCHARWIDTHW opt-in) declares the member; the landed CXMVPlayer::GetCharWidthW 0x009BCE70 binds it. The body is
//   ~CCountedPointer<CFontBank>() inline (ReleaseData: decrement the info block, call its deleter on the data and free the block on the
//   last reference, then null both words). NF95 2026-11-19 (q11313): was the stand-in Release_013ca7f0 on g_pointer_013ca7f0.
// Defining the static member makes VC7.1 emit this function (and its dynamic initializer 0x01224070 as `_$E1`).
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CXMVPLAYER_GETCHARWIDTHW
#include "engine/CCountedPointer.h"
#include "engine/CXMVPlayer.h"
CCountedPointer<CFontBank> CXMVPlayer::PlayerFont;
