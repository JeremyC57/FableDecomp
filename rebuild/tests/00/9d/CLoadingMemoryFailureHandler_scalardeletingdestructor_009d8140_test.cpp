// Behaviour test for CLoadingMemoryFailureHandler::`scalar deleting destructor' (0x009D8140): the critical section is
// deleted once, the bank list freed, then the object.
#include <cstdio>
#include <stdlib.h>

struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
static int g_deleted = 0; static _RTL_CRITICAL_SECTION* g_section = 0;
static void DeleteCriticalSectionStub(_RTL_CRITICAL_SECTION* section) { ++g_deleted; g_section = section; }   // kernel32 stand-in

class CBankFileAsync;
template <class T> class CArray {
public:
    T* m_pData; T* m_pLast; T* m_pEnd;
    CArray() : m_pData(0), m_pLast(0), m_pEnd(0) {}
    ~CArray() { if (m_pData) free(m_pData); }
};
class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler() {}
};
class CLoadingMemoryFailureHandler : public CFailedAllocationHandler {
public:
    virtual ~CLoadingMemoryFailureHandler();
    CArray<CBankFileAsync*> Banks;
    _RTL_CRITICAL_SECTION   CriticalSection;
};
CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler()
{
    DeleteCriticalSectionStub(&CriticalSection);
}

int main()
{
    CLoadingMemoryFailureHandler* h = new CLoadingMemoryFailureHandler;
    h->Banks.m_pData = (CBankFileAsync**)malloc(4 * sizeof(CBankFileAsync*));
    _RTL_CRITICAL_SECTION* expect = &h->CriticalSection;
    CFailedAllocationHandler* base = h;
    delete base;
    if (g_deleted != 1 || g_section != expect) { std::printf("009d8140_TEST FAIL\n"); return 1; }
    std::printf("009d8140_TEST PASS\n");
    return 0;
}