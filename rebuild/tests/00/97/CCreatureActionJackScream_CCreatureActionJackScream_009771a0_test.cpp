// Behaviour test for 0x009771A0 CCreatureAction_JackScream::~CCreatureAction_JackScream: the implicit destructor releases the counted pointer PModeCountedPointer
// (count decrement; at zero the block's deleter runs on the data and the block is deleted; both fields nulled) and then
// runs the combat-action base destructor chain.
#include <cstdio>
static int g_deleterCalls = 0, g_baseDtors = 0;
static void* g_deletedData = 0;
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
class CPointee { public: int Value; };
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { ++g_baseDtors; } };
class CCombatActionBase : public CCreatureActionBase { public:
    long Members[4];
};
class CCreatureAction_JackScream : public CCombatActionBase { public:
    CCountedPointer<CPointee> PModeCountedPointer;
};
int main()
{
    CPointee pointee;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 2; info->DeleteFunc = Deleter; info->Data = &pointee;
    {
        CCreatureAction_JackScream first;
        first.PModeCountedPointer.Data = &pointee; first.PModeCountedPointer.Info = info;
    }
    if (info->RefCount != 1 || g_deleterCalls != 0 || g_baseDtors != 1) { std::printf("009771a0_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_JackScream last;
        last.PModeCountedPointer.Data = &pointee; last.PModeCountedPointer.Info = info;   // last reference: deleter runs, block deleted
    }
    if (g_deleterCalls != 1 || g_deletedData != &pointee || g_baseDtors != 2) { std::printf("009771a0_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_JackScream empty;                                      // null pointer: nothing released
    }
    if (g_deleterCalls != 1 || g_baseDtors != 3) { std::printf("009771a0_TEST FAIL\n"); return 1; }
    std::printf("009771a0_TEST PASS\n");
    return 0;
}