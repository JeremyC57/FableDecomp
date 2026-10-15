// CAnimateThing::GetMeshAnims(CIVCountedPointer<C3DAnimation>& first, CIVCountedPointer<C3DAnimation>& second) -- refresh the cached animation pointers, then hand out the two current ones through counted-pointer assignment
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070E6A0, 106 bytes (+6 int3), __thiscall, ret 8.
// Callee: CAnimateThing::UpdateCachedAnims 0x0070E520 (on `this`); the release is the pointee's virtual
// slot 1 (`call [eax+4]`).
// Layout: the cached anims are intrusive counted pointers -- 8 bytes, the pointee in the second word --
// at +0x68 and +0x70 (UpdateCachedAnims 0x0070E520 assigns +0x68 -> +0x78 and +0x70 -> +0x80 with the
// same inlined operator=). The pointee keeps its reference count at +4 behind its vptr.
// Shape: the inlined `operator=` compares the held pointer against the source's (read from memory, not
// cached), releases the old one through a local copy (`dec [ecx+4] ; jne ; call [vt+4]`) and clears the
// slot, then copies the source pointer (re-read) and bumps its count when non-null.
class C3DAnimation {
public:
    virtual void v00();
    virtual void Release();                         // slot 1
    long m_refCount;                                // +4
};

template <class T>
class CIVCountedPointer {
public:
    void* m_owner;                                  // +0 (not touched here)
    T* m_pObject;                                   // +4
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                T* old = m_pObject;
                if (--old->m_refCount == 0)
                    old->Release();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                ++m_pObject->m_refCount;
        }
        return *this;
    }
};

class CAnimateThing {
public:
    unsigned char _pad_0x00[0x68];
    CIVCountedPointer<C3DAnimation> m_cachedFirst;  // +0x68
    CIVCountedPointer<C3DAnimation> m_cachedSecond; // +0x70
    void UpdateCachedAnims();                       // 0x0070E520
    void GetMeshAnims(CIVCountedPointer<C3DAnimation>& first, CIVCountedPointer<C3DAnimation>& second);
};

void CAnimateThing::GetMeshAnims(CIVCountedPointer<C3DAnimation>& first, CIVCountedPointer<C3DAnimation>& second)
{
    UpdateCachedAnims();
    first = m_cachedFirst;
    second = m_cachedSecond;
}
