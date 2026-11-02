// Behaviour test for 0x00953470 CCreatureAction_DragonTakeOffIntoFlying::~CCreatureAction_DragonTakeOffIntoFlying: the implicit destructor releases PEndAction (count decrement;
// at zero the deleter runs on the data and the block is deleted; both fields nulled), frees the MovementPoints block, then runs ~CCombatActionBase.
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
class CCreatureActionBase { public: int Value; };
class CCombatActionBase { public: virtual ~CCombatActionBase() { ++g_baseDtors; } };
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    float                               Dest[3], StartPos[3], PreviousPoint[3];
    CArray<float>                       MovementPoints;
    bool                                UsePreviousPoint;
    CCountedPointer<CCreatureActionBase> PEndAction;

};
int main()
{
    CCreatureActionBase data;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 2; info->DeleteFunc = Deleter; info->Data = &data;
    {
        CCreatureAction_DragonTakeOffIntoFlying first;
        first.PEndAction.Data = &data; first.PEndAction.Info = info;
        first.MovementPoints.Start = (float*)std::malloc(6 * sizeof(float));
    }
    if (info->RefCount != 1 || g_deleterCalls != 0 || g_frees != 1 || g_baseDtors != 1) { std::printf("00953470_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_DragonTakeOffIntoFlying last;                                                    // last reference: deleter runs, block deleted
        last.PEndAction.Data = &data; last.PEndAction.Info = info;
    }
    if (g_deleterCalls != 1 || g_deletedData != &data || g_frees != 1 || g_baseDtors != 2) { std::printf("00953470_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_DragonTakeOffIntoFlying empty;                                                   // nothing to release or free
    }
    if (g_deleterCalls != 1 || g_frees != 1 || g_baseDtors != 3) { std::printf("00953470_TEST FAIL\n"); return 1; }
    std::printf("00953470_TEST PASS\n");
    return 0;
}