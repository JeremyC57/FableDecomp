// SRecord_005bc277::`scalar deleting destructor'(unsigned int flags) -- destroy the member at +0xc (0x005BC0D3, labelled "CActiveFile::OnReadFinished") and free the record for flag bit 0 (the size-optimised module)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC277, length 31 bytes.
// Leaf: ??_GSRecord_005bc277@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: no vtable (a non-polymorphic class deleted with `delete`), `this` in esi, `test byte ptr [esp+8],1`, `ret 4`.
#pragma optimize("s", on)
// Name fix (agentNF54, nf44:5): the local CMember_005bc0d3 stand-in is gone: the member is the shared std::vector<CEngineTextureMorph>
//   (engine/CEngineTextureMorph.h), whose destructor stays the out-of-line COMDAT (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE) and binds
//   FableWin.pdb ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ (retail 0x005BC0D3).
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CEngineTextureMorph.h"
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SRecord_005bc277 {
    unsigned char      _pad[0xc];
    std::vector<CEngineTextureMorph>   m_member;                   // +0x0c
    ~SRecord_005bc277() { }
};

void Kill_005bc277(SRecord_005bc277* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}