// Behaviour test for 0x009E5290 CSymbolMap::GetSymbolsFromGroupName.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

class CCharStringData { public: const char* Chars; unsigned int Length; };
static long Compare(const char* a, const char* b) { int r = std::strcmp(a, b); return r < 0 ? -1 : (r > 0 ? 1 : 0); }
class CCharString { public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
};

struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };

static int gOverflows = 0;
struct CArray {
    CSymbolInfo* _M_start; CSymbolInfo* _M_finish; CSymbolInfo* _M_end_of_storage;
    void _M_insert_overflow(CSymbolInfo* pos, const CSymbolInfo& x, unsigned int fill, bool atend)
    {
        ++gOverflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start), cap = n ? n * 2 : 1;
        CSymbolInfo* p = (CSymbolInfo*)std::malloc(cap * sizeof(CSymbolInfo));
        for (unsigned int i = 0; i < n; ++i) p[i] = _M_start[i];
        p[n] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + 1; _M_end_of_storage = p + cap;
        (void)pos; (void)fill; (void)atend;
    }
    void push_back(const CSymbolInfo& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CSymbolInfo(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

struct SSymbolPair { CCharString first; CSymbolInfo second; };
class CSymbolMap { public:
    int _padding_;
    SSymbolPair* m_pFirst; SSymbolPair* m_pLast; SSymbolPair* m_pEnd;
    bool GetSymbolsFromGroupName(const CCharString& group, CArray& symbols) const
    {
        bool found = false;
        for (const SSymbolPair* it = m_pFirst; it != m_pLast; ++it) {
            if (it->second.GroupName == group) {
                symbols.push_back(it->second);
                found = true;
            }
        }
        return found;
    }
};

int main()
{
    CCharStringData weapons = { "WEAPON", 6 }, weapons2 = { "WEAPON", 6 }, armour = { "ARMOUR", 6 }, magic = { "MAGIC", 5 };
    CCharStringData names[4] = { { "A", 1 }, { "B", 1 }, { "C", 1 }, { "D", 1 } };
    SSymbolPair pairs[4];
    CCharStringData* groups[4] = { &weapons, &armour, &weapons2, &magic };
    for (int i = 0; i < 4; ++i) {
        pairs[i].first.PStringData = &names[i];
        pairs[i].second.GroupName.PStringData = groups[i];
        pairs[i].second.EntryName.PStringData = &names[i];
        pairs[i].second.Val = 10 * i;
    }
    CSymbolMap map = { 0, pairs, pairs + 4, pairs + 4 };
    CArray out = { 0, 0, 0 };
    CCharString weaponKey = { &weapons };
    bool ok = map.GetSymbolsFromGroupName(weaponKey, out);
    ok = ok && out._M_finish - out._M_start == 2 && out._M_start[0].Val == 0 && out._M_start[1].Val == 20 && gOverflows == 2;
    CCharStringData shield = { "SHIELD", 6 };
    CCharString shieldKey = { &shield };
    ok = ok && !map.GetSymbolsFromGroupName(shieldKey, out) && out._M_finish - out._M_start == 2;
    CCharStringData magicCopy = { "MAGIC", 5 };
    CCharString magicKey = { &magicCopy };
    ok = ok && map.GetSymbolsFromGroupName(magicKey, out) && out._M_finish - out._M_start == 3 && out._M_start[2].Val == 30;
    std::free(out._M_start);
    if (!ok) { std::printf("009e5290_TEST FAIL\n"); return 1; }
    std::printf("009e5290_TEST PASS\n");
    return 0;
}