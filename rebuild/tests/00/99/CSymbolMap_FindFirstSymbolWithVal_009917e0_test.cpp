#include <cstdio>
static int Lower(int c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static int StrICmp(const char* a, const char* b)
{
    while (*a && Lower(*a) == Lower(*b)) { ++a; ++b; }
    return Lower((unsigned char)*a) - Lower((unsigned char)*b);
}
static const char g_EmptyCharString[] = "";
class CCharStringData { public: const char* m_pChars; unsigned long m_length; };
class CCharString { public:
    CCharStringData* m_pData;
    const char* GetChars() const { return m_pData == 0 ? g_EmptyCharString : m_pData->m_pChars; }
};
struct SSymbolNameRecord { long m_flags; long m_symbol; long _pad[3]; CCharString m_name; long _pad2[3]; };
class CSymbolMap { public:
    SSymbolNameRecord* m_pFirst; SSymbolNameRecord* m_pLast;
    long FindFirstSymbolWithVal(const CCharString& name) const
    {
        for (const SSymbolNameRecord* it = m_pFirst; it != m_pLast; ++it) {
            if (StrICmp(it->m_name.GetChars(), name.GetChars()) == 0)
                return it->m_symbol;
        }
        return 0;
    }
};
int main()
{
    CCharStringData a = { "Alpha", 5 }, b = { "BETA", 4 }, k = { "beta", 4 };
    SSymbolNameRecord r[3];
    r[0].m_name.m_pData = &a; r[0].m_symbol = 1;
    r[1].m_name.m_pData = 0;  r[1].m_symbol = 2;
    r[2].m_name.m_pData = &b; r[2].m_symbol = 3;
    CSymbolMap m; m.m_pFirst = r; m.m_pLast = r + 3;
    CCharString key; key.m_pData = &k;
    CCharString none; none.m_pData = 0;
    CCharStringData z = { "zeta", 4 }; CCharString miss; miss.m_pData = &z;
    if (m.FindFirstSymbolWithVal(key) != 3 || m.FindFirstSymbolWithVal(none) != 2 || m.FindFirstSymbolWithVal(miss) != 0) { std::printf("009917e0_TEST FAIL\n"); return 1; }
    std::printf("009917e0_TEST PASS\n");
    return 0;
}