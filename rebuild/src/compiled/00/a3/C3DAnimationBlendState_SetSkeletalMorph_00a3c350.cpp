// C3DAnimationBlendState::SetSkeletalMorph(const CCountedPointer<const C3DSkeletalMorph>&) -- unshares the data block, then replaces its SkeletalMorph (+0x18): release the old count block (destroy + free at zero), adopt the new object/block, addref
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3C350, length 79 bytes.
// Callees: C3DAnimationBlendState::MakeOwnCopyOfData @ 0x00A3C1A0; the block's destroy function
// (`call [block+4]` with the object in ecx); operator delete @ 0x00BFE9BC.
// C3DAnimationBlendState is {vptr, CIVCountedPointer<CSharedData> Data}; CSharedData has
// BaseAnimation +8, the 12-byte retail CArray CombinationBlends +0xc and the non-intrusive
// CCountedPointer {object, count block} SkeletalMorph at +0x18 (PDB +0x1c with the 16-byte Ego
// CArray). The count block is {references, destroy, object}. The inlined CCountedPointer::operator=
// reads the source's object and block into registers first (ebx/edi survive the destroy call),
// compares blocks, decrements through a reference (the block is re-read before the zero test),
// destroys and frees on zero, then stores object and block and bumps the new block's count.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        void* object = other.m_pObject;
        SCountBlock* block = other.m_pCount;
        if (m_pCount != block) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = block;
            if (block)
                ++block->m_references;
        }
        return *this;
    }
};

struct CSharedData {
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;              // +4
    void* BaseAnimation;                   // +8  CIVCountedPointer<C3DAnimationStateBase>
    unsigned char CombinationBlends[0xc];  // +0xc CArray<C3DAnimationStateAlpha> (12 bytes in retail)
    CCountedPointer SkeletalMorph;         // +0x18
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                     // +4
    void MakeOwnCopyOfData();              // 0x00A3C1A0
    void SetSkeletalMorph(const CCountedPointer& morph);
};

void C3DAnimationBlendState::SetSkeletalMorph(const CCountedPointer& morph)
{
    MakeOwnCopyOfData();
    Data->SkeletalMorph = morph;
}