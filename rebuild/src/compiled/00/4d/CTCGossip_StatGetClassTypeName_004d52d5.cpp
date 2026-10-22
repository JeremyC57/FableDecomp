#include "rebuild_abi.h"

// CTCGossip::StatGetClassTypeName @ 0x004d52d5
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d52d5
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d52d5* FABLE_FASTCALL
FableStatGetClassTypeName_004d52d5(FableCharStringResult_004d52d5* result, void*)
{
    result->build((const char*)19122984, -1);
    return result;
}