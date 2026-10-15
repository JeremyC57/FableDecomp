// Behaviour test for 0x00BFA8A0 (CLocalDetailPrimitiveMeshZSpriteBatch::RelocateData): rebase the pointer inside the moved block, then refresh the primitive.
#include <cstdio>
static int g_updates;
inline bool IsInBlock(const char* p, const char* base, unsigned int size) { return p && p >= base && p < base + size; }
inline void Rebase(char*& p, const char* oldBase, char* newBase) { if (p) p = p - oldBase + newBase; }
struct Batch {
    char* m_pVertexBlock; char* m_pIndexBlock;
    void RelocateData(void*, char* oldBase, char* newBase, unsigned int size)
    {
        if (IsInBlock(m_pVertexBlock, oldBase, size)) Rebase(m_pVertexBlock, oldBase, newBase);
        else if (IsInBlock(m_pIndexBlock, oldBase, size)) Rebase(m_pIndexBlock, oldBase, newBase);
        ++g_updates;
    }
};
int main()
{
    static char oldMem[64], newMem[64], other[64];
    Batch b; b.m_pVertexBlock = oldMem + 8; b.m_pIndexBlock = oldMem + 16;
    b.RelocateData(0, oldMem, newMem, 64);
    int ok = b.m_pVertexBlock == newMem + 8 && b.m_pIndexBlock == oldMem + 16 && g_updates == 1;   // only the first one
    b.m_pVertexBlock = other; b.RelocateData(0, oldMem, newMem, 64);
    ok &= b.m_pVertexBlock == other && b.m_pIndexBlock == newMem + 16 && g_updates == 2;
    b.m_pVertexBlock = 0; b.m_pIndexBlock = 0; b.RelocateData(0, oldMem, newMem, 64);
    ok &= b.m_pVertexBlock == 0 && b.m_pIndexBlock == 0 && g_updates == 3;
    if (!ok) { std::printf("00bfa8a0_TEST FAIL\n"); return 1; }
    std::printf("00bfa8a0_TEST PASS\n");
    return 0;
}