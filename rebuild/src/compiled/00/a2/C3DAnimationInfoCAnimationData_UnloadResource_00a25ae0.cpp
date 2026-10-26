// C3DAnimationInfo::CAnimationData::UnloadResource() -- when the data may be unloaded, drop the info block's shared data and report "not resident"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A25AE0, length 55 bytes (+ int3 padding). Vtable slot 2 of
// C3DAnimationInfo::CAnimationData (0x0129CE50).
// Callees: CAnimationData::CanUnload() @ 0x00A25A70; virtual slot 1 (+0x04) of the pointee (Destroy) when its
// count reaches zero.
// Layout: PDB CAnimationData (0x40 bytes, 4 high after the 16-byte PDB map at +0x1c): AnimInfo (C3DAnimationInfo*)
// is at retail +0x38; the info block's CIVCountedPointer at +0x0c is what is released (`add esi,0Ch` forms its
// address once, the inlined operator=(T*) with null then reads/writes through it -- hence the double null store).
// Shape: `if (CanUnload()) { ...; return false; } return true;` -- the true tail (`mov al,1`) sits after the
// release path, so the body is the if-wrapped form, not an early `return true`.
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

class C3DAnimationInfo { public:
    unsigned char _pad[0x0c];
    CIVCountedPointer<CIVCountedPointeeBase> m_data;    // +0x0c the shared animation data block
};

struct CAnimationData {
    virtual void v0();
    virtual void v1();
    virtual bool UnloadResource();              // slot 2
    unsigned char _pad[0x34];                   // +0x04 .. +0x37 (bank/map/counted pointers)
    C3DAnimationInfo* AnimInfo;                 // +0x38
    bool CanUnload();                           // 0x00A25A70
};

bool CAnimationData::UnloadResource()
{
    if (CanUnload()) {
        AnimInfo->m_data = 0;
        return false;
    }
    return true;
}