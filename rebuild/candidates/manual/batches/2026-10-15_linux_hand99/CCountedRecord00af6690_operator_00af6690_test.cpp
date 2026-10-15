#include <cstdio>
struct SCountBlock;
static int g_destroyed = 0;
static void Destroy(void* object) { ++g_destroyed; *(int*)object = -1; }
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCountedPointer {
    int* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        int* object = other.m_pObject;
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};
static SCountBlock* NewBlock(int* object) { SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = Destroy; b->m_pObject = object; return b; }
struct CCountedRecord_00af6690 {
    long            m_head;
    CCountedPointer m_pObject;
    unsigned char   m_flag;
    long            m_value;
    CCountedRecord_00af6690& operator=(const CCountedRecord_00af6690& other)
    {
        m_pObject = other.m_pObject;
        m_flag = other.m_flag;
        m_value = other.m_value;
        return *this;
    }
};
int main()
{
    int objA = 1, objB = 2;
    CCountedRecord_00af6690 a, b;
    a.m_head = 11; a.m_pObject.m_pObject = &objA; a.m_pObject.m_pCount = NewBlock(&objA); a.m_flag = 0; a.m_value = 0;
    a.m_pObject.m_pCount->m_references = 2;             // shared: released but not destroyed
    b.m_head = 22; b.m_pObject.m_pObject = &objB; b.m_pObject.m_pCount = NewBlock(&objB); b.m_flag = 1; b.m_value = -9;
    SCountBlock* blockA = a.m_pObject.m_pCount;
    CCountedRecord_00af6690& r = (a = b);
    if (&r != &a || g_destroyed != 0 || blockA->m_references != 1 || a.m_pObject.m_pObject != &objB ||
        b.m_pObject.m_pCount->m_references != 2 || a.m_flag != 1 || a.m_value != -9 || a.m_head != 11) { std::printf("00af6690_TEST FAIL\n"); return 1; }
    delete blockA;
    std::printf("00af6690_TEST PASS\n");
    return 0;
}
