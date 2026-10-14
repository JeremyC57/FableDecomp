#include <cstdio>
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) { }
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
    static int g_deleted;
    CIVCountedPointeeBase() : IVRefCount(0) { }
};
int CIVCountedPointeeBase::g_deleted = 0;

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
struct CIVCountedPointerD { CIVCountedPointeeBase* Object; CIVCountedPointerD() : Object(0) { }
    ~CIVCountedPointerD() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } };
static int g_ipDestroyed = 0;
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; ~CBaseIntelligentPointer() { ++g_ipDestroyed; } };
struct CEventBase { virtual void v00() { } long m_eventType; ~CEventBase() { } };
struct __declspec(novtable) CEvent : CEventBase { CBaseIntelligentPointer m_target; CIVCountedPointerD m_data; ~CEvent() { } };
int main()
{
    CIVCountedPointeeBase a; a.IVRefCount = 1;
    { CEvent e; e.m_data.Object = &a; }
    if (a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1 || g_ipDestroyed != 1) { std::printf("0084b690_TEST FAIL\n"); return 1; }
    { CEvent e; }   // null data: nothing released
    if (CIVCountedPointeeBase::g_deleted != 1 || g_ipDestroyed != 2) { std::printf("0084b690_TEST FAIL\n"); return 1; }
    std::printf("0084b690_TEST PASS\n"); return 0;
}
