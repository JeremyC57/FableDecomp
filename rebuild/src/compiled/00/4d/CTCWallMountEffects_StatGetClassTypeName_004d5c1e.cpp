#include "rebuild_abi.h"

// CTCWallMountEffects::StatGetClassTypeName @ 0x004d5c1e
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d5c1e
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d5c1e* FABLE_FASTCALL
FableStatGetClassTypeName_004d5c1e(FableCharStringResult_004d5c1e* result, void*)
{
    result->build((const char*)19126068, -1);
    return result;
}