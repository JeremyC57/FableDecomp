// Behaviour test for NUISystem::CIconText::GetDataFromDef 0x00541FD0: a copy of the model (counted font pointer, def pointer,
// string table, font manager) driven through the same statement sequence; checks the font share / release counts, the window and
// offset copies, the text hand-over, the alignment bit per case and the def release.
#include <cstdio>
#include <cstring>
class CFontBank { public: int Id; };
class CCPPointerInfo { public: long RefCount; };
static int g_fontDeletes = 0;
struct CFontPtr {                                       // CCountedPointer<CFontBank>
    CFontBank* Data; CCPPointerInfo* Info;
    CFontPtr() : Data(0), Info(0) {}
    CFontPtr(CFontBank* d, CCPPointerInfo* i) : Data(d), Info(i) { if (Info) ++Info->RefCount; }
    CFontPtr(const CFontPtr& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void ReleaseData() { if (Info) { if (--Info->RefCount == 0) ++g_fontDeletes; } }
    CFontPtr& operator=(const CFontPtr& o)
    {
        CFontBank* d = o.Data; CCPPointerInfo* i = o.Info;
        if (Info != i) { ReleaseData(); Data = d; Info = i; if (i) ++i->RefCount; }
        return *this;
    }
    ~CFontPtr() { ReleaseData(); Data = 0; Info = 0; }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DVector { public: float X, Y; };
enum ETextAlignement { LEFT = 0, CENTER = 1, RIGHT = 2 };
static const char* g_strings[] = { "none", "FontSmall", "FontBig" };
class CDefString { public: long TablePos; const char* GetString() const { return g_strings[TablePos]; } };
static int g_defDeletes = 0;
struct CUIDef {
    long RefCount;
    CDefString Font; const wchar_t* TextValue; C2DBoxF TextWindow; C2DVector PositionOffset; ETextAlignement Alignement;
};
struct CDefPtr {
    CUIDef* Data;
    explicit CDefPtr(CUIDef* d) : Data(d) { ++Data->RefCount; }
    ~CDefPtr() { if (--Data->RefCount == 0) ++g_defDeletes; }
    const CUIDef* operator->() const { return Data; }
};
static CFontBank g_small = { 1 }, g_big = { 2 };
static CCPPointerInfo g_smallInfo = { 1 }, g_bigInfo = { 1 };
class CFontManager { public:
    CFontPtr GetFont(const char* name)
    {
        if (std::strcmp(name, "FontBig") == 0) return CFontPtr(&g_big, &g_bigInfo);
        return CFontPtr(&g_small, &g_smallInfo);
    }
};
static CFontManager g_fontManager;
struct CIconText {
    CUIDef* Def; unsigned char FlagsSetTwo; CFontPtr PFont; C2DBoxF Window; C2DVector PositionOffset; const wchar_t* Text;
    CDefPtr LoadAndReturnDef() const { return CDefPtr(Def); }
    void SetValue(const wchar_t* const& value) { Text = value; }
    void GetDataFromDef()
    {
        CDefPtr def = LoadAndReturnDef();
        PFont = g_fontManager.GetFont(def->Font.GetString());
        Window = def->TextWindow;
        PositionOffset = def->PositionOffset;
        SetValue(def->TextValue);
        switch (def->Alignement) {
        case LEFT:   FlagsSetTwo |= 0x08; break;
        case CENTER: FlagsSetTwo |= 0x10; break;
        case RIGHT:  FlagsSetTwo |= 0x20; break;
        }
    }
};
static int Fail() { std::printf("00541fd0_TEST FAIL\n"); return 1; }
int main()
{
    static const wchar_t text[] = L"Hello";
    CUIDef def;
    def.RefCount = 1; def.Font.TablePos = 2; def.TextValue = text;
    def.TextWindow.TLX = 1.0f; def.TextWindow.TLY = 2.0f; def.TextWindow.BRX = 3.0f; def.TextWindow.BRY = 4.0f;
    def.PositionOffset.X = 5.0f; def.PositionOffset.Y = 6.0f; def.Alignement = CENTER;
    {
        CIconText t; t.Def = &def; t.FlagsSetTwo = 0x01; t.Text = 0;
        std::memset(&t.Window, 0, sizeof t.Window); t.PositionOffset.X = t.PositionOffset.Y = 0.0f;
        t.GetDataFromDef();
        if (t.PFont.Data != &g_big || t.PFont.Info != &g_bigInfo || g_bigInfo.RefCount != 2) return Fail();   // shared, temp released
        if (t.Window.TLX != 1.0f || t.Window.TLY != 2.0f || t.Window.BRX != 3.0f || t.Window.BRY != 4.0f) return Fail();
        if (t.PositionOffset.X != 5.0f || t.PositionOffset.Y != 6.0f) return Fail();
        if (t.Text != text) return Fail();
        if (t.FlagsSetTwo != 0x11) return Fail();
        if (def.RefCount != 1 || g_defDeletes != 0) return Fail();                                             // def temp released
        def.Font.TablePos = 1; def.Alignement = LEFT;                                                         // new font replaces the old
        t.GetDataFromDef();
        if (t.PFont.Data != &g_small || g_smallInfo.RefCount != 2 || g_bigInfo.RefCount != 1) return Fail();
        if (t.FlagsSetTwo != 0x19) return Fail();
        def.Alignement = RIGHT;                                                                               // same font: no re-share
        t.GetDataFromDef();
        if (g_smallInfo.RefCount != 2 || t.FlagsSetTwo != 0x39) return Fail();
        def.Alignement = (ETextAlignement)3;                                                                  // out of range: no bit
        t.FlagsSetTwo = 0;
        t.GetDataFromDef();
        if (t.FlagsSetTwo != 0) return Fail();
    }
    if (g_smallInfo.RefCount != 1 || g_bigInfo.RefCount != 1 || g_fontDeletes != 0) return Fail();
    def.RefCount = 0;                                                                                         // last def reference
    { CIconText t; t.Def = &def; t.FlagsSetTwo = 0; t.Text = 0; t.GetDataFromDef(); }
    if (g_defDeletes != 1) return Fail();
    std::printf("00541fd0_TEST PASS\n");
    return 0;
}