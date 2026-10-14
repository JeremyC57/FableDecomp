#include <cstdio>
#include <cstring>
static int g_stamps = 0;
struct CDateAndTime { unsigned char _body[0x1c]; CDateAndTime() { std::memset(_body, 0, sizeof(_body)); ++g_stamps; } };
struct CFileInformation { CDateAndTime m_created; CDateAndTime m_modified; CDateAndTime m_accessed; CFileInformation() : m_created(), m_modified(), m_accessed() { } };
struct CAFile { virtual void v00() {} virtual CFileInformation GetFileInformation(); };
CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
int main()
{
    CAFile f; CFileInformation info = f.GetFileInformation();
    if (g_stamps != 3 || info.m_accessed._body[0x1b] != 0) { std::printf("00412090_TEST FAIL\n"); return 1; }
    std::printf("00412090_TEST PASS\n");
    return 0;
}