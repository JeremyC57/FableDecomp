#include <cstdio>
#include <cstring>
#include <cmath>
struct SCountInfo { int Count; void (*DeleteData)(void*); void* Object; };
static int g_deleted = 0;
static void Del(void*) { ++g_deleted; }
template <class T> class CCountedPointer {
public:
    T* Data; SCountInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData() { if (Info) { --Info->Count; if (Info->Count == 0) { Info->DeleteData(Info->Object); delete Info; } } Data = 0; Info = 0; }
};
struct CGlyphBlock { int X; };
struct SPendingLoadedGlyph { CCountedPointer<CGlyphBlock> Block; bool Loaded; unsigned int GlyphIndex; };

int main()
{
    {
        SPendingLoadedGlyph* g = new SPendingLoadedGlyph[512];
        for (int i = 0; i < 512; ++i) if (g[i].Block.Data != 0 || g[i].Block.Info != 0) do { std::printf("01223fc0_TEST FAIL\n"); return 1; } while (0);
        SCountInfo* info = new SCountInfo; info->Count = 1; info->DeleteData = Del; info->Object = 0;
        g[7].Block.Info = info;
        delete[] g;                                       // the $E2 half releases each slot
    }
    if (g_deleted != 1) do { std::printf("01223fc0_TEST FAIL\n"); return 1; } while (0);
    std::printf("01223fc0_TEST PASS\n");
    return 0;
}