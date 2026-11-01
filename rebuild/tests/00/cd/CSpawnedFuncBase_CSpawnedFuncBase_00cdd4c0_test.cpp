// Test for 0x00CDD4C0 CSpawnedFuncBase::~CSpawnedFuncBase (implicit): PPredicate released, AssociatedRegionName, Name, then
// the CSuspendableProcessMicrothread base -- reverse declaration order.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; }
class CCharString { public: char Tag; ~CCharString() { Log(Tag); } };
template <class T> class CCountedPointer { public: T* Data; void* Info; ~CCountedPointer() { Log('P'); } };
class CPredicateBase;
class CSuspendableProcessMicrothread { public: virtual ~CSuspendableProcessMicrothread() { Log('M'); } void* PMicrothread; };
class CSpawnedFuncBase : public CSuspendableProcessMicrothread { public:
    unsigned long ID; long LastScriptFrame; long CurrentScriptFrame;
    CCharString Name; CCharString AssociatedRegionName; bool FuncPaused; bool Interrupted;
    CCountedPointer<const CPredicateBase> PPredicate;
};
int main()
{
    CSpawnedFuncBase* f = new CSpawnedFuncBase; f->Name.Tag = 'N'; f->AssociatedRegionName.Tag = 'R';
    CSuspendableProcessMicrothread* b = f; delete b;
    g_log[g_n] = 0;
    const char* want = "PRNM";
    for (int i = 0; i < 5; ++i) if (g_log[i] != want[i]) { std::printf("00cdd4c0_TEST FAIL\n"); return 1; }
    std::printf("00cdd4c0_TEST PASS\n");
    return 0;
}