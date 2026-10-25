// Behaviour test for InitializeMusicTypes (0x00CBEB1A): the first call builds the symbol map and loads
// "<defs dir>atmos_types.h"; later calls do nothing.
#include <cstdio>

struct CWideString {
    wchar_t text[64];
    CWideString() { text[0] = 0; }
    CWideString(const wchar_t* t) { int i = 0; for (; t[i]; ++i) text[i] = t[i]; text[i] = 0; }
};
static int g_dirCalls = 0;
CWideString GetDefsDirectory() { ++g_dirCalls; return CWideString(L"Data\\Defs\\"); }
CWideString operator+(const CWideString& left, const wchar_t* right)
{
    CWideString r(left);
    int n = 0;
    while (r.text[n]) ++n;
    for (int i = 0; right[i]; ++i) r.text[n++] = right[i];
    r.text[n] = 0;
    return r;
}
struct CSymbolMap {
    int loads;
    CWideString file;
    CSymbolMap() : loads(0) { }
    bool LoadFromFile(const CWideString& f) { ++loads; file = f; return true; }
};
CSymbolMap* g_pMusicTypeSymbolMap = 0;

void InitializeMusicTypes()
{
    if (g_pMusicTypeSymbolMap == 0) {
        g_pMusicTypeSymbolMap = new CSymbolMap;
        g_pMusicTypeSymbolMap->LoadFromFile(GetDefsDirectory() + L"atmos_types.h");
    }
}

static bool Same(const wchar_t* a, const wchar_t* b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}

int main()
{
    InitializeMusicTypes();
    CSymbolMap* first = g_pMusicTypeSymbolMap;
    InitializeMusicTypes();
    if (first == 0 || g_pMusicTypeSymbolMap != first || first->loads != 1 || g_dirCalls != 1
        || !Same(first->file.text, L"Data\\Defs\\atmos_types.h")) {
        std::printf("00cbeb1a_TEST FAIL\n");
        return 1;
    }
    delete first;
    std::printf("00cbeb1a_TEST PASS\n");
    return 0;
}