#include "rebuild_abi.h"

// CTCAISummon::StatGetClassTypeName @ 0x004d61a4
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d61a4
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d61a4* FABLE_FASTCALL
FableStatGetClassTypeName_004d61a4(FableCharStringResult_004d61a4* result, void*)
{
    result->build((const char*)19128088, -1);
    return result;
}