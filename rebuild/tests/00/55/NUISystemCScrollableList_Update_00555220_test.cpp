#include <cstdio>
namespace stl {
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
}
struct IComponentPrimary {
    virtual void v00() {}
};
struct CObserver {
    virtual void a00() {}
    virtual void ProcessEvent(long eventType) { log[n++] = 300 + eventType; }
    virtual void a02() {}
    virtual void ObserveEvent(long eventType) { log[n++] = 100 + eventType; }
    virtual void StopObservingEvent(long eventType) { log[n++] = 200 + eventType; }
    long log[8]; int n;
};
struct IComponentSecondaryB {
    virtual void b00() {}
};
struct CFrontEndManager {
    CObserver* added[8]; int n;
    virtual void AddObserver(CObserver* observer) { added[n++] = observer; }
    static CFrontEndManager* GetInstance();
};
static CFrontEndManager g_manager;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_manager; }
struct CComponent : IComponentPrimary, CObserver, IComponentSecondaryB {
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CChangingStateComponent : CComponent {
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0
    float baseDt;
    void Update(float dt) { baseDt = dt; }
};
namespace NUISystem {
struct CScrollableList : CChangingStateComponent {
    unsigned long FirstVisible;          // +0x15c
    unsigned long VisibleCount;          // +0x160
    float         ArrowDelay;            // +0x16c
    CComponent*   PUpArrow;              // +0x170
    CComponent*   PDownArrow;            // +0x174
    bool          ShowUpArrow;           // +0x178
    bool          ShowDownArrow;         // +0x179
    void Update(float dt);
};

void CScrollableList::Update(float dt)
{
    CChangingStateComponent::Update(dt);
    if (ArrowDelay > 0.0f) {
        ArrowDelay -= dt;
        if (ArrowDelay <= 0.0f) {
            unsigned long count = Children.size();
            unsigned long end = FirstVisible + VisibleCount;
            unsigned long last = stl::min(end, count);
            for (unsigned long i = FirstVisible; i < last; ++i) {
                Children[i]->StopObservingEvent(0x19);
                Children[i]->ObserveEvent(0x19);
            }
            if (ShowUpArrow) {
                PUpArrow->StopObservingEvent(0x19);
                CFrontEndManager::GetInstance()->AddObserver(PUpArrow);
                PUpArrow->ObserveEvent(0x19);
                PUpArrow->ProcessEvent(0x19);
            }
            if (ShowDownArrow) {
                PDownArrow->StopObservingEvent(0x19);
                CFrontEndManager::GetInstance()->AddObserver(PDownArrow);
                PDownArrow->ObserveEvent(0x19);
                PDownArrow->ProcessEvent(0x19);
            }
        }
    }
}
}

int main()
{
    using namespace NUISystem;
    static CScrollableList list; static CComponent rows[5], up, down;
    static CCountedPointer<CComponent> ptrs[5];
    for (int i = 0; i < 5; ++i) { ptrs[i].Data = &rows[i]; rows[i].n = 0; }
    list.Children._M_start = ptrs; list.Children._M_finish = ptrs + 5;
    list.FirstVisible = 3; list.VisibleCount = 4; list.ArrowDelay = 0.5f;
    list.PUpArrow = &up; list.PDownArrow = &down; list.ShowUpArrow = true; list.ShowDownArrow = false;
    up.n = 0; down.n = 0; g_manager.n = 0;
    list.Update(0.25f);                                  // still waiting
    bool ok = list.baseDt == 0.25f && rows[3].n == 0 && up.n == 0 && list.ArrowDelay == 0.25f;
    list.Update(0.25f);                                  // expires: rows 3..4 (clamped to size), up arrow only
    ok = ok && rows[2].n == 0 && rows[3].n == 2 && rows[3].log[0] == 225 && rows[3].log[1] == 125 && rows[4].n == 2
            && up.n == 3 && up.log[0] == 225 && up.log[1] == 125 && up.log[2] == 325 && down.n == 0
            && g_manager.n == 1 && g_manager.added[0] == static_cast<CObserver*>(&up);
    list.Update(1.0f);                                   // delay already spent: nothing more
    ok = ok && rows[3].n == 2 && up.n == 3;
    if (!ok) { std::printf("00555220_TEST FAIL\n"); return 1; }
    std::printf("00555220_TEST PASS\n");
    return 0;
}