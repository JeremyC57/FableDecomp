// Behaviour test for std::pair<long,C3DMeshVolume>::~pair: convex volume released, then the volume name (0x00A326D0).
#include <cstdio>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_dataDeleted = 0;
static void DeleteData(void* data) { (void)data; ++g_dataDeleted; g_log[g_n++] = 50; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
struct CContainmentVolume; struct C3DMeshVolume { CCharString Name; CCountedPointer<CContainmentVolume> ConvexVolume; };
template<class A, class B> struct TPair { A first; B second; };
int main()
{
    CCPPointerInfo* shared = NewInfo(2);
    { TPair<long, C3DMeshVolume> e; e.first = 5; e.second.Name.id = 2; e.second.ConvexVolume.Data = 0; e.second.ConvexVolume.Info = shared; }
    bool ok1 = g_n == 1 && g_log[0] == 2 && shared->RefCount == 1 && g_dataDeleted == 0;
    g_n = 0;
    { TPair<long, C3DMeshVolume> e; e.first = 5; e.second.Name.id = 3; e.second.ConvexVolume.Data = 0; e.second.ConvexVolume.Info = shared; }
    bool ok2 = g_n == 2 && g_log[0] == 50 && g_log[1] == 3 && g_dataDeleted == 1;
    if (!(ok1 && ok2)) { std::printf("00a326d0_TEST FAIL\n"); return 1; }
    std::printf("00a326d0_TEST PASS\n");
    return 0;
}