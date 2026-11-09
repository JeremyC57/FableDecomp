// Behaviour test for CString::Left (0x00C0E1F0): a negative count is clamped to 0, a count at or past the length returns
// a copy of the whole string, else the first nCount characters (AllocCopy into an empty string).
#include <cstdio>
#include <cstring>
#include <string>

struct CStringData { int nDataLength; };
class CString {
public:
    std::string s;
    CStringData d;
    CString() { d.nDataLength = 0; }
    CString(const char* t) : s(t) { d.nDataLength = (int)s.size(); }
    const CStringData* GetData() const { return &d; }
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        int nNewLen = nCopyLen + nExtraLen;
        if (nNewLen == 0) return;
        dest.s = s.substr(nCopyIndex, nCopyLen); dest.d.nDataLength = nCopyLen;
    }
    CString Left(int nCount) const
    {
        if (nCount < 0) nCount = 0;
        if (nCount >= GetData()->nDataLength) return *this;
        {
            CString dest;
            AllocCopy(dest, nCount, 0, 0);
            return dest;
        }
    }
};

int main()
{
    CString a("abcdef");
    bool ok = a.Left(3).s == "abc" && a.Left(3).GetData()->nDataLength == 3;
    ok = ok && a.Left(6).s == "abcdef" && a.Left(99).s == "abcdef";
    ok = ok && a.Left(0).s.empty() && a.Left(-5).s.empty() && a.Left(-5).GetData()->nDataLength == 0;
    ok = ok && CString("").Left(2).s.empty();
    if (!ok) { std::printf("00c0e1f0_TEST FAIL\n"); return 1; }
    std::printf("01210f76_TEST PASS\n");
    return 0;
}