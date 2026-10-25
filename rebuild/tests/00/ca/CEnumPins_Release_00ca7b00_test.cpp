// Behaviour test for 0x00CA7B00 CEnumPins::Release (DirectShow amfilter.cpp): decrement the reference count and delete the enumerator through its virtual destructor at zero; return the new count.
// Test model: plain C++98 copies of the members the function touches; Win32 / COM callees are recording stand-ins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
typedef int HRESULT;
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
inline int operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003u)
#define E_OUTOFMEMORY ((HRESULT)0x8007000Eu)
#define E_FAIL ((HRESULT)0x80004005u)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define State_Stopped 0
#define State_Paused 1
#define State_Running 2
struct CCritSec {
    int depth; int entered;
    CCritSec() : depth(0), entered(0) {}
    void Lock() { depth++; entered++; }
    void Unlock() { depth--; }
};
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* plock) : m_pLock(plock) { m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct CAMEvent {                                // the test's event: a flag instead of a kernel HANDLE
    int signalled;
    CAMEvent() : signalled(0) {}
    void Set() { signalled = 1; }
    void Reset() { signalled = 0; }
};
static int g_destroyed;
static long InterlockedDecrement(long* p) { return --*p; }
struct CEnumPins {
    int m_Position; int m_PinCount; void* m_pFilter; long m_Version; long m_cRef;
    CEnumPins() : m_Position(0), m_PinCount(0), m_pFilter(0), m_Version(0), m_cRef(1) {}
    virtual ~CEnumPins() { g_destroyed++; }
    unsigned int AddRef() { return (unsigned int)++m_cRef; }
    unsigned int Release();
};
unsigned int CEnumPins::Release()
{
    unsigned int cRef = (unsigned int)InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}
int main()
{
    CEnumPins* e = new CEnumPins;
    int ok = 1;
    ok &= e->AddRef() == 2 && e->AddRef() == 3;
    ok &= e->Release() == 2 && g_destroyed == 0;
    ok &= e->Release() == 1 && g_destroyed == 0;
    ok &= e->Release() == 0 && g_destroyed == 1;
    if (!ok) { std::printf("00ca7b00_TEST FAIL\n"); return 1; }
    std::printf("00ca7b00_TEST PASS\n");
    return 0;
}