// Behaviour test for 0x009C32C0 CFileInstaller::GetPercentComplete.
#include <cstdio>
static int g_locks = 0;
struct Lock { Lock() { ++g_locks; } ~Lock() { --g_locks; } };
struct CActiveFile { int m_bytesProcessed; };
class CFileInstaller { public:
    CActiveFile* PCurrentWriteFile; int NoTotalBytesWritten; int TotalSizeOfAllFiles;
    float GetPercentComplete()
    {
        Lock lock;
        if (TotalSizeOfAllFiles == 0)
            return 1.0f;
        int done = NoTotalBytesWritten;
        if (PCurrentWriteFile)
            done += PCurrentWriteFile->m_bytesProcessed;
        float fraction = (float)done / TotalSizeOfAllFiles;
        return fraction;
    }
};
static int Fail() { std::printf("009c32c0_TEST FAIL\n"); return 1; }
int main()
{
    CActiveFile cur; cur.m_bytesProcessed = 25;
    CFileInstaller f; f.PCurrentWriteFile = 0; f.NoTotalBytesWritten = 50; f.TotalSizeOfAllFiles = 0;
    if (f.GetPercentComplete() != 1.0f) return Fail();
    f.TotalSizeOfAllFiles = 200;
    if (f.GetPercentComplete() != 0.25f) return Fail();
    f.PCurrentWriteFile = &cur;
    if (f.GetPercentComplete() != 0.375f || g_locks != 0) return Fail();
    std::printf("009c32c0_TEST PASS\n");
    return 0;
}