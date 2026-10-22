// CGraphicInfoFrame::CFrameData::OnResourceLoaded() -- once an async read is pending (+0x30): try to upload the frame (0x009FF220, 0); when that fails and no graphic frame data exists (+0x1c), drop the owner's counted data pointer (owner at +0x40, pointer at its +8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FF860, length 80 bytes.
// Callees: UploadFrame @ 0x009FF220 (on `this`, 0; a bool in al), CIVCountedPointeeBase virtual slot 1 (Delete) when the count reaches zero. Shape: the inlined
// `CIVCountedPointer::operator=(0)` (GOTCHAS wave 24h: the release through a local pointee copy, its zero store inside the null test, then the assignment's
// unconditional zero store); the pointer address `owner+8` formed with `add esi,8`.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(CIVCountedPointeeBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
        return *this;
    }
};

struct CFrameOwner_009ff860 {
    unsigned char     _pad[8];
    CIVCountedPointer m_data;                      // +0x08
};

struct CFrameData {
    unsigned char         _pad[0x1c];
    void*                 m_pGraphic;              // +0x1c
    unsigned char         _pad20[0x10];
    void*                 m_pAsyncRead;            // +0x30
    unsigned char         _pad34[0xc];
    CFrameOwner_009ff860* m_pOwner;                // +0x40
    bool UploadFrame(long flags);                  // 0x009FF220
    void OnResourceLoaded();
};

void CFrameData::OnResourceLoaded()
{
    if (m_pAsyncRead) {
        if (!UploadFrame(0)) {
            if (!m_pGraphic)
                m_pOwner->m_data = 0;
        }
    }
}