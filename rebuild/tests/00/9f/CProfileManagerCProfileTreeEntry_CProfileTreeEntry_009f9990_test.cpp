// Behaviour test for 0x009F9990 CProfileManager::CProfileTreeEntry::CProfileTreeEntry().
#include <cstdio>
#include <cstring>
#include <new>

class CProfileManager;
class CWideString {
public:
    const wchar_t* PData;
    CWideString() : PData(L"") {}
};
class CProfile {
public:
    CProfileManager* Manager;
    double           StartTime;
    CWideString      Name;
    long             TreeDepth;
    CProfile() : Manager(0) {}
};
class CEntryVector {                    // stands in for vector<CProfileTreeEntry*>: three null pointers
public:
    void* Start;
    void* Finish;
    void* EndOfStorage;
    CEntryVector() : Start(0), Finish(0), EndOfStorage(0) {}
};
class CProfileTreeEntry {
public:
    CProfile           Profile;
    long               ID;
    CEntryVector       Children;
    CProfileTreeEntry* Parent;
    CProfileTreeEntry() {}
};

int main()
{
    static double storage[(sizeof(CProfileTreeEntry) + 7) / 8 + 1];
    unsigned char* bytes = (unsigned char*)storage;
    for (unsigned int i = 0; i < sizeof(storage); ++i) bytes[i] = 0xCD;
    CProfileTreeEntry* e = new (storage) CProfileTreeEntry;
    if (!(e->Profile.Manager == 0 && e->Profile.Name.PData[0] == 0 && e->Children.Start == 0 && e->Children.Finish == 0 && e->Children.EndOfStorage == 0)) {
        std::printf("009f9990_TEST FAIL\n");
        return 1;
    }
    std::printf("009f9990_TEST PASS\n");
    return 0;
}