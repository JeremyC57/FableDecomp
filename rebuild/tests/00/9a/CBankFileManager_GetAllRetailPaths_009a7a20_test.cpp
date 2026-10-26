// Behaviour test for 0x009A7A20 CBankFileManager::GetAllRetailPaths: every registered bank whose RetailBankFilePath is
// non-empty contributes BasePath + that path to the set (duplicates collapse, empty paths are skipped), in bank order.
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

struct CBankInfo { CWideString DevBankFilePath, RetailBankFilePath, DevHeaderFilePath, RetailHeaderFilePath; void* RegisteredBank; };
struct CBankFileManager {
    SortedMap<CCharString, CBankInfo> Banks;
    CWideString BasePath;
    void GetAllRetailPaths(SortedSet<CWideString>& paths)
    {
        for (int i = 0; i < Banks.n; ++i)
            if (Banks.vals[i].RetailBankFilePath.GetLength())
                paths.insert(BasePath + Banks.vals[i].RetailBankFilePath);
    }
};
int main()
{
    CBankFileManager m; m.BasePath = CWideString(L"Data/");
    const char* names[4] = { "text", "graphics", "sound", "frontend" };
    const wchar_t* retail[4] = { L"text.big", L"", L"sound.big", L"text.big" };
    for (int i = 0; i < 4; ++i) { CBankInfo b; b.RetailBankFilePath = CWideString(retail[i]); b.RegisteredBank = 0; m.Banks.insert(CCharString(names[i]), b); }
    SortedSet<CWideString> paths;
    paths.insert(CWideString(L"Data/old.big"));
    m.GetAllRetailPaths(paths);
    // old.big + sound.big + text.big (twice -> once); graphics skipped
    if (paths.n != 3) { std::printf("009a7a20_TEST FAIL\n"); return 1; }
    if (paths.find(CWideString(L"Data/text.big")) < 0 || paths.find(CWideString(L"Data/sound.big")) < 0 || paths.find(CWideString(L"Data/")) >= 0) { std::printf("009a7a20_TEST FAIL\n"); return 1; }
    std::printf("009a7a20_TEST PASS\n");
    return 0;
}