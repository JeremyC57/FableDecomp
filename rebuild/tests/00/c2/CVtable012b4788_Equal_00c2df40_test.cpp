#include <cstdio>

// Behaviour model of CVtable_012b4788::Equal (0x00C2DF40): items ordered by the CString at +8 through MFC's
// inline operators over CString::Compare (a strcmp).
#include <cstring>
class CString {
public:
    const char* m_pchData;
    int Compare(const char* s) const { return std::strcmp(m_pchData, s); }
    operator const char*() const { return m_pchData; }
};
inline bool operator==(const CString& s1, const CString& s2) { return s1.Compare(s2) == 0; }
inline bool operator<(const CString& s1, const CString& s2) { return s1.Compare(s2) < 0; }
struct Item { int _pad[2]; CString m_name; };
struct Cmp { int F(const Item* a, const Item* b) const { return a->m_name == b->m_name; } };

int main()
{

    Cmp c;
    Item x; x.m_name.m_pchData = "alpha"; Item y; y.m_name.m_pchData = "beta"; Item z; z.m_name.m_pchData = "alpha";
    int xy = c.F(&x, &y), yx = c.F(&y, &x), xz = c.F(&x, &z);
    if (xy != 0 || yx != 0 || xz != 1) { std::printf("00c2df40_TEST FAIL\n"); return 1; }
    std::printf("00c2df40_TEST PASS\n");
    return 0;
}