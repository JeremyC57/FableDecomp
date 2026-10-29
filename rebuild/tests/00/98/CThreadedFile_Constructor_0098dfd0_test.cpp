// Behaviour test for 0x0098DFD0 CThreadedFile::CThreadedFile.
#include <cstdio>
static int g_wideStrings = 0;
class CBaseClassNonCopyable {
public:  CBaseClassNonCopyable() {} virtual ~CBaseClassNonCopyable() {} };
class CWideString { public: CWideString() : m_pData(0) { ++g_wideStrings; } ~CWideString() { --g_wideStrings; } unsigned short* m_pData; };
class CThreadedFile : public CBaseClassNonCopyable {
public:
    CThreadedFile() : OpenedForWrite(false), OpenFlag(false) { FileHandle = (void*)-1; }
    long DeviceID; void* FileHandle; CWideString Filename; unsigned long Length; unsigned long PhysicalSortKey;
    bool OpenedForWrite; bool OpenFlag;
};
int main()
{
    {
        CThreadedFile f;
        if (f.FileHandle != (void*)-1 || f.OpenedForWrite || f.OpenFlag || f.Filename.m_pData != 0 || g_wideStrings != 1) {
            std::printf("0098dfd0_TEST FAIL\n"); return 1;
        }
    }
    if (g_wideStrings != 0) { std::printf("0098dfd0_TEST FAIL\n"); return 1; }
    std::printf("0098dfd0_TEST PASS\n");
    return 0;
}
