#include "rebuild_abi.h"

// CTCCameraPointScripted::StatGetClassTypeName @ 0x004d3beb
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d3beb
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d3beb* FABLE_FASTCALL
FableStatGetClassTypeName_004d3beb(FableCharStringResult_004d3beb* result, void*)
{
    result->build((const char*)19116748, -1);
    return result;
}