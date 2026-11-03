// Behaviour test for 0x00B3B4F0 (see src/00b3b4f0.cpp): ResizeHiresTextureMemory sizing the DXT1 pool and restoring the bank level.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b3b4f0_TEST FAIL\n"); return 1; } } while (0)

class CPixelFormat { public: unsigned int m_format; void Initialise(unsigned int f) { m_format = f; } };
static int CalculateTextureSize(unsigned int w, unsigned int h, bool mips, const CPixelFormat& f)
{
    int size = (f.m_format == 0x31545844u) ? (int)(w * h / 2) : (int)(w * h * 4);
    return mips ? size + size / 3 : size;
}
class CTextureManager { public: int size; int count; int levelSeen; bool ResizePoolByFormat(int s, int c) { size = s; count = c; return c > 0; } };
class CGraphicDataBank { public:
    int m_level; CTextureManager* m_pTextureManager; int sceneChanges; int setCalls;
    void ChangeScene() { ++sceneChanges; }
    void SetLevel(int l) { m_level = l; ++setCalls; if (m_pTextureManager) m_pTextureManager->levelSeen = l; }
};
class CSurface { public: int flushes; void FlushCache() { ++flushes; } };
static CSurface g_surface = { 0 };
class CEngineResourceManager { public:
    CGraphicDataBank* m_pGraphicBank; CGraphicDataBank* m_pMainGraphicBank;
    bool ResizeHiresTextureMemory(int sizeKB)
    {
        m_pGraphicBank->ChangeScene();
        g_surface.FlushCache();
        CPixelFormat format; format.Initialise(0x31545844u);
        int textureSize = CalculateTextureSize(0x200, 0x200, true, format);
        int level = m_pMainGraphicBank->m_level;
        m_pMainGraphicBank->SetLevel(0);
        bool result = m_pMainGraphicBank->m_pTextureManager->ResizePoolByFormat(textureSize, (sizeKB << 10) / textureSize);
        m_pMainGraphicBank->SetLevel(level);
        return result;
    }
};
int main()
{
    CTextureManager tm = { 0, 0, -1 };
    CGraphicDataBank bank = { 0, 0, 0, 0 }, mainBank = { 3, &tm, 0, 0 };
    CEngineResourceManager m; m.m_pGraphicBank = &bank; m.m_pMainGraphicBank = &mainBank;
    CHECK(m.ResizeHiresTextureMemory(8192));
    CHECK(bank.sceneChanges == 1 && g_surface.flushes == 1 && tm.size == 174762 && tm.count == (8192 << 10) / 174762);
    CHECK(mainBank.m_level == 3 && mainBank.setCalls == 2 && tm.levelSeen == 3);
    CHECK(!m.ResizeHiresTextureMemory(16));
    std::printf("00b3b4f0_TEST PASS\n");
    return 0;
}