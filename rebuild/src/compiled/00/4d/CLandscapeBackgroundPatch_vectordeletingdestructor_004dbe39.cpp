#pragma optimize("s",on)
// Leaf: ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
typedef unsigned int uint;

struct Sub;
class CLandscapeBackgroundPatch { public:
    char pad[8];
    Sub* sub;
};


void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, uint flags)
{
    ((CBaseIntelligentPointer*)((char*)self + 8))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}