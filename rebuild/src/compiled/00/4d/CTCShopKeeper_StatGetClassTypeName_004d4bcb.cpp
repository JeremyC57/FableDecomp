#include "rebuild_abi.h"

// CTCShopKeeper::StatGetClassTypeName @ 0x004d4bcb
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d4bcb
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d4bcb* FABLE_FASTCALL
FableStatGetClassTypeName_004d4bcb(FableCharStringResult_004d4bcb* result, void*)
{
    result->build((const char*)19121000, -1);
    return result;
}