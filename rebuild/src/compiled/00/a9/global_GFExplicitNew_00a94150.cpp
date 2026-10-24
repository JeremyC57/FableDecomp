// GFExplicitNew(T* p, unsigned long n) -- placement-construct n default N3DPrimitive::CStaticBlock objects in raw storage at p
// Leaf: GFExplicitNew
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A94150, length 61 bytes (`ret`; __fastcall p, n).
// Identity: the label is genuine -- the only caller is C3DPrimitive2::LoadBinary, which allocates the static blocks.
// Model: CStaticBlock = CBlockBase {start, count, three flag bytes} + fixup offsets, texture pointers and MaterialIndex (-1),
//   from Ego_r.pdb member names; no engine header exists for the N3DPrimitive blocks.
// Shape: `ebx = 0` holds the zero for every store and `ecx = -1` the MaterialIndex; the pointer walks at +8 into the block.
#include <new>
struct CTexture;
namespace N3DPrimitive {
struct CBlockBase {
    long m_start; long m_count; unsigned char m_flag0, m_flag1, m_flag2;
    CBlockBase() : m_start(0), m_count(0), m_flag0(0), m_flag1(0), m_flag2(0) {}
};
struct CStaticBlock : CBlockBase {
    unsigned long DiffuseTextureFixupOffset; unsigned long BumpTextureFixupOffset;
    CTexture* DiffuseTexture; CTexture* BumpTexture; long MaterialIndex;
    CStaticBlock() : DiffuseTextureFixupOffset(0), BumpTextureFixupOffset(0), DiffuseTexture(0), BumpTexture(0), MaterialIndex(-1) {}
};
}
template <class T> void __fastcall GFExplicitNew(T* p, unsigned long n)
{
    for (unsigned long i = 0; i < n; ++i)
        new (&p[i]) T;
}
template void __fastcall GFExplicitNew<N3DPrimitive::CStaticBlock>(N3DPrimitive::CStaticBlock*, unsigned long);
