// Behaviour test for 00c2ecd0: IsSubAnimationName -- names longer than five characters whose last five start with "_SUB"
#include <cstdio>
#include <cstring>
class CString {
public:
    char* m_pchData;
    operator const char*() const { return m_pchData; }
};
static int IsSubAnimationName(const CString& name)
{
    if (std::strlen(name) > 5 && std::strncmp("_SUB", (const char*)name + std::strlen(name) - 5, 4) == 0)
        return 1;
    return 0;
}
static int Check(const char* text) { CString s; s.m_pchData = (char*)text; return IsSubAnimationName(s); }
int main()
{
    if (Check("WALK_SUB1") != 1 || Check("A_SUB2") != 1) { std::printf("00c2ecd0_TEST FAIL\n"); return 1; }
    if (Check("_SUB1") != 0 || Check("WALK_SUB") != 0 || Check("WALKING") != 0 || Check("") != 0) { std::printf("00c2ecd0_TEST FAIL\n"); return 1; }
    std::printf("00c2ecd0_TEST PASS\n");
    return 0;
}