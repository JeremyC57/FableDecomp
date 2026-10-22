// CAnimateThing::CalcCachedAnims() -- recompute the start (t=0) and end (t=1) blend states from the animation queue head, then apply the owner's skeletal morph to both when it has one
// Leaf: ?CalcCachedAnims@CAnimateThing
// Layout: CAnimateThing as the landed UpdateCachedAnims 0x0070E520 (StartAnimState +0x68, EndAnimState +0x70, owner +0xa0);
//   C3DAnimationBlendState {vptr, CIVCountedPointer Data} and the {object, count block} CCountedPointer as the landed
//   SetSkeletalMorph 0x00A3C350; CThing bitset +0x20, interface map +0x44.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};
struct CSkeletalMorph;
struct CCountedPointer_CSkeletalMorph {                 // CCountedPointer<C3DSkeletalMorph>
    CSkeletalMorph* m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer_CSkeletalMorph()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
struct CCountedPointer_ConstSkeletalMorph {             // CCountedPointer<const C3DSkeletalMorph>
    const CSkeletalMorph* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer_ConstSkeletalMorph(const CCountedPointer_CSkeletalMorph& other)
        : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer_ConstSkeletalMorph()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CTCSkeletalMorph {
    CCountedPointer_CSkeletalMorph GetPSkeletalMorph(); // 0x00786870
};
struct CIVCountedAnimationData {
    virtual void Destroy(int);
    virtual void Delete();
    unsigned long IVRefCount;                           // +4
    __forceinline unsigned long DecRef() { return --IVRefCount; }
};
struct CIVAnimationDataPointer {
    CIVCountedAnimationData* Object;
    CIVAnimationDataPointer& operator=(const CIVAnimationDataPointer& other)
    {
        CIVCountedAnimationData* old = Object;
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
    ~CIVAnimationDataPointer()
    {
        if (Object) {
            CIVCountedAnimationData* object = Object;
            if (object->DecRef() == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct CBaseClass {
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct C3DAnimationBlendState : CBaseClass {
    CIVAnimationDataPointer Data;                       // +4
    void SetSkeletalMorph(const CCountedPointer_ConstSkeletalMorph& morph);   // 0x00A3C350
};
struct CAnimQueueEntry {
    unsigned char _pad00[8];
    unsigned char Anims[1];                             // +8
};
struct CAnimQueue {
    CAnimQueueEntry* Head;                              // +0
};
struct CAnimateThing {
    unsigned char _pad00[0x60];
    CAnimQueue* PQueue;                                 // +0x60
    unsigned char _pad64[0x68 - 0x64];
    C3DAnimationBlendState StartAnimState;              // +0x68
    C3DAnimationBlendState EndAnimState;                // +0x70
    C3DAnimationBlendState LastStartAnimState;          // +0x78
    C3DAnimationBlendState LastEndAnimState;            // +0x80
    float Scale, ChangeAlphaBy;
    long AnimDelay;
    unsigned long Interruptable;
    unsigned long CurrentAnimRefreshStamp, LastAnimRefreshStamp;
    CThing* PThing;                                     // +0xa0
    C3DAnimationBlendState CalcBlendState(const CAnimateThing& thing, const void* anims, float time);   // 0x0070D940
    void CalcCachedAnims();
};

void CAnimateThing::CalcCachedAnims()
{
    StartAnimState.Data = CalcBlendState(*this, PQueue->Head->Anims, 0.0f).Data;
    EndAnimState.Data = CalcBlendState(*this, PQueue->Head->Anims, 1.0f).Data;
    CTCSkeletalMorph* morph;
    if (PThing->GetInterface(&morph, 0x78)) {
        bool hasMorph = morph->GetPSkeletalMorph().m_pObject != 0;
        if (hasMorph) {
            StartAnimState.SetSkeletalMorph(morph->GetPSkeletalMorph());
            EndAnimState.SetSkeletalMorph(morph->GetPSkeletalMorph());
        }
    }
}
