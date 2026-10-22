#include "rebuild_abi.h"

// CTCTavernGameMaster::StatGetClassTypeName @ 0x004d5b19
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d5b19
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d5b19* FABLE_FASTCALL
FableStatGetClassTypeName_004d5b19(FableCharStringResult_004d5b19* result, void*)
{
    result->build((const char*)19125832, -1);
    return result;
}