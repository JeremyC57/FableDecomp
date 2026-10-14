// CalculateTextureChangeFlags(void* context, const unsigned short* indices) -- for each of the inline texture ids at +0x04 call virtual slot 3 with (context, id, indices[i])
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C581D0, length 64 bytes.
// Callees: vslot +0x0c (virtual, re-fetched each iteration)
// The manifest name is a propagated label and the layout (vptr, 7 inline ids, count at +0x20) is not the
// C3DMesh2 of 0x00A89200; modelled locally as the class the bytes describe. ret 8, unsigned loop bound.
// `xor ecx,ecx ; mov cx,[..] ; push ecx` is an unsigned short passed by value to an unsigned short parameter
// (a long/unsigned long parameter compiles to movzx).
//
#include "rebuild_abi.h"
struct CTextureIdTable {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void ProcessTexture(void* context, unsigned long textureId, unsigned short index);  // slot 3 (+0x0c)
    unsigned long m_textureIds[7];     // +0x04 inline id table
    unsigned long m_numTextures;       // +0x20
    void CalculateTextureChangeFlags(void* context, const unsigned short* indices);
};
void CTextureIdTable::CalculateTextureChangeFlags(void* context, const unsigned short* indices)
{
    for (unsigned long i = 0; i < m_numTextures; ++i)
        ProcessTexture(context, m_textureIds[i], indices[i]);
}
