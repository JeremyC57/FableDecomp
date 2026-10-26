#include <cstdio>
#include <new>
class CTexture;
namespace N3DPrimitive {
struct CBlockBase {
    long m_start; long m_count; unsigned char m_flag0, m_flag1, m_flag2;
    CBlockBase() : m_start(0), m_count(0), m_flag0(0), m_flag1(0), m_flag2(0) {}
};
class CStaticBlock : public CBlockBase { public:
    unsigned long DiffuseTextureFixupOffset, BumpTextureFixupOffset;
    CTexture* DiffuseTexture;
    CTexture* BumpTexture;
    long MaterialIndex;
    CStaticBlock() : DiffuseTextureFixupOffset(0), BumpTextureFixupOffset(0), DiffuseTexture(0), BumpTexture(0), MaterialIndex(-1) {}
};
}
// The candidate logic: placement-construct n default blocks.
template <class T> void ExplicitNew(T* p, unsigned long n)
{
    for (unsigned long i = 0; i < n; ++i)
        new (&p[i]) T;
}
int main()
{
    unsigned char buffer[sizeof(N3DPrimitive::CStaticBlock) * 4];
    unsigned long k = 0;
    for (; k < sizeof(buffer); ++k)
        buffer[k] = 0xAB;
    N3DPrimitive::CStaticBlock* blocks = (N3DPrimitive::CStaticBlock*)buffer;
    ExplicitNew(blocks, 3);
    bool ok = true;
    unsigned long i = 0;
    for (; i < 3; ++i) {
        const N3DPrimitive::CStaticBlock& b = blocks[i];
        if (b.m_start != 0 || b.m_count != 0 || b.m_flag0 != 0 || b.m_flag2 != 0 || b.DiffuseTexture != 0 || b.BumpTexture != 0 ||
            b.DiffuseTextureFixupOffset != 0 || b.BumpTextureFixupOffset != 0 || b.MaterialIndex != -1)
            ok = false;
    }
    // the fourth block is untouched
    if (buffer[sizeof(N3DPrimitive::CStaticBlock) * 3] != 0xAB)
        ok = false;
    if (!ok) { std::printf("00a94150_TEST FAIL\n"); return 1; }
    std::printf("00a94150_TEST PASS\n");
    return 0;
}