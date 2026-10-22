#include "rebuild_abi.h"

// CTCActionUseActivateQuest::StatGetClassTypeName @ 0x004d57fa
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d57fa
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d57fa* FABLE_FASTCALL
FableStatGetClassTypeName_004d57fa(FableCharStringResult_004d57fa* result, void*)
{
    result->build((const char*)19124932, -1);
    return result;
}