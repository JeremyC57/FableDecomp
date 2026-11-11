#include <cstdio>
struct CCharStringArray { int n; };

static CCharStringArray const* g_lastQuests;
static int g_lastA, g_lastB, g_called;

struct CQuestMgr {
    void Activate(CCharStringArray const& quests, int a, int b);
};
void CQuestMgr::Activate(CCharStringArray const& quests, int a, int b) {
    g_lastQuests = &quests; g_lastA = a; g_lastB = b; g_called = 1;
}
CQuestMgr realMgr;
// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
class CQuestManager;
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CQuestManager>;
static const bool TestPClassInit = ((reinterpret_cast<CQuestMgr*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data)) = &realMgr, true);

void __stdcall CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(CCharStringArray const& quests);

int main() {
    CCharStringArray arr; arr.n = 7;
    CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(arr);
    if (g_called == 1 && g_lastQuests == &arr && g_lastA == 1 && g_lastB == 0) {
        printf("OK_00892ee0\n");
    } else {
        printf("FAIL called=%d a=%d b=%d\n", g_called, g_lastA, g_lastB);
    }
    return 0;
}