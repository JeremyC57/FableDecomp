// CCountedDataOwner005e0de0::`scalar deleting destructor'(unsigned int) -- release the counted pointer at +4, run the instance-counted base destructor, free when bit 0 of the flags is set
// Leaf: ??_GCCountedDataOwner005e0de0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0DE0, length 56 bytes (+ int3 padding).
// Callees: the pointee's virtual slot 1 (`call [eax+4]`, destroy at count zero), the base destructor @ 0x0099EAE0
// (calls 0x0099E9B0 and decrements the live-instance counter at 0x013BD800; the manifest's "_Cons_val<...>" label is
// propagated), operator delete @ 0x00BFE9BC.
// Label correction: the manifest's "CThingHolySite::`vector_deleting_destructor'" is propagated over byte-identical
// bodies; no vtable or call references this copy (an ICF-surviving duplicate), so the class is named by address.
// Layout: local model -- a base object at +0 whose destructor is out of line, a CIVCountedPointer at +4.
// Shape: the same as 0x004C3CD0 (C3DAnimationBlendState): no vtable store, the base destructor on `this` after the
// member release = a non-polymorphic class with the base object as its FIRST MEMBER; the counted pointer's release
// (`dec dword ptr [ecx+4] ; jne`, clear inside the null test) and the class destructor are `__forceinline`.
struct CInstanceCountedBase {
    virtual ~CInstanceCountedBase();                // 0x0099EAE0
};

struct CCountedData {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> struct CIVCountedPointer {
    T* m_p;
    __forceinline ~CIVCountedPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};

struct CCountedDataOwner005e0de0 {
    CInstanceCountedBase            m_base;         // +0x00
    CIVCountedPointer<CCountedData> m_data;         // +0x04
    __forceinline ~CCountedDataOwner005e0de0() {}
};

void DeleteCCountedDataOwner005e0de0(CCountedDataOwner005e0de0* p)
{
    delete p;
}
