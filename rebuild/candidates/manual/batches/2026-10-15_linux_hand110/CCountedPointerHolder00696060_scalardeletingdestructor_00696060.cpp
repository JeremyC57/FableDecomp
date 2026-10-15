// CCountedPointerHolder00696060::`scalar deleting destructor'(unsigned int) -- release the counted pointer at +0xC and free the object when bit 0 of the flags is set
// Leaf: ??_GCCountedPointerHolder00696060
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696060, length 49 bytes (+ int3 padding).
// Callees: the pointee's virtual slot 1 (`call [eax+4]`, destroy at count zero), operator delete @ 0x00BFE9BC.
// Label correction: the manifest's "CThingHolySite::`vector_deleting_destructor'" is propagated over byte-identical
// bodies; nothing references this copy, so the class is named by address.
// Layout: local model -- three plain words, then a CIVCountedPointer at +0xC; no vtable.
// Shape: a NON-polymorphic class deleted with `delete` (VC7.1 emits the non-virtual `??_G`, no vtable store); the
// counted pointer's release is `__forceinline` (`dec dword ptr [ecx+4] ; jne`, the clear inside the null test).
struct CCountedObject {
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

struct CCountedPointerHolder00696060 {
    long                              m_word00;     // +0x00
    long                              m_word04;     // +0x04
    long                              m_word08;     // +0x08
    CIVCountedPointer<CCountedObject> m_object;     // +0x0c
};

void DeleteCCountedPointerHolder00696060(CCountedPointerHolder00696060* p)
{
    delete p;
}
