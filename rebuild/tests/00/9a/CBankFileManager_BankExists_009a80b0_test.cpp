// Behaviour test for 0x009A80B0 CBankFileManager::BankExists: the name is resolved through the alias map; in retail-bank
// mode any open retail bank whose entry index holds it answers true, otherwise the registered-bank map decides.
#include <cstdio>
#include <cstring>
#include <cstdlib>
// Minimal value models: strings as short owned buffers; sorted-array "map"/"set" with find/insert semantics.
class CWideString { public:
    wchar_t Text[48];
    CWideString() { Text[0] = 0; }
    CWideString(const wchar_t* t) { int i = 0; while (t[i] && i < 47) { Text[i] = t[i]; ++i; } Text[i] = 0; }
    long GetLength() const { long n = 0; while (Text[n]) ++n; return n; }
    bool operator<(const CWideString& o) const { int i = 0; while (Text[i] && Text[i] == o.Text[i]) ++i; return Text[i] < o.Text[i]; }
    bool operator==(const CWideString& o) const { return !(*this < o) && !(o < *this); }
};
static CWideString operator+(const CWideString& a, const CWideString& b)
{ CWideString r(a); long n = r.GetLength(); int i = 0; while (b.Text[i] && n < 47) r.Text[n++] = b.Text[i++]; r.Text[n] = 0; return r; }
class CCharString { public:
    char Text[32];
    CCharString() { Text[0] = 0; }
    CCharString(const char* t) { std::strncpy(Text, t, 31); Text[31] = 0; }
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
};
template <class K, class V, int N = 16> struct SortedMap {
    K keys[N]; V vals[N]; int n;
    SortedMap() : n(0) {}
    int find(const K& k) const { for (int i = 0; i < n; ++i) if (!(keys[i] < k) && !(k < keys[i])) return i; return -1; }
    bool insert(const K& k, const V& v)
    {
        if (find(k) >= 0) return false;
        int i = n; while (i > 0 && k < keys[i - 1]) { keys[i] = keys[i - 1]; vals[i] = vals[i - 1]; --i; }
        keys[i] = k; vals[i] = v; ++n; return true;
    }
};
struct Empty {};
template <class K, int N = 16> struct SortedSet : SortedMap<K, Empty, N> {
    bool insert(const K& k) { return SortedMap<K, Empty, N>::insert(k, Empty()); }
};

struct CRetailBank { SortedMap<CCharString, int> EntryIndex; };
struct CBankFileManager {
    SortedMap<CCharString, int> Banks;
    CRetailBank* RetailBanks[4]; int nRetail;
    bool UsingRetailBanks;
    SortedMap<CCharString, CCharString> BankAliasMap;
    CCharString ResolveAlias(const CCharString& name) const
    { int a = BankAliasMap.find(name); if (a >= 0) return BankAliasMap.vals[a]; return name; }
    bool BankExists(const CCharString& bank) const
    {
        CCharString key = ResolveAlias(bank);
        if (UsingRetailBanks) {
            for (int i = 0; i < nRetail; ++i)
                if (RetailBanks[i]->EntryIndex.find(key) >= 0) return true;
        }
        else if (Banks.find(key) >= 0) return true;
        return false;
    }
};
int main()
{
    CBankFileManager m; m.nRetail = 0; m.UsingRetailBanks = false;
    m.Banks.insert(CCharString("text"), 1); m.Banks.insert(CCharString("sound"), 2);
    m.BankAliasMap.insert(CCharString("dialogue"), CCharString("sound"));
    m.BankAliasMap.insert(CCharString("ghost"), CCharString("missing"));
    if (!m.BankExists(CCharString("text")) || !m.BankExists(CCharString("dialogue")) || m.BankExists(CCharString("ghost")) || m.BankExists(CCharString("fx")))
        { std::printf("009a80b0_TEST FAIL\n"); return 1; }
    CRetailBank r1, r2; r1.EntryIndex.insert(CCharString("fx"), 0); r2.EntryIndex.insert(CCharString("missing"), 0);
    m.RetailBanks[0] = &r1; m.RetailBanks[1] = &r2; m.nRetail = 2; m.UsingRetailBanks = true;
    if (m.BankExists(CCharString("text")) || !m.BankExists(CCharString("fx")) || !m.BankExists(CCharString("ghost")) || m.BankExists(CCharString("dialogue")))
        { std::printf("009a80b0_TEST FAIL\n"); return 1; }
    m.nRetail = 0;
    if (m.BankExists(CCharString("fx"))) { std::printf("009a80b0_TEST FAIL\n"); return 1; }
    std::printf("009a80b0_TEST PASS\n");
    return 0;
}