#include "rebuild_abi.h"

// CTCDoor::StatGetClassTypeName @ 0x004d32f7
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d32f7
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d32f7* FABLE_FASTCALL
FableStatGetClassTypeName_004d32f7(FableCharStringResult_004d32f7* result, void*)
{
    result->build((const char*)19113736, -1);
    return result;
}