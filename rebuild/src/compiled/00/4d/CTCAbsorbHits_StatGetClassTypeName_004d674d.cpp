#include "rebuild_abi.h"

// CTCAbsorbHits::StatGetClassTypeName @ 0x004d674d
struct FableCharStringResult_004d674d
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d674d* FABLE_FASTCALL
FableStatGetClassTypeName_004d674d(FableCharStringResult_004d674d* result, void*)
{
    result->build((const char*)19130188, -1);
    return result;
}