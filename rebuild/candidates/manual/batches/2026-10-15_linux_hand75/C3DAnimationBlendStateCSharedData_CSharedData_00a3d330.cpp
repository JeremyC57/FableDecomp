// C3DAnimationBlendState::CSharedData::CSharedData(const CSharedData& other) -- copy constructor: the CIVCountedPointeeBase part (vtable, reference count copied), this class's vtable, the intrusive BaseAnimation pointer add-ref'ed, the blend array copied out of line, the counted SkeletalMorph shared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3D330, length 76 bytes.
// Leaf: ??0CSharedData
// Callees: CArray<C3DAnimationStateAlpha>::CArray(const CArray&) @ 0x00A3CDD0.
// Layout as the landed SetSkeletalMorph 0x00A3C350: IVRefCount +4, BaseAnimation +8 (CIVCountedPointer: the pointee's
// IVRefCount at +4), the 12-byte retail CArray CombinationBlends +0xc, the CCountedPointer {object, count block}
// SkeletalMorph +0x18 (the block's reference count at +0). Vtables 0x012354D0 (CIVCountedPointeeBase) and 0x0129D1F8.
// engine/CIVCountedPointeeBase.h is a flat record (void* vptr, IVRefCount) with no virtual functions, so the polymorphic
// base whose vtable store this body inlines is modelled locally with the header's member name.
// Shape: the base's implicit copy (its vtable, then IVRefCount) before the derived vtable; each pointer copied, then
// null-tested on the stored register and add-ref'ed with `inc dword ptr [reg+N]`; `ret 4`.
struct CIVCountedPointeeBase {                  // vtable 0x012354D0
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                   // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
    void AddRef() { ++IVRefCount; }
};

struct C3DAnimationStateBase : CIVCountedPointeeBase {
};

template <class T> struct CIVCountedPointer {   // intrusive: the pointee carries its own count
    T* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            m_pObject->AddRef();
    }
};

struct SCountBlock {
    long  m_references;                         // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

template <class T> struct CCountedPointer {     // non-intrusive: object + shared count block
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};

struct C3DAnimationStateAlpha;
struct C3DSkeletalMorph;

template <class T> struct CArray {              // retail CArray: first, last, end (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray(const CArray& other);                // 0x00A3CDD0
};

struct C3DAnimationBlendState {
    struct CSharedData : CIVCountedPointeeBase {                        // vtable 0x0129D1F8
        CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;         // +0x08
        CArray<C3DAnimationStateAlpha>          CombinationBlends;     // +0x0c
        CCountedPointer<const C3DSkeletalMorph> SkeletalMorph;         // +0x18
        CSharedData(const CSharedData& other);
        virtual void Destroy(int flags);
    };
};

C3DAnimationBlendState::CSharedData::CSharedData(const CSharedData& other)
    : CIVCountedPointeeBase(other),
      BaseAnimation(other.BaseAnimation),
      CombinationBlends(other.CombinationBlends),
      SkeletalMorph(other.SkeletalMorph)
{
}
