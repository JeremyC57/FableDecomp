// Behaviour test for pair<EPSCType,CComponentInstance>::~pair: the component released, then the CBaseClass root (0x00AF65B0).
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
class CBaseClass { public: virtual ~CBaseClass() { g_log[g_n++] = 100; } };
class CPSCBase; class CComponentInstance : public CBaseClass { public: CCountedPointer<CPSCBase> Component; bool Enabled; unsigned long InstanceName; };
enum EPSCType { PSC_NONE = 0 }; template<class A, class B> struct TPair { A first; B second; };
int main()
{
    { TPair<EPSCType, CComponentInstance> s; s.first = PSC_NONE; s.second.Component.Data = 0; s.second.Component.Info = NewInfo(1); }
    bool ok1 = g_n == 2 && g_log[0] == 50 && g_log[1] == 100 && g_dataDeleted == 1;
    g_n = 0;
    { TPair<EPSCType, CComponentInstance> s; s.first = PSC_NONE; s.second.Component.Data = 0; s.second.Component.Info = 0; }
    bool ok2 = g_n == 1 && g_log[0] == 100;
    if (!(ok1 && ok2)) { std::printf("00af65b0_TEST FAIL\n"); return 1; }
    std::printf("00af65b0_TEST PASS\n");
    return 0;
}