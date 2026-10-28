// Behaviour test for NUISystem::CIconText::~CIconText (0x005424B0): a handle's Clear notifies the primitive only when
// it holds the last reference and then drops it; the destructor clears the text, drop-shadow and every icon handle,
// drops the font and releases the def's font name from the font manager.
#include <cstdio>
#include <cstring>

struct CPrimitive { long RefCount; int Notified; CPrimitive() : RefCount(0), Notified(0) { } };
class CEnginePrimitiveHandle { public:
    CPrimitive* Data;
    CEnginePrimitiveHandle() : Data(0) { }
    void Set(CPrimitive* p) { Data = p; if (p) ++p->RefCount; }
    void Clear() { if (Data && Data->RefCount == 1) ++Data->Notified; if (Data) --Data->RefCount; Data = 0; }
};
class CFontManager { public: char Released[32]; int Count; CFontManager() : Count(0) { Released[0] = 0; } void ReleaseFont(const char* n) { std::strcpy(Released, n); ++Count; } };
static CFontManager g_FontManager;
struct CIconText {
    CEnginePrimitiveHandle TextHandle, DropShadowHandle, IconHandles[3]; int IconCount; bool HasFont; const char* DefFont;
    ~CIconText()
    {
        TextHandle.Clear();
        DropShadowHandle.Clear();
        for (int i = 0; i < IconCount; ++i) IconHandles[i].Clear();
        HasFont = false;
        g_FontManager.ReleaseFont(DefFont);
    }
};

int main()
{
    CPrimitive text, shadow, icon, shared;
    {
        CIconText t; t.IconCount = 2; t.HasFont = true; t.DefFont = "FONT_MAIN";
        t.TextHandle.Set(&text); t.DropShadowHandle.Set(&shadow); t.IconHandles[0].Set(&icon); t.IconHandles[1].Set(&shared);
        ++shared.RefCount;   // someone else still holds the shared primitive
    }
    bool ok = text.Notified == 1 && shadow.Notified == 1 && icon.Notified == 1 && shared.Notified == 0 && shared.RefCount == 1
        && text.RefCount == 0 && g_FontManager.Count == 1 && !std::strcmp(g_FontManager.Released, "FONT_MAIN");
    if (!ok) { std::printf("005424b0_TEST FAIL\n"); return 1; }
    std::printf("005424b0_TEST PASS\n");
    return 0;
}