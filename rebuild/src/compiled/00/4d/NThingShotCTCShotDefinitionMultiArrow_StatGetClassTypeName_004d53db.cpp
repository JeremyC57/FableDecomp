#include "rebuild_abi.h"

// NThingShot::CTCShotDefinitionMultiArrow::StatGetClassTypeName @ 0x004d53db
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d53db
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d53db* FABLE_FASTCALL
FableStatGetClassTypeName_004d53db(FableCharStringResult_004d53db* result, void*)
{
    result->build((const char*)19123492, -1);
    return result;
}