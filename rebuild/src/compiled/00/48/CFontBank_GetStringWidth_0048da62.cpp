// CFontBank::GetStringWidth(const CWideString&) const @ 0x0048DA62
// Flags: /O2 /Oy
// Leaf: ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z
// The shared inline definition is emitted by the 00541A00 shipping object.
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_ICONTEXT_FIT_PASS30
#include "engine/CFontBank.h"

// Fidelity: the private instantiation requests the existing inline member for standalone object builds.
namespace {
typedef float (CFontBank::*Owner)(const CWideString&) const;
template<Owner Method>
#if defined(__GNUC__) || defined(__clang__)
__attribute__((used))
#endif
static Owner EmitOriginalMember() { return Method; }
#if defined(_MSC_VER) && _MSC_VER == 1310 && !defined(__clang__)
template Owner EmitOriginalMember<static_cast<Owner>(&CFontBank::GetStringWidth)>();
#else
template Owner EmitOriginalMember<&CFontBank::GetStringWidth>();
#endif
}
