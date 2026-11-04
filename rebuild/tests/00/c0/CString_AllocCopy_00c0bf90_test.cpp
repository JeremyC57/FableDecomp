// Behaviour test for CString::AllocCopy 0x00C0BF90 (MFC strcore.cpp): a zero total length gives dest the shared nil characters;
// otherwise dest gets a fresh buffer of nCopyLen + nExtraLen holding nCopyLen characters from nCopyIndex.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_nil[1] = { 0 };
static char* _afxPchNil = g_nil;
struct CString {
    char* m_pchData; int Alloc;
    CString() : m_pchData(_afxPchNil), Alloc(0) {}
    void Init() { m_pchData = _afxPchNil; }
    void AllocBuffer(int n) { m_pchData = (char*)std::calloc(n + 1, 1); Alloc = n; }
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        int nNewLen = nCopyLen + nExtraLen;
        if (nNewLen == 0) dest.Init();
        else { dest.AllocBuffer(nNewLen); std::memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(char)); }
    }
};
int main()
{
    CString s; s.m_pchData = (char*)"abcdefgh";
    CString d; s.AllocCopy(d, 3, 2, 0);
    if (d.Alloc != 3 || std::strncmp(d.m_pchData, "cde", 3) != 0) { std::printf("00c0bf90_TEST FAIL\n"); return 1; }
    CString e; s.AllocCopy(e, 2, 0, 4);
    if (e.Alloc != 6 || e.m_pchData[0] != 'a' || e.m_pchData[1] != 'b') { std::printf("00c0bf90_TEST FAIL\n"); return 1; }
    CString z; z.m_pchData = 0; s.AllocCopy(z, 0, 5, 0);
    if (z.m_pchData != _afxPchNil) { std::printf("00c0bf90_TEST FAIL\n"); return 1; }
    std::free(d.m_pchData); std::free(e.m_pchData);
    std::printf("00c0bf90_TEST PASS\n");
    return 0;
}