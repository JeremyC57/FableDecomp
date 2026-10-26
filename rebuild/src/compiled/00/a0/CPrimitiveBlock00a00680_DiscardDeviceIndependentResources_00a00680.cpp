// CPrimitiveBlock_00a00680::DiscardDeviceIndependentResources() -- drop the device-independent data block (intrusive counted pointer at +0x08)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00680, length 36 bytes.
// Callees: virtual slot 1 (+0x04) of the pointee: CIVCountedPointeeBase::Destroy, called when the count hits zero.
// Shape: CIVCountedPointer<T>::operator=(T*) with a null argument, inlined -- the Release() part
// (`if (p) { if (--p->count == 0) p->Destroy(); p = 0; }`) followed by the unconditional store of the
// new (null) value, which is why the null store appears twice. `this` rides in esi across the virtual call.
// The manifest label (CAnimatedBlock::DiscardDeviceIndependentResources) is propagated from the +0x2c body
// @ 0x00A8EE90; this body resets a counted pointer at +0x08, so it is a sibling N3DPrimitive block class
// (not identified in the PDB layouts) -- modelled by its address.
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

struct CPrimitiveBlock_00a00680 {
    unsigned char _pad[0x08];
    CIVCountedPointer<CDeviceIndependent> Data;   // +0x08
    void DiscardDeviceIndependentResources();
};

void CPrimitiveBlock_00a00680::DiscardDeviceIndependentResources()
{
    Data = 0;
}