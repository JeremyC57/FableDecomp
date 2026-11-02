// Behaviour test for 0x009EEA50 std::pair<CCharString, CCountedPointer<CConsoleInputBase> >::~pair: the implicit destructor releases
// `second` (count decrement; at zero the deleter runs on the data and the block is deleted; Data and Info nulled), then destroys `first`.
#include <cstdio>
static int g_deleterCalls = 0, g_nameDtors = 0, g_order = 0, g_nameOrder = 0, g_ptrOrder = 0;
static void* g_deletedData = 0;
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
static void Deleter(void* data) { ++g_deleterCalls; g_deletedData = data; }
class CCharString { public:
    const char* Text;
    CCharString() : Text(0) { }
    ~CCharString() { ++g_nameDtors; g_nameOrder = ++g_order; }
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        g_ptrOrder = ++g_order;
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
template <class T1, class T2> struct pair { T1 first; T2 second; };
class CConsoleInputBase { public: int Value; };
int main()
{
    CConsoleInputBase input;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 2; info->DeleteFunc = Deleter; info->Data = &input;
    {
        pair<CCharString, CCountedPointer<CConsoleInputBase> > entry;
        entry.second.Data = &input; entry.second.Info = info;
    }
    if (info->RefCount != 1 || g_deleterCalls != 0 || g_nameDtors != 1 || g_ptrOrder >= g_nameOrder) { std::printf("009eea50_TEST FAIL\n"); return 1; }
    {
        pair<CCharString, CCountedPointer<CConsoleInputBase> > last;                 // last reference: deleter runs, block deleted
        last.second.Data = &input; last.second.Info = info;
    }
    if (g_deleterCalls != 1 || g_deletedData != &input || g_nameDtors != 2 || g_ptrOrder >= g_nameOrder) { std::printf("009eea50_TEST FAIL\n"); return 1; }
    {
        pair<CCharString, CCountedPointer<CConsoleInputBase> > empty;               // nothing to release
    }
    if (g_deleterCalls != 1 || g_nameDtors != 3) { std::printf("009eea50_TEST FAIL\n"); return 1; }
    std::printf("009eea50_TEST PASS\n");
    return 0;
}