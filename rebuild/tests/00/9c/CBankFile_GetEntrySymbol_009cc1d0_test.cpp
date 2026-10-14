#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CCharString {
    const char* m_text; long m_length;
    CCharString(const char* text, long length) : m_text(text), m_length(length) {}
    CCharString(const CCharString& o) : m_text(o.m_text), m_length(o.m_length) {}
};
template <class T> struct CArray {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    struct iterator { T* m_p; iterator(T* p) : m_p(p) {} iterator operator+(unsigned long n) const { return iterator(m_p + n); } T& operator*() const { return *m_p; } };
    bool IsEmpty() const { return m_pFirst == m_pLast; }
    iterator begin() const { return iterator(m_pFirst); }
};
struct CBankFile {
    CArray<CCharString> Symbols;
    CCharString GetEntrySymbol(unsigned long index) const
    {
        if (Symbols.IsEmpty()) return CCharString("(Unknown)", -1);
        return CCharString(*(Symbols.begin() + index));
    }
};
static bool Check()
{
    CCharString syms[2] = { CCharString("zero", 4), CCharString("one", 3) };
    CBankFile b; b.Symbols.m_pFirst = syms; b.Symbols.m_pLast = syms; b.Symbols.m_pEnd = syms + 2;
    CCharString e = b.GetEntrySymbol(1);
    if (std::strcmp(e.m_text, "(Unknown)") != 0 || e.m_length != -1) return false;
    b.Symbols.m_pLast = syms + 2;
    CCharString s = b.GetEntrySymbol(1);
    return std::strcmp(s.m_text, "one") == 0 && s.m_length == 3;
}
int main()
{
    if (!Check()) { std::printf("009cc1d0_TEST FAIL\n"); return 1; }
    std::printf("009cc1d0_TEST PASS\n");
    return 0;
}