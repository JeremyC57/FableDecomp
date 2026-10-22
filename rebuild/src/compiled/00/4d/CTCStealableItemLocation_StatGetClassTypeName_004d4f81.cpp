#include "rebuild_abi.h"

// CTCStealableItemLocation::StatGetClassTypeName @ 0x004d4f81
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d4f81
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d4f81* FABLE_FASTCALL
FableStatGetClassTypeName_004d4f81(FableCharStringResult_004d4f81* result, void*)
{
    result->build((const char*)19121720, -1);
    return result;
}