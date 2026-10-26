#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static const void* g_setPackage = 0;
class CInputEventPackage { public: unsigned char _events[0x0c]; long TimeStamp; unsigned char _rest[0x20]; };
class CChecksum { public: long Checksum1, Checksum2, Checksum3, Checksum4; };
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x38];
    CChecksum Checksum;
    CInputEventPackage* m_pLoadedPackagesFirst; CInputEventPackage* m_pLoadedPackagesLast; CInputEventPackage* m_pLoadedPackagesEnd;
    long CurrentFilePackageIndex; long TimeStamp;
    void SetEventsFromEventPackage(CInputEventPackage* package) { std::strcat(g_seq, "S"); g_setPackage = package; }
    unsigned long CalculateInternalChecksum() { std::strcat(g_seq, "C"); return 0x1234; }
    void LoadEventPackage()
    {
        CInputEventPackage* package = &m_pLoadedPackagesFirst[CurrentFilePackageIndex];
        if (package->TimeStamp == TimeStamp) { SetEventsFromEventPackage(package); Checksum.Checksum4 = CalculateInternalChecksum(); ++CurrentFilePackageIndex; }
    }
};
int main()
{
    CInputManager m; CInputEventPackage p[3]; p[0].TimeStamp = 10; p[1].TimeStamp = 20; p[2].TimeStamp = 30; m.m_pLoadedPackagesFirst = p; m.CurrentFilePackageIndex = 1; m.TimeStamp = 20; m.Checksum.Checksum4 = 0;
    m.LoadEventPackage(); if (std::strcmp(g_seq, "SC") || g_setPackage != &p[1] || m.Checksum.Checksum4 != 0x1234 || m.CurrentFilePackageIndex != 2) { std::printf("009f5650_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.LoadEventPackage(); if (std::strcmp(g_seq, "") || m.CurrentFilePackageIndex != 2) { std::printf("009f5650_TEST FAIL\n"); return 1; }
    std::printf("009f5650_TEST PASS\n");
    return 0;
}