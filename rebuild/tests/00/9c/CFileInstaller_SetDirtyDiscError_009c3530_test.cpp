#include <cstdio>
#include <cstring>

struct _RTL_CRITICAL_SECTION { int Depth; int Entered; };   // stand-in: counts entries

class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock) : CriticalSection(&section), Locked(true)
    { (void)tryLock; ++section.Depth; ++section.Entered; }
    ~CCriticalSectionLock() { Release(); }
    void Release() { if (Locked) { --CriticalSection->Depth; Locked = false; } }
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};

template <class T> class CTBaseSingleton {
public:
    static T& Get() { ++Calls; return TheInstance; }
    static T TheInstance;
    static int Calls;
};

class CFileInstaller {
public:
    _RTL_CRITICAL_SECTION CriticalSection;
    bool ExitThread;
    static bool DirtyDiscError;
    static void SetDirtyDiscError(bool error)
    {
        CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
        DirtyDiscError = error;
        if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
            error = true;
        CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
    }
};
bool CFileInstaller::DirtyDiscError = false;
template <class T> T CTBaseSingleton<T>::TheInstance;
template <class T> int CTBaseSingleton<T>::Calls = 0;

int main()
{
    CFileInstaller& fi = CTBaseSingleton<CFileInstaller>::TheInstance;
    std::memset(&fi.CriticalSection, 0, sizeof(fi.CriticalSection));
    struct { bool exitBefore, error, exitAfter; } cases[] = {
        { false, false, false }, { false, true, true }, { true, false, true }, { true, true, true },
    };
    for (int i = 0; i < 4; ++i) {
        fi.ExitThread = cases[i].exitBefore;
        CFileInstaller::DirtyDiscError = !cases[i].error;
        CTBaseSingleton<CFileInstaller>::Calls = 0;
        CFileInstaller::SetDirtyDiscError(cases[i].error);
        if (fi.ExitThread != cases[i].exitAfter || CFileInstaller::DirtyDiscError != cases[i].error ||
            CTBaseSingleton<CFileInstaller>::Calls != 3 || fi.CriticalSection.Depth != 0 || fi.CriticalSection.Entered != i + 1) {
            std::printf("009c3530_TEST FAIL\n"); return 1;
        }
    }
    std::printf("009c3530_TEST PASS\n");
    return 0;
}