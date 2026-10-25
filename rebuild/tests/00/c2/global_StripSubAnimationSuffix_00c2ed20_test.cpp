// Behaviour test for 00c2ed20: StripSubAnimationSuffix -- a name longer than five characters loses its last five
#include <cstdio>
#include <cstring>
class CString {
public:
    char* m_pchData;
    static int SafeStrlen(const char* p) { return p ? (int)std::strlen(p) : 0; }
    void SetAt(int nIndex, char ch) { m_pchData[nIndex] = ch; }
};
static void StripSubAnimationSuffix(CString& name)
{
    if (CString::SafeStrlen(name.m_pchData) > 5)
        name.SetAt(CString::SafeStrlen(name.m_pchData) - 5, 0);
}
int main()
{
    char a[] = "WALK_SUB1"; CString s; s.m_pchData = a;
    StripSubAnimationSuffix(s);
    if (std::strcmp(a, "WALK") != 0) { std::printf("00c2ed20_TEST FAIL\n"); return 1; }
    char b[] = "_SUB1"; s.m_pchData = b;
    StripSubAnimationSuffix(s);
    if (std::strcmp(b, "_SUB1") != 0) { std::printf("00c2ed20_TEST FAIL\n"); return 1; }
    std::printf("00c2ed20_TEST PASS\n");
    return 0;
}