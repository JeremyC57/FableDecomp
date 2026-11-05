#pragma optimize("s",on)
// Leaf: ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x004da6a9, __fastcall(self /*ecx*/, uint flags /*stack*/)
// Harness rewrites __fastcall->__fastcall, so model this in ecx, a dummy in edx,
// and the real flags on the stack.

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
typedef unsigned int uint;

struct Sub {
    void nonvirt_dtor();
};

class CLandscapeBackgroundPatch { public:
    int   field0;      // +0
    Sub   sub;         // +4
};

// scalar destructor of subobject at +4 (retail 0x00a01c10), __fastcall

// operator delete (retail 0x00bfe9bc), cdecl caller-cleanup

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx dummy*/, uint flags)
{
    ((CBaseIntelligentPointer*)(&self->sub))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}