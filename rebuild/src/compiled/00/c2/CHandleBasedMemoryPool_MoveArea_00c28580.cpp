// CHandleBasedMemoryPool_012b4098::MoveArea(CHeader* from, CHeader* to) -- defragment step: have the owner move the resource's bytes into the free area, cut that area down to the moved size, hand it the resource, handle and lock flag, repoint the handle table, account the padding change and free the old area
// Leaf: ?MoveArea@CHandleBasedMemoryPool_012b4098@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28580, 129 bytes (`ret 8`). The libpk build of the engine's
//   CHandleBasedMemoryPool_012b4098::MoveArea (engine copy 0x00A028F0; Ego_r.pdb name set `MoveArea(CHeader*, CHeader*)` and the
//   virtual `MoveResource(void*, long, void*, void*, unsigned long)`); the manifest's "Thiscall_BlockDefragmentCopy" is
//   propagated.
// Callees: the pool's virtual slot 0 MoveResource(resource, handle, from data, to data, size), RemoveFreeArea
//   @ 0x00C27F50, ResizeArea @ 0x00C28290, InsertFreeArea @ 0x00C27EF0.
// Layout: this library's pool keeps the handle table as a VC7.1 Dinkumware vector<CHeader*> at +0x43C (_Myfirst
//   +0x440, as Lock 0x00C283C0 reads it), after free lists that start at +0x3C -- one word above
//   engine/CHandleBasedMemoryPool.h -- so the pool is modelled locally.
// Shape: the table store goes through Dinkumware's `operator[]` = `*(begin() + _Pos)` with the _Ptrit
//   `operator+` built on a `_Tmp += _Off` copy: that keeps the slot address in its own register
//   (`lea eax,[edx+ecx*4] ; mov [eax],ebx`); a plain `_Myfirst[_Pos]` folds into `mov [edx+ecx*4],ebx`. The handle
//   is re-read after the header copy (the stores through `to` may alias `from`).
struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area (landed 0x00C27F50 names)
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10 (the free-list link that points at this area)
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
    _Ty& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

class CHandleBasedMemoryPool_012b4098 { public:                 // this library's pool variant (see Layout)
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 0
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
    void RemoveFreeArea(CHeader* area);         // 0x00C27F50
    void ResizeArea(CHeader* area, long newSize);   // 0x00C28290
    void InsertFreeArea(CHeader* area);         // 0x00C27EF0
    void MoveArea(CHeader* from, CHeader* to);
};

void CHandleBasedMemoryPool_012b4098::MoveArea(CHeader* from, CHeader* to)
{
    long size = from->Size;
    MoveResource(from->Resource, from->Handle, (char*)from + sizeof(CHeader), (char*)to + sizeof(CHeader), size);
    RemoveFreeArea(to);
    ResizeArea(to, size);
    to->Resource = from->Resource;
    to->Status = from->Status;
    to->Handle = from->Handle;
    to->Locked = from->Locked;
    Handles[from->Handle] = to;
    AllocatedSize += to->Padding - from->Padding;
    from->Status = 'N';
    InsertFreeArea(from);
}