// Behaviour test for 0054e350: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

namespace NUISystem { class CComponent; }
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};
typedef CCountedPointer<NUISystem::CComponent> CComponentPointer;
struct CComponentPointerVector {
    CComponentPointer* _M_start;
    CComponentPointer* _M_finish;
    CComponentPointer* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CComponentPointer& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NUISystem {
class CComponent {
public:
    CComponent* AcceptButton;
    int SetCalls;
    CComponentPointer Storage[4];
    CComponentPointerVector Children;
    CComponent() : AcceptButton(0), SetCalls(0) { Children._M_start = Children._M_finish = Children._M_end_of_storage = Storage; }
    void Add(CComponent* c) { Children._M_finish->Data = c; Children._M_finish->Info = 0; ++Children._M_finish; }
    virtual ~CComponent() { }
    virtual void SetPAcceptButton(CComponent* acceptButton) { AcceptButton = acceptButton; ++SetCalls; }
    virtual CComponentPointerVector& GetChildren() { return Children; }
};
}
using NUISystem::CComponent;
static void SetPAcceptButtonRecursive(CComponent* component, CComponent* acceptButton)
{
    for (unsigned int i = 0; i < component->GetChildren().size(); ++i) {
        if (component->GetChildren()[i].Get() != acceptButton) {
            component->GetChildren()[i]->SetPAcceptButton(acceptButton);
            SetPAcceptButtonRecursive(component->GetChildren()[i].Get(), acceptButton);
        }
    }
}
static void RunTest()
{
    // screen -> { list -> { item1, item2 }, accept -> { acceptLabel }, cancel }
    CComponent screen, list, item1, item2, accept, acceptLabel, cancel;
    screen.Add(&list); screen.Add(&accept); screen.Add(&cancel);
    list.Add(&item1); list.Add(&item2);
    accept.Add(&acceptLabel);
    SetPAcceptButtonRecursive(&screen, &accept);
    CHECK(screen.SetCalls == 0);                                     // the root itself is not told
    CHECK(list.AcceptButton == &accept && item1.AcceptButton == &accept && item2.AcceptButton == &accept);
    CHECK(cancel.AcceptButton == &accept && cancel.SetCalls == 1);
    CHECK(accept.SetCalls == 0 && acceptLabel.SetCalls == 0);        // the button's own subtree is skipped
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0054e350_TEST FAIL\n");
        return 1;
    }
    std::printf("0054e350_TEST PASS\n");
    return 0;
}