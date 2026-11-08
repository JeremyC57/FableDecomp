// CHandleBasedMemoryPool_012b4098::Free(long handle) -- release one allocation: mark its area free and put it on the free list, count the release and take the area's bytes (header, payload and padding) off the allocated size, merge it with free neighbours and push the handle onto the free-handle chain
// Leaf: ?Free@CHandleBasedMemoryPool_012b4098@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28690, 82 bytes, thiscall `ret 4`. Callers 0x00C221CC, 0x00C22226, 0x00C261C6,
//   0x00C263D5, 0x00C265D5 (the libpk sample / stream owners). The libpk build of the engine's CHandleBasedMemoryPool::Free
//   (Ego_r.pdb `?Free@CHandleBasedMemoryPool@@QAEXJ@Z`, engine copy 0x00A02B30 landed) on this library's pool (vtable
//   0x012B4098, owner decision 2026-10-31): no critical section, no Locked clear, and the neighbour merge is the pool's
//   CombineAdjacentAreas instead of two CombineAreas calls. The catalog row names the engine class; this address is the
//   0x012B4098 copy (NAME_FIXES).
// Callees: InsertFreeArea_00c27ef0 @ 0x00C27EF0 (the landed spelling), CombineAdjacentAreas(CHeader*) @ 0x00C282E0,
//   FreeHandle(long) @ 0x00C28610 (all landed).
// Layout: this library's pool (landed MoveArea 0x00C28580 / FreeHandle 0x00C28610): the handle table is a VC7.1 Dinkumware
//   vector<CHeader*> at +0x43C (_Myfirst +0x440) after the free lists at +0x3C, so the pool is modelled locally.
// Shape: operator[] = `*(begin() + _Pos)` keeps the dead `lea eax,[eax+ebx*4]`; the sizes are read back after
//   InsertFreeArea (which folds the padding into the size); `-0x20 - Padding - Size` is one subtraction chain added to the
//   allocated size read before the count bump.
struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free, 'T' taken)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};

namespace std {
template<class _Ty> class allocator { };

template<class _Ty> class _Ptrit {              // VC7.1 Dinkumware vector iterator: a class over one pointer
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ptrit& operator+=(int _Off) { current += _Off; return (*this); }
    _Ptrit operator+(int _Off) const { _Ptrit _Tmp = *this; return (_Tmp += _Off); }
    _Ty* current;
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    iterator begin() { return (iterator(_Myfirst)); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    _Ty& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

class CHandleBasedMemoryPool_012b4098 { public:                 // this library's pool variant (see Layout)
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 0
    virtual int  CanMoveResource(void* resource, long handle);                                        // slot 1
    virtual void ValidateResource(void* resource);                                                    // slot 2
    void*                 Buffer;               // +0x04
    void*                 UnalignedBuffer;      // +0x08
    long                  PoolSize;             // +0x0c
    long                  AllocatedSize;        // +0x10
    long                  NumAllocations;       // +0x14
    long                  LockCount;            // +0x18
    long                  LargestFreeArea;      // +0x1c
    unsigned char         _pad20[0x34 - 0x20];
    void*                 m_defaultHandler;     // +0x34 embedded default handler (one vptr)
    void*                 FailureHandler;       // +0x38
    CHeader*              FreeLists[256];       // +0x3c
    std::vector<CHeader*> Handles;              // +0x43c
    long                  FirstUnusedHandle;    // +0x44c
    void InsertFreeArea_00c27ef0(CHeader* area);            // 0x00C27EF0
    CHeader* CombineAdjacentAreas(CHeader* area);           // 0x00C282E0
    void FreeHandle(long handle);                           // 0x00C28610
    void Free(long handle);
};

void CHandleBasedMemoryPool_012b4098::Free(long handle)
{
    CHeader* area = Handles[handle];
    area->Status = 'N';
    InsertFreeArea_00c27ef0(area);
    ++NumAllocations;
    AllocatedSize -= area->Padding + area->Size + 0x20;
    CombineAdjacentAreas(area);
    FreeHandle(handle);
}