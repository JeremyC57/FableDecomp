// C3DAnimationInterpolator::operator=(const C3DAnimationInterpolator&) -- assign the start and end blend states (their shared-data counted pointers)
// Leaf: ??4C3DAnimationInterpolator
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD85F0, 112 bytes. Callees: the old pointee's virtual Delete (slot 1, +4)
// when its count (+4) drops to zero.
// Layout from the PDB: StartState +4 and EndState +0xC are C3DAnimationBlendState {vptr, CIVCountedPointer<CSharedData>
// Data} (landed model 0x00A3C350), so only the Data words at +8 / +0x10 move (the implicit blend-state assignment
// leaves the vptrs alone). Each is the inlined CIVCountedPointer::operator=: compare the pointers, release the old pointee through an inline member of the
// pointee (`dec [p+4] ; jne ; call [vt+4]`, the pointer stays in ecx), clear, then re-read the source,
// store and add-ref (`inc [p+4]`).
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1 (+4)
    unsigned long IVRefCount;              // +4
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) Delete(); }
};

template <class T>
class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                m_pObject->Release();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                m_pObject->AddRef();
        }
        return *this;
    }
};

struct CSharedData : CIVCountedPointeeBase { };       // C3DAnimationBlendState::CSharedData

class C3DAnimationBlendState { public:
    virtual void v00();
    CIVCountedPointer<CSharedData> Data;                     // +4
};

class C3DAnimationInterpolator { public:
    virtual void v00();
    C3DAnimationBlendState StartState;                       // +0x04
    C3DAnimationBlendState EndState;                         // +0x0C
    C3DAnimationInterpolator& operator=(const C3DAnimationInterpolator& other);
};

C3DAnimationInterpolator& C3DAnimationInterpolator::operator=(const C3DAnimationInterpolator& other)
{
    StartState = other.StartState;
    EndState = other.EndState;
    return *this;
}