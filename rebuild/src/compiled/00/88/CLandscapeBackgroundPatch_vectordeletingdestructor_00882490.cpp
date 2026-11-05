// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// Leaf: @VecDelDtor@12
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// retail 0x0047b300, __fastcall(self, uint flags) -> void*
//
// Pattern:
//   push esi; mov esi,ecx
//   lea ecx,[esi+4]; call innerDtor   ; scalar dtor of member/base at +4
//   test byte[esp+8],1; je S
//   push esi; call operator delete; add esp,4
// S: mov eax,esi; pop esi; ret 4

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Sub;                 // subobject at offset +4
class CLandscapeBackgroundPatch { public:
    int   m_vtbl;           // +0  (vtable / first dword)
    Sub*  dummy;            // placeholder so member sits at +4
};

// scalar destructor operating on the subobject located at self+4.
// __fastcall => arg1 in ecx. Single pointer param = ecx.

// operator delete (cdecl)

// __fastcall: this in ecx, flags on the stack. Model via __fastcall with an
// unused edx dummy so the real `flags` param lands on the stack (=> ret 4).
extern "C" void* __fastcall VecDelDtor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    // scalar dtor on the subobject at self+4
    ((CBaseIntelligentPointer*)((char*)self + 4))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}