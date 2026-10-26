#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x004664ff  CResource::`scalar deleting destructor'
// Standard MSVC vector-deleting-destructor thunk.
//   this in ecx, flags on stack (bit0 => operator delete(this)).
// Modeled as a free __fastcall(this) function (byte-identical to __fastcall).

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CLandscapeBackgroundPatch;

// scalar/base destructor (this-only) at 0x596051
// operator delete at 0x7984bd

extern "C" void* __fastcall CResource_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    ((CResource*)(thisptr))->CResource::~CResource();
    if (flags & 1)
    {
        operator delete(thisptr);
    }
    return thisptr;
}