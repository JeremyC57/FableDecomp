// CAnimationEntry::CAnimationEntry(long bankIndex, long transitionInTime, long flags, long delay) -- build an entry for a bank animation and attach transition-in-time, flags and delay components
// Leaf: ??0CAnimationEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DED00, 189 bytes (+3 int3), ret 0x10, returns `this`.
// Ego_r.pdb name ??0CAnimationEntry@@QAE@JJJJ@Z.
// Callees: operator new 0x00BFEA1A, CAnimationEntry::AddComponent(CAnimComponentBase*) 0x005DEB40.
// Components (12 bytes each: vptr, IVRefCount, one long): CAnimComponentTransitionInTime vtable 0x01255670,
// CAnimComponentFlags vtable 0x01255850, CAnimComponentDelayTime vtable 0x01255698 (rtti-vtables.tsv).
// Layout: PDB CAnimationEntry (0x10): BankIndex +0, CDefString AnimName +4, GroupName +8, CIVCountedPointer Components +0xc.
// Shape: same as the landed copy ctor 0x005DEC70: default member ctors (both CDefStrings -1 through one register), each
// component `new`ed with the base's inline count-of-1 constructor, the value stored through the result after the null
// join, then AddComponent. The last block's tail is duplicated by the optimiser (two `ret 0x10` exits).
void* operator new(unsigned int size);              // 0x00BFEA1A

class CIVCountedPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    unsigned long IVRefCount;                       // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) {}
};

class CAnimComponentBase : public CIVCountedPointeeBase {
};

class CAnimComponentTransitionInTime : public CAnimComponentBase {   // vtable 0x01255670
public:
    long TransitionInTime;                          // +0x08
    virtual void v00();
};
class CAnimComponentFlags : public CAnimComponentBase {              // vtable 0x01255850
public:
    long Flags;                                     // +0x08
    virtual void v00();
};
class CAnimComponentDelayTime : public CAnimComponentBase {          // vtable 0x01255698
public:
    long Delay;                                     // +0x08
    virtual void v00();
};

template<class T> struct CIVCountedPointer {        // intrusive counted pointer (4 bytes)
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) {}
};

struct CDefString {
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
    void AddComponent(CAnimComponentBase* component);   // 0x005DEB40
    CAnimationEntry(long bankIndex, long transitionInTime, long flags, long delay);
};

CAnimationEntry::CAnimationEntry(long bankIndex, long transitionInTime, long flags, long delay)
{
    BankIndex = bankIndex;
    CAnimComponentTransitionInTime* transition = new CAnimComponentTransitionInTime;
    transition->TransitionInTime = transitionInTime;
    AddComponent(transition);
    CAnimComponentFlags* flagComponent = new CAnimComponentFlags;
    flagComponent->Flags = flags;
    AddComponent(flagComponent);
    CAnimComponentDelayTime* delayComponent = new CAnimComponentDelayTime;
    delayComponent->Delay = delay;
    AddComponent(delayComponent);
}
