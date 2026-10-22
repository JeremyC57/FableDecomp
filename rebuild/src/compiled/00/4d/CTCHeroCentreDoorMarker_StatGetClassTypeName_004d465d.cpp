#include "rebuild_abi.h"

// CTCHeroCentreDoorMarker::StatGetClassTypeName @ 0x004d465d
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d465d
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d465d* FABLE_FASTCALL
FableStatGetClassTypeName_004d465d(FableCharStringResult_004d465d* result, void*)
{
    result->build((const char*)19119800, -1);
    return result;
}