#include "rebuild_abi.h"

// CTCCombat::StatGetClassTypeName @ 0x004d3533
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d3533
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d3533* FABLE_FASTCALL
FableStatGetClassTypeName_004d3533(FableCharStringResult_004d3533* result, void*)
{
    result->build((const char*)19114556, -1);
    return result;
}