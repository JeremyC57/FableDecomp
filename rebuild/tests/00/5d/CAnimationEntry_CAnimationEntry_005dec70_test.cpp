// Behaviour test for CAnimationEntry::CAnimationEntry(const CAnimationEntry&, long) (0x005DEC70): copy
// with counted-pointer add-ref, then a new transition-in-time component (count 1) handed to AddComponent.
#include <cstdio>

class CIVCountedPointeeBase { public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) {}
    virtual ~CIVCountedPointeeBase() {}
    virtual void Destroy() {}
};
class CAnimComponentTransitionInTime : public CIVCountedPointeeBase { public: long TransitionInTime; };
struct CPtr {
    CIVCountedPointeeBase* m_pObject;
    CPtr() : m_pObject(0) {}
    CPtr& operator=(const CPtr& o)
    {
        if (m_pObject != o.m_pObject) {
            if (m_pObject) { if (--m_pObject->IVRefCount == 0) m_pObject->Destroy(); m_pObject = 0; }
            m_pObject = o.m_pObject;
            if (m_pObject) ++m_pObject->IVRefCount;
        }
        return *this;
    }
};
class CDefString { public: long TablePos; CDefString() : TablePos(-1) {} };
class CAnimationEntry { public:
    long BankIndex; CDefString AnimName, GroupName; CPtr Components;
    CAnimComponentTransitionInTime* added;
    CAnimationEntry() : BankIndex(0), added(0) {}
    CAnimationEntry& operator=(const CAnimationEntry& o)
    { BankIndex = o.BankIndex; AnimName = o.AnimName; GroupName = o.GroupName; Components = o.Components; return *this; }
    void AddComponent(CAnimComponentTransitionInTime* c) { added = c; }
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime) : added(0)
    {
        *this = other;
        CAnimComponentTransitionInTime* component = new CAnimComponentTransitionInTime;
        component->TransitionInTime = transitionInTime;
        AddComponent(component);
    }
};

int main()
{
    CIVCountedPointeeBase map; map.IVRefCount = 1;
    CAnimationEntry src; src.BankIndex = 4; src.AnimName.TablePos = 10; src.GroupName.TablePos = 11; src.Components.m_pObject = &map;
    CAnimationEntry e(src, 250);
    if (e.BankIndex != 4 || e.AnimName.TablePos != 10 || e.GroupName.TablePos != 11 || e.Components.m_pObject != &map
        || map.IVRefCount != 2 || !e.added || e.added->TransitionInTime != 250 || e.added->IVRefCount != 1) {
        std::printf("005dec70_TEST FAIL\n"); return 1;
    }
    delete e.added;
    std::printf("005dec70_TEST PASS\n");
    return 0;
}