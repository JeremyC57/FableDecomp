#include "rebuild_abi.h"

// CTCChest::StatGetClassTypeName @ 0x004d3813
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d3813
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d3813* FABLE_FASTCALL
FableStatGetClassTypeName_004d3813(FableCharStringResult_004d3813* result, void*)
{
    result->build((const char*)19115208, -1);
    return result;
}