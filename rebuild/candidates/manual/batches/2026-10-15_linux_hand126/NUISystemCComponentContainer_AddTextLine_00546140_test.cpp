// Behaviour test for 00546140 NUISystem::CComponentContainer::AddTextLine: an empty font name adds the
// line with no font; a named font is looked up through a temporary reference to the front end's font
// bank (the reference count goes up during the lookup and is restored afterwards); text, colour and
// flags pass through unchanged.
#include <cstdio>
#include <string.h>

struct CFont { int id; };
static CFont g_font = { 7 };
static long g_refsDuringLookup = 0;
struct SCountBlock { long m_references; };
static SCountBlock g_block = { 1 };
struct CFontBank { CFont* GetFont(const char* name) { g_refsDuringLookup = g_block.m_references; return strcmp(name, "Big") == 0 ? &g_font : 0; } };
static CFontBank g_bank;
struct CCountedFontBank {
    CFontBank* m_pObject; SCountBlock* m_pCount;
    CCountedFontBank(CFontBank* o, SCountBlock* b) : m_pObject(o), m_pCount(b) { }
    CCountedFontBank(const CCountedFontBank& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedFontBank() { if (m_pCount) --m_pCount->m_references; }
    CFontBank* operator->() const { return m_pObject; }
};
static CCountedFontBank g_frontEndBank(&g_bank, &g_block);
struct CRGBColour { unsigned char B, G, R, A; };
struct CContainer {
    const char* lastText; CRGBColour lastColour; CFont* lastFont; long lastFlags;
    void AddText(const char* text, CRGBColour colour, CFont* font, long flags) { lastText = text; lastColour = colour; lastFont = font; lastFlags = flags; }
    void AddTextLine(const char* text, CRGBColour colour, const char* fontName, long flags)
    {
        CFont* font = 0;
        if (fontName != 0 && fontName[0] != 0) {
            CCountedFontBank bank = g_frontEndBank;
            font = bank->GetFont(fontName);
        }
        AddText(text, colour, font, flags);
    }
};

int main()
{
    CContainer c; CRGBColour col = { 1, 2, 3, 4 };
    c.AddTextLine("hello", col, "", 5);
    bool ok = c.lastFont == 0 && c.lastFlags == 5 && strcmp(c.lastText, "hello") == 0 && c.lastColour.R == 3 && g_refsDuringLookup == 0;
    c.AddTextLine("big", col, "Big", 6);
    ok = ok && c.lastFont == &g_font && c.lastFlags == 6 && g_refsDuringLookup == 2 && g_block.m_references == 1;
    c.AddTextLine("x", col, "Other", 0);
    ok = ok && c.lastFont == 0 && g_block.m_references == 1;
    if (!ok) { std::printf("00546140_TEST FAIL\n"); return 1; }
    std::printf("00546140_TEST PASS\n");
    return 0;
}
