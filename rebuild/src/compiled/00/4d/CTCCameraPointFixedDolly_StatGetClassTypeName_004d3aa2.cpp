#include "rebuild_abi.h"

// CTCCameraPointFixedDolly::StatGetClassTypeName @ 0x004d3aa2
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d3aa2
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d3aa2* FABLE_FASTCALL
FableStatGetClassTypeName_004d3aa2(FableCharStringResult_004d3aa2* result, void*)
{
    result->build((const char*)19116060, -1);
    return result;
}