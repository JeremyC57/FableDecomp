// C3DAnimationBlendState::Validate() const -- validate the shared data's base animation and every combination blend
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3C0C0, 114 bytes. Callees: virtual slot 5 (+0x14) of the animation
// states (C3DAnimationState vtable 0x01249F50 slot 5 = 0x00A3BE10).
// Model as the landed C3DAnimationBlendState::SetSkeletalMorph (0x00A3C350): {vptr, CIVCountedPointer<CSharedData>
// Data +4}; CSharedData has BaseAnimation +8 and the 12-byte retail CArray<C3DAnimationStateAlpha>
// CombinationBlends +0xC (20-byte polymorphic elements: the 0x66666667 / sar 3 divide). Data is re-read and
// the size recomputed after every virtual call (the loop test is `i < Size()` through the member);
// VC strength-reduces the element address to a running byte offset. BaseAnimation must be a
// CIVCountedPointer with an inline operator-> (a raw pointer member fetches the vtable into edx, not eax).
struct C3DAnimationStateBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void Validate();               // slot 5 (+0x14)
    unsigned long IVRefCount;              // +4
};

struct C3DAnimationStateAlpha : C3DAnimationStateBase {   // 0x14 bytes
    unsigned char _body[0x08];             // +0x08 .. +0x0F
    float AlphaWeight;                     // +0x10
};

template <class T>
struct CArray {                            // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;                           // +0
    T* m_pEnd;                             // +4
    T* m_pEndOfStorage;                    // +8
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

template <class T>
struct CIVCountedPointer {
    T* m_pObject;
    T* operator->() const { return m_pObject; }
};

struct CSharedData {
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                          // +4
    CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;   // +8
    CArray<C3DAnimationStateAlpha> CombinationBlends;  // +0xc
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                     // +4 CIVCountedPointer<CSharedData>
    void Validate() const;
};

void C3DAnimationBlendState::Validate() const
{
    if (Data) {
        Data->BaseAnimation->Validate();
        for (unsigned long i = 0; i < Data->CombinationBlends.Size(); ++i)
            Data->CombinationBlends[i].Validate();
    }
}
