#include <cstdio>

// Model of 0x00404440 CFileInstallerSingleton::Get: create the CFileInstaller on first use into the static counted
// pointer (Reset 0x00403DB0 through an inline operator=) and return the object; later calls reuse it.
static int g_constructed = 0;
class CFileInstaller { public:
    unsigned char m_body[0xA0];
    CFileInstaller() { ++g_constructed; m_body[0] = 0x5A; }
};
struct CCountedFileInstallerPointer {
    CFileInstaller* m_pObject;
    int m_resets;
    void Reset(CFileInstaller* object) { m_pObject = object; ++m_resets; }
    CCountedFileInstallerPointer& operator=(CFileInstaller* object) { Reset(object); return *this; }
};
static CCountedFileInstallerPointer g_FileInstallerSingleton = { 0, 0 };
struct CFileInstallerSingleton {
    static CFileInstaller* Get()
    {
        if (g_FileInstallerSingleton.m_pObject == 0)
            g_FileInstallerSingleton = new CFileInstaller;
        return g_FileInstallerSingleton.m_pObject;
    }
};

int main()
{
    CFileInstaller* a = CFileInstallerSingleton::Get();
    CFileInstaller* b = CFileInstallerSingleton::Get();
    if (a == 0 || a != b || g_constructed != 1 || g_FileInstallerSingleton.m_resets != 1 || a->m_body[0] != 0x5A) {
        std::printf("00404440_TEST FAIL\n");
        return 1;
    }
    delete a;
    std::printf("00404440_TEST PASS\n");
    return 0;
}