// Behaviour test for 0x0122A610 NStreamingFontBankGlobals dynamic atexit destructor for g_PendingLoadedGlyphs
#include <cstdio>
static int g_deleted = 0;
static int g_order[8];
static int g_orderCount = 0;
struct SCountInfo {
    long Count;
    void (*DeleteData)(void* object);
    void* Object;
};
static void DeleteGlyph(void* object) { ++g_deleted; g_order[g_orderCount++] = *(int*)object; }
struct CCountedPointer {
    int* Data;
    SCountInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->DeleteData(Info->Object);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct SPendingLoadedGlyph { CCountedPointer Block; bool Loaded; unsigned long GlyphIndex; };
static void Attach(SPendingLoadedGlyph& g, int* object, SCountInfo* info) { g.Block.Data = object; g.Block.Info = info; ++info->Count; }
int main()
{
    int objects[3] = { 10, 20, 30 };
    int ok;
    {
        SPendingLoadedGlyph glyphs[4];             // destroyed last to first, as the atexit thunk does
        SCountInfo* a = new SCountInfo; a->Count = 0; a->DeleteData = DeleteGlyph; a->Object = &objects[0];
        SCountInfo* b = new SCountInfo; b->Count = 0; b->DeleteData = DeleteGlyph; b->Object = &objects[1];
        SCountInfo* c = new SCountInfo; c->Count = 0; c->DeleteData = DeleteGlyph; c->Object = &objects[2];
        Attach(glyphs[0], &objects[0], a);
        Attach(glyphs[1], &objects[1], b);
        Attach(glyphs[3], &objects[1], b);       // shared block: freed only on its last release
        Attach(glyphs[2], &objects[2], c);
        c->Count = 2;                             // an extra owner elsewhere keeps c alive
        ok = 1;
    }
    ok = ok && g_deleted == 2 && g_order[0] == 20 && g_order[1] == 10;
    if (!ok) { std::printf("0122a610_TEST FAIL\n"); return 1; }
    std::printf("0122a610_TEST PASS\n");
    return 0;
}