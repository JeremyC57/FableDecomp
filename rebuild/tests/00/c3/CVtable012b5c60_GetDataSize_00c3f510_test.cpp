// Behaviour test for CVtable_012b5c60::GetDataSize (0x00C3F510): rows * row size, plus the owner descriptor's extra
// size when the keys differ; 0 without an owner. The descriptor accessor is called twice only when the keys differ.
#include <cstdio>
struct CDescriptor { long m_extraSize; long m_key; };
static int g_calls = 0;
struct COwner { CDescriptor m_descriptor; CDescriptor* GetDescriptor() { ++g_calls; return &m_descriptor; } };
struct CObj {
    COwner* m_pOwner; long m_key; long m_rowSize; long m_rowCount;
    long GetDataSize()
    {
        if (m_pOwner) {
            long key = m_key;
            long size = m_rowCount * m_rowSize;
            if (key != m_pOwner->GetDescriptor()->m_key)
                size += m_pOwner->GetDescriptor()->m_extraSize;
            return size;
        }
        return 0;
    }
};
int main()
{
    COwner owner; owner.m_descriptor.m_extraSize = 100; owner.m_descriptor.m_key = 5;
    CObj o; o.m_pOwner = &owner; o.m_key = 5; o.m_rowSize = 16; o.m_rowCount = 3;
    bool ok = o.GetDataSize() == 48 && g_calls == 1;
    o.m_key = 6; g_calls = 0;
    ok = ok && o.GetDataSize() == 148 && g_calls == 2;
    o.m_pOwner = 0; g_calls = 0;
    ok = ok && o.GetDataSize() == 0 && g_calls == 0;
    if (!ok) { std::printf("00c3f510_TEST FAIL\n"); return 1; }
    std::printf("00c3f510_TEST PASS\n");
    return 0;
}