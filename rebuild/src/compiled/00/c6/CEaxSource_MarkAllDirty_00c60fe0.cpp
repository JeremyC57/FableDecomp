// CEaxSource::MarkAllDirty() -- flag every property of a software EAX source for re-sending (sign bit of the dirty mask at +0x368); returns 0
// Leaf: ?MarkAllDirty@CEaxSource@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60FE0, 13 bytes, thiscall: `or dword ptr [ecx+368h],80000000h ; xor eax,eax ;
//   ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CShaderRenderManager::OnPostViewportChanged 0xC60FE0 =
//   CEaxSource::MarkAllDirty"): agent442's landed CEaxContext::InvalidateAll 0x00C60A30 calls it on each registered source
//   (`result = node->m_pSource->MarkAllDirty(); if (result < 0) return result;`), declared there with this name and
//   comment ("dirty mask +0x368 |= 0x80000000, returns 0"). The landed row was a FABLE_FASTCALL extern "C" stand-in under
//   the propagated label.
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEaxContext (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEaxSource (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEaxSource::Set 0x00C61B40) and the
//   context's source-list nodes CEaxSourceNode (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CPKObject (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
class CEaxSource {                                  // vtable 0x012B9344 (the vptr is not modelled here)
public:
    unsigned char _pad000[0x368];
    unsigned long m_dirty;                          // +0x368 sign bit: re-send every property
    long MarkAllDirty();
};

long CEaxSource::MarkAllDirty()
{
    m_dirty |= 0x80000000;
    return 0;
}
