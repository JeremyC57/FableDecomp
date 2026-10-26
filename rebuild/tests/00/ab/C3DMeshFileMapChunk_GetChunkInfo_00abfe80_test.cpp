#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_live = 0;
class CCharString {
public:
    char* m_pText;
    CCharString(const char* t) { m_pText = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(m_pText, t); ++g_live; }
    CCharString(const CCharString& o) { m_pText = (char*)std::malloc(std::strlen(o.m_pText) + 1); std::strcpy(m_pText, o.m_pText); ++g_live; }
    ~CCharString() { std::free(m_pText); --g_live; }
};
static CCharString Cat(const char* a, const char* b)
{
    char* s = (char*)std::malloc(std::strlen(a) + std::strlen(b) + 1); std::strcpy(s, a); std::strcat(s, b);
    CCharString r(s); std::free(s); return r;
}
CCharString operator+(const char* left, const CCharString& right) { return Cat(left, right.m_pText); }
CCharString operator+(const CCharString& left, const char* right) { return Cat(left.m_pText, right); }
CCharString operator+(const CCharString& left, const CCharString& right) { return Cat(left.m_pText, right.m_pText); }
class C3DMeshFileMapChunk { public:
    long MapIndex; CCharString Name; CCharString Filename;
    C3DMeshFileMapChunk() : MapIndex(0), Name("diffuse"), Filename("a.tga") {}
    CCharString GetName() const { return Name; }
    CCharString GetFilename() const { return Filename; }
    const CCharString GetChunkInfo() const
    {
        CCharString info = "Description: " + GetName() + ", Filename: " + GetFilename();
        return info;
    }
};
int main()
{
    int ok;
    {
        C3DMeshFileMapChunk c;
        CCharString s = c.GetChunkInfo();
        ok = std::strcmp(s.m_pText, "Description: diffuse, Filename: a.tga") == 0 && g_live == 3;
    }
    if (!ok || g_live != 0) { std::printf("00abfe80_TEST FAIL\n"); return 1; }
    std::printf("00abfe80_TEST PASS\n");
    return 0;
}