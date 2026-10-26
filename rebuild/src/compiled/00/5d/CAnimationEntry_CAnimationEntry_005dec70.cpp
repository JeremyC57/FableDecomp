// CAnimationEntry::CAnimationEntry(const CAnimationEntry& other, long transitionInTime) -- copy an animation entry and give the copy a fresh transition-in-time component
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DEC70, 135 bytes (+9 int3), __fastcall, ret 8, returns `this`.
// Leaf: ??0CAnimationEntry@@
// Callees: operator new 0x00BFEA1A, CAnimationEntry::AddComponent(CAnimComponentBase*) 0x005DEB40; the old
// component map's release is its virtual slot 1 (`call [eax+4]`).
// Layout: PDB CAnimationEntry (0x10, as the landed operator= 0x00471A09): BankIndex, CDefString AnimName,
// CDefString GroupName, CIVCountedPointer<CAnimComponentMap> Components. CAnimComponentTransitionInTime
// (vtable 0x01255670, engine/CAnimComponentTransitionInTime.h) is 12 bytes: vptr, IVRefCount,
// TransitionInTime (long).
// Shape: the members are default-constructed first (both CDefStrings -1 through one register, the
// counted pointer nulled with an immediate), then the body assigns `*this = other` through the inline
// memberwise operator= (three dwords, then the counted-pointer assignment with its inline release). The
// component is `new`ed: CIVCountedPointeeBase's inline constructor stores the count of 1 before the
// derived vtable (the base vtable store is dead), the null check yields 0 on failure, and the time is
// stored through the result after the join (no second null test), then handed to AddComponent.
void* operator new(unsigned int size);              // 0x00BFEA1A

class CIVCountedPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    unsigned long IVRefCount;                       // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) {}
};

class CAnimComponentBase : public CIVCountedPointeeBase {
public:
};

class CAnimComponentTransitionInTime : public CAnimComponentBase {
public:
    long TransitionInTime;                          // +0x08
    virtual void v00();
};

template<class T> class CIVCountedPointer { public:        // intrusive counted pointer (4 bytes)
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) {}
    void Release()
    {
        T* p = m_pObject;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Destroy();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            T* p = other.m_pObject;
            m_pObject = p;
            if (p)
                ++p->IVRefCount;
        }
        return *this;
    }
};

class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) {}
};

class CAnimComponentMap : public CIVCountedPointeeBase {
};

class CAnimationEntry {
public:
    long BankIndex;                                 // +0x00
    CDefString AnimName;                            // +0x04
    CDefString GroupName;                           // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;   // +0x0c
    CAnimationEntry& operator=(const CAnimationEntry& other)
    {
        BankIndex = other.BankIndex;
        AnimName = other.AnimName;
        GroupName = other.GroupName;
        Components = other.Components;
        return *this;
    }
    void AddComponent(CAnimComponentBase* component);   // 0x005DEB40
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime);
};

CAnimationEntry::CAnimationEntry(const CAnimationEntry& other, long transitionInTime)
{
    *this = other;
    CAnimComponentTransitionInTime* component = new CAnimComponentTransitionInTime;
    component->TransitionInTime = transitionInTime;
    AddComponent(component);
}