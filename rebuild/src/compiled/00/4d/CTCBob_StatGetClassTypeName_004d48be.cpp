#include "rebuild_abi.h"

// CTCBob::StatGetClassTypeName @ 0x004d48be
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d48be
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d48be* FABLE_FASTCALL
FableStatGetClassTypeName_004d48be(FableCharStringResult_004d48be* result, void*)
{
    result->build((const char*)19120392, -1);
    return result;
}