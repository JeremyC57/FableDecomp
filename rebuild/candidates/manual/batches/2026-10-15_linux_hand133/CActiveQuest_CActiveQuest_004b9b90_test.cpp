// Behaviour test for CActiveQuest::~CActiveQuest @ 0x004B9B90
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0;
static void DestroyObject(void* object) { g_destroyed += *(int*)object; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
template <class T> static void Share(CCountedPointer<T>& p, T* object, SCountBlock* block) { p.m_pObject = object; p.m_pCount = block; }
static SCountBlock* NewBlock(void* object, long refs) { SCountBlock* b = new SCountBlock; b->m_references = refs; b->m_pfnDestroy = DestroyObject; b->m_pObject = object; return b; }
static int g_strings = 0;
class CCharString { public: void* PStringData; ~CCharString() { ++g_strings; } };
static int g_freed = 0;
template <class T> struct CRetailVector { T* m_start; T* m_finish; T* m_endOfStorage; ~CRetailVector() { if (m_start) { ++g_freed; std::free(m_start); } } };
struct C2DCoordI { long X, Y; };
class CActiveQuest {
public:
    long ID; void* PScriptInfo; CCountedPointer<int> PScript; CRetailVector<const void*> Regions; C2DCoordI MarkerPosition;
    bool Persistent, LoadedResources; CCountedPointer<int> PScriptData; CCharString QuestName;
    ~CActiveQuest() {}
};
int main()
{
    int script = 1, data = 10;
    SCountBlock* shared = NewBlock(&script, 2);      // the script is shared with another owner
    SCountBlock* owned = NewBlock(&data, 1);
    {
        static unsigned char storage[sizeof(CActiveQuest)];
        CActiveQuest* q = new (storage) CActiveQuest;
        Share(q->PScript, &script, shared); Share(q->PScriptData, &data, owned);
        q->Regions.m_start = (const void**)std::malloc(8); q->Regions.m_finish = q->Regions.m_start; q->Regions.m_endOfStorage = q->Regions.m_start + 2;
        q->~CActiveQuest();
        if (q->PScript.m_pObject || q->PScript.m_pCount || q->PScriptData.m_pCount) { std::printf("004b9b90_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 10 || shared->m_references != 1 || g_freed != 1 || g_strings != 1) { std::printf("004b9b90_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("004b9b90_TEST PASS\n");
    return 0;
}
