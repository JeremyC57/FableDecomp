#include "rebuild_abi.h"

// CTCLifeTimer::StatGetClassTypeName @ 0x004d6549
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d6549
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d6549* FABLE_FASTCALL
FableStatGetClassTypeName_004d6549(FableCharStringResult_004d6549* result, void*)
{
    result->build((const char*)19129380, -1);
    return result;
}