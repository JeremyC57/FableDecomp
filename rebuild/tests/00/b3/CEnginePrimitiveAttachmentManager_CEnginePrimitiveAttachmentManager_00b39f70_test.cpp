// Behaviour test for CEnginePrimitiveAttachmentManager::~CEnginePrimitiveAttachmentManager 0x00B39F70: every attached-primitive record
// is destroyed (through its virtual destructor) before the CEngineComponent base and then the CBaseClassNonCopyable base.
#include <cstdio>
#include <list>
static int g_records = 0, g_log[4], g_n = 0;
struct CBaseClass { virtual ~CBaseClass() { } };
struct CEngineInternalAttachedPrimitiveInfo : CBaseClass {
    CEngineInternalAttachedPrimitiveInfo() { } CEngineInternalAttachedPrimitiveInfo(const CEngineInternalAttachedPrimitiveInfo&) { }
    ~CEngineInternalAttachedPrimitiveInfo() { ++g_records; }
};
struct CBaseClassNonCopyable : CBaseClass { ~CBaseClassNonCopyable() { g_log[g_n++] = 2; } };
struct CEngineComponent { virtual ~CEngineComponent() { g_log[g_n++] = 1; } };
struct CEnginePrimitiveAttachmentManager : CBaseClassNonCopyable, CEngineComponent {
    std::list<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives;
    ~CEnginePrimitiveAttachmentManager() { }
};
int main()
{
    CEnginePrimitiveAttachmentManager* m = new CEnginePrimitiveAttachmentManager;
    for (int i = 0; i < 3; ++i) m->AttachedPrimitives.push_back(CEngineInternalAttachedPrimitiveInfo());
    g_records = 0;
    delete m;
    if (g_records != 3 || g_n != 2 || g_log[0] != 1 || g_log[1] != 2) { std::printf("00b39f70_TEST FAIL\n"); return 1; }
    std::printf("00b39f70_TEST PASS\n");
    return 0;
}