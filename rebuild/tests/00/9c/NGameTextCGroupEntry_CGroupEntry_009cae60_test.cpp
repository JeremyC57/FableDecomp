// Behaviour test for 0x009CAE60 NGameText::CGroupEntry::~CGroupEntry: the implicit destructor releases PFileData (count decrement;
// at zero the deleter runs on the data and the block is deleted; both fields nulled), frees the Entries block, then runs ~CResource.
#include <cstdio>
#include <cstdlib>
static int g_deleterCalls = 0, g_baseDtors = 0, g_frees = 0;
static void* g_deletedData = 0;
static void TrackedFree(void* p) { if (p) { ++g_frees; std::free(p); } }
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
static void Deleter(void* data) { ++g_deleterCalls; g_deletedData = data; }
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
template <class T> class CArray { public:              // the STLport vector's storage: start / finish / end of storage
    T* Start; T* Finish; T* EndOfStorage;
    CArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CArray() { TrackedFree(Start); }
};
class CBankFileAsyncData { public: int Value; };
class CResource { public: virtual ~CResource() { ++g_baseDtors; } };
class CGroupEntry : public CResource { public:
    CArray<long>                        Entries;
    CCountedPointer<CBankFileAsyncData> PFileData;
    const void*                         Owner;
};
int main()
{
    CBankFileAsyncData data;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 2; info->DeleteFunc = Deleter; info->Data = &data;
    {
        CGroupEntry first;
        first.PFileData.Data = &data; first.PFileData.Info = info;
        first.Entries.Start = (long*)std::malloc(4 * sizeof(long));
    }
    if (info->RefCount != 1 || g_deleterCalls != 0 || g_frees != 1 || g_baseDtors != 1) { std::printf("009cae60_TEST FAIL\n"); return 1; }
    {
        CGroupEntry last;                                                    // last reference: deleter runs, block deleted
        last.PFileData.Data = &data; last.PFileData.Info = info;
    }
    if (g_deleterCalls != 1 || g_deletedData != &data || g_frees != 1 || g_baseDtors != 2) { std::printf("009cae60_TEST FAIL\n"); return 1; }
    {
        CGroupEntry empty;                                                   // nothing to release or free
    }
    if (g_deleterCalls != 1 || g_frees != 1 || g_baseDtors != 3) { std::printf("009cae60_TEST FAIL\n"); return 1; }
    std::printf("009cae60_TEST PASS\n");
    return 0;
}