// CMeshData::~CMeshData() -- release the def pointer at +0x18, then the four def pointers of the array +8..+0x14 in reverse order (each: the pointee's reference count at +4 decremented, Delete (slot 1) at zero, the pointer cleared), then the CIVCountedPointeeBase base's inline destructor stores its vtable (0x012354D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2B6F0, length 80 bytes.
// Leaf: ??1CMeshData@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class, `this` in edi, the array destroyed by a count-down loop (esi stepping back by 4, ebx the count), `ret`.
struct CIVCountedPointeeBase {                     // vtable 0x012354D0
    virtual void Destroy(int flags);
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +0x04
    ~CIVCountedPointeeBase() { }                   // inline: the vtable store
};
struct CDefPointer_00a2b6f0 {                      // 4 bytes: a counted pointee
    CIVCountedPointeeBase* m_pObject;
    ~CDefPointer_00a2b6f0()
    {
        CIVCountedPointeeBase* object = m_pObject;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            m_pObject = 0;
        }
    }
};

struct __declspec(novtable) CMeshData : CIVCountedPointeeBase {   // no vtable store of its own: only the base's inline destructor stores one
    CDefPointer_00a2b6f0 m_lods[4];                // +0x08
    CDefPointer_00a2b6f0 m_pSkeleton;              // +0x18
    ~CMeshData();
};

CMeshData::~CMeshData()
{
}
