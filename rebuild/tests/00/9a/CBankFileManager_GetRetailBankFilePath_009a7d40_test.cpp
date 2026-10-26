// Behaviour test for 0x009A7D40 CBankFileManager::GetRetailBankFilePath: alias-resolved bank -> BasePath + its retail bank
// path; the first request for a path creates the retail bank file and records it, later requests do not; an unknown bank
// throws CGenericException.
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

struct CGenericException { CGenericException(const char*) {} };
struct CBankInfo { CWideString DevBankFilePath, RetailBankFilePath, DevHeaderFilePath, RetailHeaderFilePath; void* RegisteredBank; };
static int g_created = 0;
struct CBankFileManager {
    SortedMap<CCharString, CBankInfo> Banks;
    SortedMap<CCharString, CCharString> BankAliasMap;
    CWideString BasePath;
    SortedSet<CWideString> CreatedRetailBanks;
    CCharString ResolveAlias(const CCharString& name) { int a = BankAliasMap.find(name); if (a >= 0) return BankAliasMap.vals[a]; return name; }
    void CreateRetailBank(const CWideString&) { ++g_created; }
    CWideString GetRetailBankFilePath(const CCharString& bank)
    {
        CCharString key = ResolveAlias(bank);
        int info = Banks.find(key);
        if (info >= 0) {
            CWideString path = BasePath + Banks.vals[info].RetailBankFilePath;
            if (CreatedRetailBanks.find(path) < 0) { CreateRetailBank(path); CreatedRetailBanks.insert(path); }
            return path;
        }
        throw CGenericException("bank not found!");
    }
};
int main()
{
    CBankFileManager m; m.BasePath = CWideString(L"Data/");
    CBankInfo b; b.RetailBankFilePath = CWideString(L"text.big"); m.Banks.insert(CCharString("text"), b);
    m.BankAliasMap.insert(CCharString("strings"), CCharString("text"));
    CWideString p1 = m.GetRetailBankFilePath(CCharString("text"));
    CWideString p2 = m.GetRetailBankFilePath(CCharString("strings"));
    if (!(p1 == CWideString(L"Data/text.big")) || !(p2 == p1) || g_created != 1 || m.CreatedRetailBanks.n != 1) { std::printf("009a7d40_TEST FAIL\n"); return 1; }
    bool threw = false;
    try { m.GetRetailBankFilePath(CCharString("nope")); } catch (const CGenericException&) { threw = true; }
    if (!threw || g_created != 1) { std::printf("009a7d40_TEST FAIL\n"); return 1; }
    std::printf("009a7d40_TEST PASS\n");
    return 0;
}