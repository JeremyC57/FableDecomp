#include <cstdio>
struct CQuestMgr { void Activate(const void* q, int a, int b); };
// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
class CQuestManager;
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CQuestManager>;
static const void* seen_q; static int seen_a, seen_b; static int seen_this;
void CQuestMgr::Activate(const void* q, int a, int b){ seen_this=(this==(reinterpret_cast<CQuestMgr*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data))); seen_q=q; seen_a=a; seen_b=b; }
void __stdcall CGameScriptInterface_ActivateMultipleQuests(const void* quests);
int main(){
    static CQuestMgr mgr; (reinterpret_cast<CQuestMgr*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data))=&mgr;
    int arr[3]={7,8,9};
    CGameScriptInterface_ActivateMultipleQuests(arr);
    if(seen_this && seen_q==(const void*)arr && seen_a==1 && seen_b==1) printf("OK_00892ea0\n");
    else printf("FAIL\n");
    return 0;
}