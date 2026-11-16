// CPersistContext::Transfer<float>(const char*, float&) -- the two-argument member template: Transfer(name, value, GetDefaultVal<float>())
// with the zero default built in a local and passed by reference to the three-argument Transfer<float>
// Leaf: ??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004107E0.
// NF39 (2026-11-16): moved off its local CPersistContext stand-in (Transfer_long_, a float transfer through a long local) onto the shared
//   engine/CPersistContext.h: Ego_r.pdb + FableWin.pdb ??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z; its landed callers (probe: 0x0057E2F1,
//   0x00F2D6E0, which passes float members 190 times) already bind it. GetDefaultVal's body is the one the landed GetDefaultVal rows
//   (0x0040E260 / 0x0040E250) define.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#include "engine/CPersistContext.h"

template <class T> T FABLE_FASTCALL CPersistContext::GetDefaultVal()
{
    T value = T();
    return value;
}

template void CPersistContext::Transfer<float>(const char* name, float& value);
