#include <cstdio>
#include <cstdlib>
#include <cstring>
// CBasePin::QueryId: AMGetWideString(m_pName, Id) -- an allocated copy of the pin name.
typedef unsigned short WCHAR;
static int AMGetWideString(const WCHAR* psz, WCHAR** ppszReturn)
{
    if (ppszReturn == 0) return (int)0x80004003;
    unsigned int n = 0;
    while (psz[n]) ++n;
    *ppszReturn = (WCHAR*)std::malloc((n + 1) * sizeof(WCHAR));
    std::memcpy(*ppszReturn, psz, (n + 1) * sizeof(WCHAR));
    return 0;
}
struct Pin { WCHAR* m_pName; WCHAR* Name() { return m_pName; } int QueryId(WCHAR** Id) { return AMGetWideString(Name(), Id); } };
int main()
{
    WCHAR name[4] = { 'O', 'u', 't', 0 };
    Pin p; p.m_pName = name;
    WCHAR* id = 0;
    if (p.QueryId(&id) != 0 || id == name || id[0] != 'O' || id[2] != 't' || id[3] != 0) { std::printf("00ca84b0_TEST FAIL\n"); return 1; }
    std::free(id);
    if (p.QueryId(0) != (int)0x80004003) { std::printf("00ca84b0_TEST FAIL\n"); return 1; }
    std::printf("00ca84b0_TEST PASS\n");
    return 0;
}