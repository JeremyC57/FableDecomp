// Behaviour test for 0x00A92870 C3DPrimitive2::UploadAnimatedBuffers
#include <cstdio>
static int g_devDeleted = 0, g_blockDestroyed = 0, g_ibUploads = 0;
struct CDev { virtual ~CDev() { ++g_devDeleted; } };
struct CCountedObject {
    long m_refCount; CCountedObject() : m_refCount(1) {}
    virtual ~CCountedObject() {}
    virtual void Destroy() { ++g_blockDestroyed; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> struct CIVCountedPointer {
    T* Object; CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& o) { Release(); Object = o.Object; if (Object) ++Object->m_refCount; return *this; }
};
struct CAnimatedBlock { CIVCountedPointer<CCountedObject> Data; };
struct C3DPrimitive2 {
    CDev* Data; unsigned long VertexBufferStatus, IndexBufferStatus, AnimatedBlockCount; CAnimatedBlock* AnimatedBlocks; bool UploadedFlag;
    bool vbResult;
    bool UploadToIndexBuffer(unsigned long) { ++g_ibUploads; return false; }
    bool UploadToVertexBufferAnimated(unsigned long) { if (vbResult) VertexBufferStatus |= 1; return vbResult; }
    bool UploadAnimatedBuffers(unsigned long flags)
    {
        if (!(IndexBufferStatus & 1)) { UploadToIndexBuffer(flags); IndexBufferStatus |= 1; }
        if (!(VertexBufferStatus & 1)) { if (!UploadToVertexBufferAnimated(flags)) return false; }
        if (flags & 8) {
            CDev* old = Data; Data = 0; delete old;
            for (unsigned long i = 0; i < AnimatedBlockCount; ++i) AnimatedBlocks[i].Data = CIVCountedPointer<CCountedObject>();
        }
        UploadedFlag = true;
        return true;
    }
};
int main()
{
    CCountedObject b0, b1; b1.m_refCount = 2;
    CAnimatedBlock blocks[3]; blocks[0].Data.Object = &b0; blocks[2].Data.Object = &b1;
    C3DPrimitive2 p; p.Data = new CDev; p.VertexBufferStatus = 0; p.IndexBufferStatus = 0; p.AnimatedBlockCount = 3; p.AnimatedBlocks = blocks; p.UploadedFlag = false;
    p.vbResult = false;
    bool ok = !p.UploadAnimatedBuffers(8) && g_ibUploads == 1 && p.IndexBufferStatus == 1 && !p.UploadedFlag && p.Data != 0;
    p.vbResult = true;
    ok = ok && p.UploadAnimatedBuffers(8) && g_ibUploads == 1 && p.UploadedFlag && p.Data == 0 && g_devDeleted == 1
        && g_blockDestroyed == 1 && b1.m_refCount == 1 && blocks[0].Data.Object == 0 && blocks[2].Data.Object == 0;
    blocks[0].Data.Object = 0;
    if (!ok) { std::printf("00a92870_TEST FAIL\n"); return 1; }
    std::printf("00a92870_TEST PASS\n");
    return 0;
}