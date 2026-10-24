// Behaviour test for InstallationComplete (0x0042E3CE): 1 with no installer, 1 when the installer reports complete, 0 while
// it is still copying.
#include <cstdio>
class CFileInstaller {
public:
    CFileInstaller() : m_complete(false) {}
    bool IsComplete() const { return m_complete; }
    bool m_complete;
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CFileInstallerSingleton {
public:
    static CFileInstaller& Get() { if (!Instance.Data) Instance.Data = &s_installer; return *Instance.Data; }
    static bool Exists() { return Instance.Data != 0; }
    static CCountedPointer<CFileInstaller> Instance;
    static CFileInstaller s_installer;
};
CCountedPointer<CFileInstaller> CFileInstallerSingleton::Instance = { 0, 0 };
CFileInstaller CFileInstallerSingleton::s_installer;
int InstallationComplete()
{
    if (CFileInstallerSingleton::Exists() && !CFileInstallerSingleton::Get().IsComplete())
        return 0;
    return 1;
}
int main()
{
    int bad = 0;
    if (InstallationComplete() != 1) bad = 1;
    if (CFileInstallerSingleton::Exists()) bad = 1;          // the test must not create the installer
    CFileInstallerSingleton::Get();
    if (InstallationComplete() != 0) bad = 1;
    CFileInstallerSingleton::s_installer.m_complete = true;
    if (InstallationComplete() != 1) bad = 1;
    if (bad) { std::printf("0042e3ce_TEST FAIL\n"); return 1; }
    std::printf("0042e3ce_TEST PASS\n");
    return 0;
}