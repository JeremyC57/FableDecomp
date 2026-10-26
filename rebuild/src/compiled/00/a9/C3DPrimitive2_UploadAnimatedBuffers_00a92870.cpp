// C3DPrimitive2::UploadAnimatedBuffers(unsigned long flags) -- upload the index buffer once (marking it uploaded), the animated vertex buffer once (failing out if it cannot), optionally drop the device-independent data of the primitive and of every animated block, mark uploaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A92870, length 140 bytes.
// Leaf: UploadAnimatedBuffers
// Callees: C3DPrimitive2::UploadToIndexBuffer(ulong) @ 0x00A8F9E0 (result ignored here),
// C3DPrimitive2::UploadToVertexBufferAnimated(ulong) @ 0x00A914B0, N3DPrimitive::CDeviceIndependent's virtual
// deleting destructor (slot 0, `push 1 ; call [eax]`), the animated block data's virtual slot 1 (Destroy) when
// its count at +4 reaches zero.
// Layout: as the landed sibling UploadStaticBuffers (0x00A93320): Data +0x2c, VertexBufferStatus +0x38,
// IndexBufferStatus +0x3c, AnimatedBlockCount +0x48, AnimatedBlocks +0x4c, UploadedFlag +0x64. CAnimatedBlock
// (PDB, 48 bytes) holds its CIVCountedPointer<CDeviceIndependent> Data at +0x2c.
// Shape: the scoped_ptr reset detaches before deleting (the landed sibling's form); each block's pointer is
// assigned an empty CIVCountedPointer (Release with its own null test and inner zero store, then the copied
// null stored again -- the landed 0x00A24930 operator=); the loop keeps the index in ebx and the byte offset
// in ebp (reusing the zero register) and re-reads the count and the array.
struct N3DPrimitiveDeviceIndependent {             // N3DPrimitive::CDeviceIndependent
    virtual ~N3DPrimitiveDeviceIndependent();      // slot 0: the deleting destructor is called through the vtable
};
struct CCountedObject {                            // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        Release();
        Object = other.Object;
        if (Object)
            ++Object->m_refCount;
        return *this;
    }
};
struct CAnimatedBlockDeviceIndependent : CCountedObject { };   // N3DPrimitive::CAnimatedBlock::CDeviceIndependent
struct CAnimatedBlock {                            // 48 bytes (PDB)
    unsigned char _pad00[0x2c];
    CIVCountedPointer<CAnimatedBlockDeviceIndependent> Data;   // +0x2c
};

class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char                  _pad04[0x28];
    N3DPrimitiveDeviceIndependent* Data;                 // +0x2c scoped_ptr<N3DPrimitive::CDeviceIndependent>
    unsigned char                  _pad30[0x08];
    unsigned long                  VertexBufferStatus;   // +0x38
    unsigned long                  IndexBufferStatus;    // +0x3c
    unsigned char                  _pad40[0x08];
    unsigned long                  AnimatedBlockCount;   // +0x48
    CAnimatedBlock*                AnimatedBlocks;       // +0x4c
    unsigned char                  _pad50[0x14];
    bool                           UploadedFlag;         // +0x64
    bool UploadAnimatedBuffers(unsigned long flags);
    bool UploadToIndexBuffer(unsigned long flags);              // 0x00A8F9E0
    bool UploadToVertexBufferAnimated(unsigned long flags);     // 0x00A914B0
};

bool C3DPrimitive2::UploadAnimatedBuffers(unsigned long flags)
{
    if (!(IndexBufferStatus & 1)) {
        UploadToIndexBuffer(flags);
        IndexBufferStatus |= 1;
    }
    if (!(VertexBufferStatus & 1)) {
        if (!UploadToVertexBufferAnimated(flags))
            return false;
    }
    if (flags & 8) {
        N3DPrimitiveDeviceIndependent* old = Data;   // scoped_ptr::reset(): detach first, then delete
        Data = 0;
        delete old;
        for (unsigned long i = 0; i < AnimatedBlockCount; ++i)
            AnimatedBlocks[i].Data = CIVCountedPointer<CAnimatedBlockDeviceIndependent>();
    }
    UploadedFlag = true;
    return true;
}