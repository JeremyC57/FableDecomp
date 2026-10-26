// Behaviour test for 0x00527200 CCharString::ReplaceAll: every occurrence of the key is replaced (including adjacent
// and trailing ones), a missing key leaves the text unchanged, an empty string stays empty, and no temporary leaks.
#include <cstdio>
#include <cstring>
#include <cstdlib>
using std::strstr; using std::strlen;
static int g_live;

class CCharStringData { public:
    char*         m_pData;                      // +0x0
    unsigned long m_length;                     // +0x4
};
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0 (null = empty)
    CCharString() : PStringData(0) { ++g_live; }
    CCharString(const char* text, int length = -1) : PStringData(0) { ++g_live; Set(text, length < 0 ? (int)std::strlen(text) : length); }
    ~CCharString() { --g_live; Free(); }
    void Free() { if (PStringData) { std::free(PStringData->m_pData); delete PStringData; PStringData = 0; } }
    void Set(const char* text, int n)
    {
        Free();
        PStringData = new CCharStringData; PStringData->m_pData = (char*)std::malloc(n + 1);
        std::memcpy(PStringData->m_pData, text, n); PStringData->m_pData[n] = 0; PStringData->m_length = n;
    }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) { const char* t = other.GetString(); int n = (int)std::strlen(t); char* c = (char*)std::malloc(n + 1);
            std::memcpy(c, t, n + 1); Set(c, n); std::free(c); }
        return *this;
    }
    CCharString& operator+=(const CCharString& other)
    {
        const char* a = GetString(); const char* b = other.GetString();
        int na = (int)std::strlen(a), nb = (int)std::strlen(b);
        char* c = (char*)std::malloc(na + nb + 1); std::memcpy(c, a, na); std::memcpy(c + na, b, nb + 1);
        Set(c, na + nb); std::free(c);
        return *this;
    }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    void ReplaceAll(const CCharString& find, const CCharString& replacement);
};
void CCharString::ReplaceAll(const CCharString& find, const CCharString& replacement)
{
    CCharString result;
    char* start = (char*)GetString();
    const char* key = find.GetString();
    char* hit = strstr(start, key);
    while (hit) {
        *hit = 0;
        hit += strlen(key);
        result += CCharString(start);
        result += replacement;
        start = hit;
        hit = strstr(hit, key);
    }
    result += CCharString(start);
    *this = result;
}
int main()
{
    int ok = 1;
    {
        CCharString s("a.b..c."), f("."), r("<>");
        s.ReplaceAll(f, r);
        ok &= std::strcmp(s.GetString(), "a<>b<><>c<>") == 0;
        CCharString t("HERO_NAME says HERO_NAME"), k("HERO_NAME"), v("Sparrow");
        t.ReplaceAll(k, v);
        ok &= std::strcmp(t.GetString(), "Sparrow says Sparrow") == 0;
        CCharString u("nothing here"), x("zz");
        u.ReplaceAll(x, v);
        ok &= std::strcmp(u.GetString(), "nothing here") == 0;
        CCharString e;
        e.ReplaceAll(x, v);
        ok &= std::strcmp(e.GetString(), "") == 0;
        CCharString w("abab"), ab("ab"), none;
        w.ReplaceAll(ab, none);
        ok &= std::strcmp(w.GetString(), "") == 0;
    }
    ok &= g_live == 0;
    if (!ok) { std::printf("00527200_TEST FAIL\n"); return 1; }
    std::printf("00527200_TEST PASS\n");
    return 0;
}