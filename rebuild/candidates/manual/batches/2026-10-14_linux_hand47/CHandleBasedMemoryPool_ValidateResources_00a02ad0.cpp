// CHandleBasedMemoryPool::ValidateResources() -- for every handle slot (the CArray at +0x438) that points at a taken area ('T'), validate the area's resource handle through virtual slot 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02AD0, length 96 bytes.
// Callees: this pool's virtual slot 3 (+0xc, one argument: the header's handle word). Shape: the count recomputed from the array's first/last pointers before the loop and after
// every virtual call (`sar 2`, unsigned compares), the index in edi, the aligned loop head.
// Layout: engine/CHandleBasedMemoryPool.h models Handles as a 16-byte CArray; retail's is 12 bytes {first, last, end} at +0x438 (a local model with the pool's vtable).
struct CHeader {                                   // CHandleBasedMemoryPool::CHeader
    long           Handle;                         // +0x00
    long           _unused04;
    long           Size;                           // +0x08
    short          Padding;                        // +0x0c
    char           Type;                           // +0x0e ('N' free, 'T' taken)
    char           Flags;
    CHeader**      PPrevFreeLink;                  // +0x10
    CHeader*       PNextFree;                      // +0x14
    CHeader*       PPrevPhysical;                  // +0x18
    CHeader*       PNextPhysical;                  // +0x1c
};

template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long GetCount() const { return (unsigned long)(m_pLast - m_pFirst); }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CHandleBasedMemoryPool {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void ValidateResource(long handle);    // slot 3 (+0x0c)
    unsigned char    _pad04[0x438 - 4];
    CArray<CHeader*> Handles;                      // +0x438 CArray<CHandleInfo>
    void ValidateResources();
};

void CHandleBasedMemoryPool::ValidateResources()
{
    for (unsigned long index = 0; index < Handles.GetCount(); ++index) {
        CHeader* area = Handles[index];
        if (area && area->Type == 'T')
            ValidateResource(area->Handle);
    }
}
