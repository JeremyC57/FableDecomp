// behaviour test for 0x0071AE90 CTCEmoteIcon::DrawPreparePrimitivesForRendering
#include <cstdio>
struct Prim { long RefCount; int lastHandle; int destroyed; };
struct Handle { Prim* Data;
    void Release() { if (Data && Data->RefCount == 1) Data->lastHandle++; if (Data) { if (--Data->RefCount == 0) Data->destroyed++; Data = 0; } Data = 0; } };
static bool g_guiAllows = true;
struct Emote {
    long ActiveTimer; long EmoteType; bool Enabled; bool drawable; Handle PrimitiveHandle; int icons, texts;
    void Draw()
    {
        if (ActiveTimer != 0 && Enabled && g_guiAllows && drawable)
        {
            if (EmoteType == 1) { ++icons; return; }
            if (EmoteType == 0) { ++texts; return; }
        }
        PrimitiveHandle.Release();
    }
};
int main()
{
    Prim p = { 1, 0, 0 };
    Emote e = { 5, 1, true, true, { &p }, 0, 0 };
    e.Draw();
    bool ok = e.icons == 1 && e.PrimitiveHandle.Data == &p;
    e.EmoteType = 0; e.Draw();
    ok = ok && e.texts == 1;
    e.EmoteType = 2; e.Draw();
    ok = ok && e.PrimitiveHandle.Data == 0 && p.lastHandle == 1 && p.destroyed == 1;
    e.EmoteType = 1; g_guiAllows = false; e.Draw();
    ok = ok && e.icons == 1;
    if (!ok) { std::printf("0071ae90_TEST FAIL\n"); return 1; }
    std::printf("0071ae90_TEST PASS\n");
    return 0;
}