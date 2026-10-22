#include "rebuild_abi.h"

// NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName @ 0x004d5562
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d5562
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d5562* FABLE_FASTCALL
FableStatGetClassTypeName_004d5562(FableCharStringResult_004d5562* result, void*)
{
    result->build((const char*)19124392, -1);
    return result;
}