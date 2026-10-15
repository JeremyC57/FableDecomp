// Behaviour test for CSoundBankRecord::~CSoundBankRecord @ 0x00992510
#include <cstdio>
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
class CSoundBankRecord {
public:
    CCountedPointer<int> m_pBank; CCountedPointer<int> m_pData;
    CCharString m_name, m_fileName, m_dataFileName, m_description;
    ~CSoundBankRecord() {}
};
int main()
{
    int bank = 3, data = 40;
    static unsigned char storage[sizeof(CSoundBankRecord)];
    CSoundBankRecord* r = new (storage) CSoundBankRecord;
    Share(r->m_pBank, &bank, NewBlock(&bank, 1)); Share(r->m_pData, &data, NewBlock(&data, 1));
    r->~CSoundBankRecord();
    if (g_destroyed != 43 || g_strings != 4 || r->m_pBank.m_pCount || r->m_pData.m_pObject) { std::printf("00992510_TEST FAIL\n"); return 1; }
    std::printf("00992510_TEST PASS\n");
    return 0;
}
