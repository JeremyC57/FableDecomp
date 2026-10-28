// Test: CFontManager::GetFont -- static symbol map first, then the streaming one; unknown names fall back to the static lookup
#include <cstdio>
#include <cstring>
class CSymbolMap { public:
    const char* names[2]; long ids[2];
    bool SymbolExists(const char* n) const { for (int i = 0; i < 2; ++i) if (names[i] && !std::strcmp(names[i], n)) return true; return false; }
    long GetSymbolSLONG(const char* n) const { for (int i = 0; i < 2; ++i) if (names[i] && !std::strcmp(names[i], n)) return ids[i]; return -1; }
};
struct CFont { int streaming; long index; };
class CFontManager { public:
    CSymbolMap* PSymbols; CSymbolMap* PStreamingSymbols;
    CFont GetStaticFont(unsigned long i) { CFont f = { 0, (long)i }; return f; }
    CFont GetStreamingFont(unsigned long i) { CFont f = { 1, (long)i }; return f; }
    CFont GetFont(const char* name)
    {
        if (PSymbols->SymbolExists(name)) return GetStaticFont(PSymbols->GetSymbolSLONG(name));
        if (PStreamingSymbols->SymbolExists(name)) return GetStreamingFont(PStreamingSymbols->GetSymbolSLONG(name));
        return GetStaticFont(PSymbols->GetSymbolSLONG(name));
    }
};
int main()
{
    CSymbolMap st = { { "Main", "Both" }, { 4, 5 } }, sm = { { "Subs", "Both" }, { 7, 8 } };
    CFontManager m = { &st, &sm };
    CFont a = m.GetFont("Main"), b = m.GetFont("Subs"), c = m.GetFont("Both"), d = m.GetFont("None");
    if (a.streaming || a.index != 4 || !b.streaming || b.index != 7 || c.streaming || c.index != 5 || d.streaming || d.index != -1) { std::printf("009e2c80_TEST FAIL\n"); return 1; }
    std::printf("009e2c80_TEST PASS\n");
    return 0;
}