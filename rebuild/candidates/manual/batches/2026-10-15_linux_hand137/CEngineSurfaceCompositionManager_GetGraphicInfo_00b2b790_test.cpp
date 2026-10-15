// Behaviour test for 0x00B2B790 CEngineSurfaceCompositionManager::GetGraphicInfo: top-bit handles use the local table.
#include <cstdio>

struct CGraphicInfoFrame { int id; };
struct SComposedGraphic { unsigned char _pad[0x28]; CGraphicInfoFrame* m_pFrame; };
struct SBankGraphic { unsigned char _pad[0x28]; CGraphicInfoFrame* m_pFrame; };
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    CRetailVector<SBankGraphic> m_records;
    SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
};
struct CEngineGlobal { CGraphicBank* m_pGraphicBank; };
static CEngineGlobal g_engine;
static CEngineGlobal* g_pEngine = &g_engine;
struct CEngineSurfaceCompositionManager {
    CRetailVector<SComposedGraphic> m_composedGraphics;
    SComposedGraphic* GetComposed(unsigned long index) { return index < m_composedGraphics.size() ? &m_composedGraphics[index] : 0; }
    CGraphicInfoFrame* GetGraphicInfo(unsigned long handle)
    {
        if (handle & 0x80000000)
            return GetComposed(handle & 0x7fffffff)->m_pFrame;
        return g_pEngine->m_pGraphicBank->Get(handle)->m_pFrame;
    }
};

int main()
{
    CGraphicInfoFrame f[4];
    SComposedGraphic local[2]; local[0].m_pFrame = &f[0]; local[1].m_pFrame = &f[1];
    SBankGraphic bank[2]; bank[0].m_pFrame = &f[2]; bank[1].m_pFrame = &f[3];
    CGraphicBank b; b.m_records.m_pFirst = bank; b.m_records.m_pLast = bank + 2; b.m_records.m_pEnd = bank + 2;
    g_engine.m_pGraphicBank = &b;
    CEngineSurfaceCompositionManager m; m.m_composedGraphics.m_pFirst = local; m.m_composedGraphics.m_pLast = local + 2; m.m_composedGraphics.m_pEnd = local + 2;
    if (m.GetGraphicInfo(0x80000001u) != &f[1] || m.GetGraphicInfo(0) != &f[2] || m.GetGraphicInfo(1) != &f[3]
        || m.GetComposed(2) != 0 || b.Get(5) != 0) {
        std::printf("00b2b790_TEST FAIL\n"); return 1;
    }
    std::printf("00b2b790_TEST PASS\n");
    return 0;
}
