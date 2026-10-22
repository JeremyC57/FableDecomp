#include "rebuild_abi.h"

// CTCActivationTrigger::StatGetClassTypeName @ 0x004d43a6
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d43a6
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d43a6* FABLE_FASTCALL
FableStatGetClassTypeName_004d43a6(FableCharStringResult_004d43a6* result, void*)
{
    result->build((const char*)19118988, -1);
    return result;
}