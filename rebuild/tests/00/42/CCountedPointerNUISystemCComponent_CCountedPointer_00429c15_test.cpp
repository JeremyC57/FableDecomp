// Behaviour test for 0x00429C15 CCountedPointer<NUISystem::CComponent>::CCountedPointer(CComponent*): a non-null component gets a
// count block {1, DeleteData, Data}; a null one leaves Info null; the stored deleter destroys the component.
#include <stdio.h>
namespace NUISystem { class CComponent { public: static int Live; CComponent() { ++Live; } virtual ~CComponent() { --Live; } }; int CComponent::Live = 0; }
struct CCPPointerInfo {
    unsigned long RefCount; void (*DeleteFunc)(void*); void* Data;
    CCPPointerInfo(unsigned long r, void (*f)(void*), void* d) : RefCount(r), DeleteFunc(f), Data(d) { }
};
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    static void DeleteData(void* data) { delete static_cast<T*>(data); }
    CCountedPointer(T* data) : Data(data) { if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)Data); else Info = 0; }
};
int main()
{
    using NUISystem::CComponent;
    CComponent* c = new CComponent;
    CCountedPointer<CComponent> p(c);
    bool ok = p.Data == c && p.Info && p.Info->RefCount == 1 && p.Info->Data == c && CComponent::Live == 1;
    p.Info->DeleteFunc(p.Info->Data);
    ok = ok && CComponent::Live == 0;
    delete p.Info;
    CCountedPointer<CComponent> q(0);
    ok = ok && q.Data == 0 && q.Info == 0;
    printf(ok ? "00429c15_TEST PASS\n" : "00429c15_TEST FAIL\n");
    return ok ? 0 : 1;
}
