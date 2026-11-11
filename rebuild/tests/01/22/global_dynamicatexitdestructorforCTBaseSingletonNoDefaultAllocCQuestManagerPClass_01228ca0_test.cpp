// Behaviour test for 0x01228CA0, the atexit destructor of CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass: the static
// counted pointer's destructor (engine/CCountedPointer.h body: release the shared count block -- call its deleter on the last
// reference and free the block -- then Data = 0 and Info = 0). Portable model (no __fastcall), the destructor run explicitly.
#include <cstdio>
struct CQuestManager { int id; };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteData(void* p) { ++g_deleted; delete static_cast<CQuestManager*>(p); }
struct CCountedPointer {
    CQuestManager* Data; CCPPointerInfo* Info;
    ~CCountedPointer() {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
int main()
{
    // last reference: the deleter runs once and both words are cleared
    CCountedPointer obj; obj.Data = 0; obj.Info = 0;     // the scope-end destructor finds both words 0
    CCountedPointer* p = &obj;
    CQuestManager* q = new CQuestManager(); p->Data = q;
    p->Info = new CCPPointerInfo(); p->Info->RefCount = 1; p->Info->DeleteFunc = DeleteData; p->Info->Data = q;
    p->~CCountedPointer();
    if (g_deleted != 1 || p->Data != 0 || p->Info != 0) { std::printf("01228ca0_TEST FAIL last\n"); return 1; }
    // shared reference: no delete, count drops, words cleared
    CCPPointerInfo shared; shared.RefCount = 2; shared.DeleteFunc = DeleteData; shared.Data = 0;
    p->Data = 0; p->Info = &shared;
    p->~CCountedPointer();
    if (g_deleted != 1 || shared.RefCount != 1 || p->Info != 0) { std::printf("01228ca0_TEST FAIL shared\n"); return 1; }
    // empty pointer: nothing to release
    p->Data = 0; p->Info = 0; p->~CCountedPointer();
    std::printf("01228ca0_TEST PASS\n");
    return 0;
}
