// Behaviour test for CBankFileAsync::SetLoadingMemoryFailureHandler (0x009D66A0): old handler detached,
// new handler attached, counted references balanced (the old block destroyed when it hits zero).
#include <cstdio>

static int g_destroyed = 0;
struct SCountBlock { long m_references; void* m_pObject; };
struct CHandler { int banks; void RemoveBank() { --banks; } void AddBank() { ++banks; } };
struct CPtr {
    CHandler* m_pObject; SCountBlock* m_pCount;
    CPtr(CHandler* o, SCountBlock* b) : m_pObject(o), m_pCount(b) { if (b) ++b->m_references; }
    CPtr(const CPtr& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CPtr() { if (m_pCount) { if (--m_pCount->m_references == 0) ++g_destroyed; } }
    CPtr& operator=(const CPtr& other)
    {
        CHandler* object = other.m_pObject; SCountBlock* block = other.m_pCount; SCountBlock* current = m_pCount;
        if (current != block) {
            if (current) { --current->m_references; current = m_pCount; if (current->m_references == 0) ++g_destroyed; }
            m_pObject = object; m_pCount = block;
            if (block) ++block->m_references;
        }
        return *this;
    }
};
class CBankFileAsync { public:
    CPtr MemoryFailureHandler;
    CBankFileAsync() : MemoryFailureHandler(0, 0) {}
    void SetLoadingMemoryFailureHandler(CPtr handler)
    {
        if (MemoryFailureHandler.m_pObject) MemoryFailureHandler.m_pObject->RemoveBank();
        if (handler.m_pObject) handler.m_pObject->AddBank();
        MemoryFailureHandler = handler;
    }
};

int main()
{
    CHandler h1 = { 0 }, h2 = { 0 };
    SCountBlock b1 = { 0, &h1 }, b2 = { 0, &h2 };
    CBankFileAsync bank;
    { CPtr p(&h1, &b1); bank.SetLoadingMemoryFailureHandler(p); }
    if (h1.banks != 1 || b1.m_references != 1 || g_destroyed != 0) { std::printf("009d66a0_TEST FAIL\n"); return 1; }
    { CPtr p(&h2, &b2); bank.SetLoadingMemoryFailureHandler(p); }
    if (h1.banks != 0 || h2.banks != 1 || b1.m_references != 0 || g_destroyed != 1 || b2.m_references != 1) { std::printf("009d66a0_TEST FAIL\n"); return 1; }
    std::printf("009d66a0_TEST PASS\n");
    return 0;
}