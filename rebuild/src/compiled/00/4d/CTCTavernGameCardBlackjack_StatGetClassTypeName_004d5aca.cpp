#include "rebuild_abi.h"

// CTCTavernGameCardBlackjack::StatGetClassTypeName @ 0x004d5aca
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d5aca
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d5aca* FABLE_FASTCALL
FableStatGetClassTypeName_004d5aca(FableCharStringResult_004d5aca* result, void*)
{
    result->build((const char*)19125668, -1);
    return result;
}