// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// Leaf: @VecDelDtor@12
// retail 0x0047b300, __fastcall(self, uint flags) -> void*
//
// Pattern:
//   push esi; mov esi,ecx
//   lea ecx,[esi+4]; call innerDtor   ; scalar dtor of member/base at +4
//   test byte[esp+8],1; je S
//   push esi; call operator delete; add esp,4
// S: mov eax,esi; pop esi; ret 4

// Name fix (agentNF54, nf44:5): the extern "C" InnerDtor_005bc0d3 stand-in is gone; the +4 member is destroyed through the shared
//   std::vector<CEngineTextureMorph> (engine/CEngineTextureMorph.h), whose out-of-line destructor (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE)
//   binds FableWin.pdb ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ (retail 0x005BC0D3). The
//   enclosing class (catalog _global::ScalarDeletingDtor_007220f0) stays unproven.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CEngineTextureMorph.h"

struct Sub;                 // subobject at offset +4
class CLandscapeBackgroundPatch { public:
    int   m_vtbl;           // +0  (vtable / first dword)
    Sub*  dummy;            // placeholder so member sits at +4
};

// The subobject at self+4 is a vector<CEngineTextureMorph> (its out-of-line destructor 0x005BC0D3).

// operator delete (cdecl)

// __fastcall: this in ecx, flags on the stack. Model via __fastcall with an
// unused edx dummy so the real `flags` param lands on the stack (=> ret 4).
extern "C" void* __fastcall VecDelDtor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    // scalar dtor on the subobject at self+4
    reinterpret_cast<std::vector<CEngineTextureMorph>*>((char*)self + 4)->~vector();
    if (flags & 1)
        operator delete(self);
    return self;
}