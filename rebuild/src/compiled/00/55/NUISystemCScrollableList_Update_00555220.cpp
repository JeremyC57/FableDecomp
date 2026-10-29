// NUISystem::CScrollableList::Update(float) -- after the arrow delay runs out, re-arm the visible rows and the shown arrows as event observers
// Leaf: Update
// Retail 0x00555220, 368 bytes, ret 4. Callees: CChangingStateComponent::Update 0x0052C7E0, CFrontEndManager::GetInstance
// 0x0041E5F2 and its AddObserver (slot 2, +0x08); the components' CObserver virtuals (base at +4): ProcessEvent slot 1,
// ObserveEvent slot 3, StopObservingEvent slot 4 (names as in the landed CClickable/CMouseCursor models).
// Layout: retail CComponent offsets as the landed CComponentContainer::Initialise 0x005461D0 (Children +0xB0, a 12-byte
// STLport vector of 8-byte CCountedPointer<CComponent>); CScrollableList FirstVisible +0x15C, VisibleCount +0x160,
// ArrowDelay +0x16C, PUpArrow +0x170, PDownArrow +0x174, ShowUpArrow +0x178, ShowDownArrow +0x179 (descriptive names).
// Shape: `min(first + visible, size())` is STLport's reference-returning min (`b < a ? b : a`: the size lives in the dead
// dt slot and the address of the smaller is taken); Children[i] is re-indexed for each call; AddObserver(arrow) converts
// to the CObserver base with VC's null test while the member calls adjust the pointer unconditionally.
namespace stl {
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
}
struct IComponentPrimary {
    virtual void v00();
};
struct CObserver {
    virtual void a00();
    virtual void ProcessEvent(long eventType);           // slot 1 (+0x04)
    virtual void a02();
    virtual void ObserveEvent(long eventType);           // slot 3 (+0x0c)
    virtual void StopObservingEvent(long eventType);     // slot 4 (+0x10)
    unsigned char _pad04[0x10];
};
struct IComponentSecondaryB {
    virtual void b00();
};
namespace NUISystem { class CManager { public:
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
}; }
struct CComponent : IComponentPrimary, CObserver, IComponentSecondaryB {
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
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
    unsigned char _pad1c[0xb0 - 0x1c];
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0
    void Update(float dt);                                  // 0x0052C7E0
};
namespace NUISystem {
class CScrollableList : public CChangingStateComponent { public:
    unsigned char _padbc[0x15c - 0xbc];
    unsigned long FirstVisible;          // +0x15c
    unsigned long VisibleCount;          // +0x160
    unsigned char _pad164[8];
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
                CManager::GetInstance()->AddObserver(PUpArrow);
                PUpArrow->ObserveEvent(0x19);
                PUpArrow->ProcessEvent(0x19);
            }
            if (ShowDownArrow) {
                PDownArrow->StopObservingEvent(0x19);
                CManager::GetInstance()->AddObserver(PDownArrow);
                PDownArrow->ObserveEvent(0x19);
                PDownArrow->ProcessEvent(0x19);
            }
        }
    }
}
}