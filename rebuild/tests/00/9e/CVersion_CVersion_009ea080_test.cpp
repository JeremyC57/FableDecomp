#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
struct CVersion {
    unsigned char m_major; unsigned char m_minor; long m_build; long m_revision;
    CVersion(unsigned char minor, unsigned char major, long build, long revision)
        : m_major(major), m_minor(minor), m_build(build), m_revision(revision) { }
};
int main()
{
    CVersion v(3, 7, 1234, -5);
    if (v.m_major != 7 || v.m_minor != 3 || v.m_build != 1234 || v.m_revision != -5) { std::printf("009ea080_TEST FAIL\n"); return 1; }
    std::printf("009ea080_TEST PASS\n"); return 0;
}