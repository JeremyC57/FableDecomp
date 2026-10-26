// CAnimatedBlock::DiscardDeviceIndependentResources() -- drop the device-independent data block (intrusive counted pointer at +0x2c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8EE90, length 36 bytes.
// Callees: virtual slot 1 (+0x04) of the pointee: CIVCountedPointeeBase::Destroy, called when the count hits zero.
// Shape: CIVCountedPointer<T>::operator=(T*) with a null argument, inlined -- the Release() part
// (`if (p) { if (--p->count == 0) p->Destroy(); p = 0; }`) followed by the unconditional store of the
// new (null) value, which is why the null store appears twice. `this` rides in esi across the virtual call.
// Layout: N3DPrimitive::CAnimatedBlock (PDB struct_layouts_egor.tsv, 0x30 bytes): the
// CIVCountedPointer<CAnimatedBlock::CDeviceIndependent> Data sits at +0x2c -- the only member touched here.
class CIVCountedPointeeBase { public:
    virtual void v0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};

template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release()
    {
        T* p = m_pObject;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(T* object)
    {
        Release();
        m_pObject = object;
        return *this;
    }
};

struct CDeviceIndependent : CIVCountedPointeeBase { };

struct CAnimatedBlock {
    unsigned char _pad[0x2c];
    CIVCountedPointer<CDeviceIndependent> Data;   // +0x2c
    void DiscardDeviceIndependentResources();
};

void CAnimatedBlock::DiscardDeviceIndependentResources()
{
    Data = 0;
}