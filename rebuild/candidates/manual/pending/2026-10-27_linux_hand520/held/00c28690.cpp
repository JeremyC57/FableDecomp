// CHandleBasedMemoryPool::Free(long handle) -- release the area behind a handle: mark it free, put it on its free list, account for it, merge it with free neighbours and recycle the handle
// Leaf: ?Free@CHandleBasedMemoryPool@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28690, 82 bytes, thiscall, `ret 4`.
// Identity: the libpk build of the engine's pool. Ego_r.pdb donor order MoveArea 0x00B6D830, InternalFreeHandle
//   0x00B6D910, ValidateResources 0x00B6DA30, Free 0x00B6DA90, GrowFreeArea 0x00B6DB90: retail MoveArea 0x00C28580,
//   InternalFreeHandle 0x00C28610, ValidateResources 0x00C28640, this row, 0x00C286F0. The engine copy 0x00A02B30 is the
//   same body plus its critical section and Locked reset. Name fix: the caller 0x00C265B0 binds "FreeAllocation".
// Callees: InsertFreeArea 0x00C27EF0, CombineAdjacentAreas 0x00C282E0 (the libpk copy returns the surviving area),
//   InternalFreeHandle 0x00C28610.
// Layout: libpk is Dinkumware: the handle vector<CHeader*> at +0x43C (_Myfirst +0x440).
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
    typedef unsigned int size_type;
    iterator begin() { return (iterator(_Myfirst)); }
    size_type size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    _Ty& operator[](size_type _Pos) { return (*(begin() + _Pos)); }
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

class CHandleBasedMemoryPool {                  // the libpk build (vtable 0x012B4098, no RTTI)
public:
    class CHeader {                             // 32 bytes in front of every area (Ego_r.pdb layout)
    public:
        void*         Resource;                 // +0x00
        long          Handle;                   // +0x04
        long          Size;                     // +0x08
        short         Padding;                  // +0x0c
        unsigned char Status;                   // +0x0e ('T' taken, 'N' free)
        unsigned char Locked;                   // +0x0f
        CHeader**     RefAvailable;             // +0x10
        CHeader*      NextAvailable;            // +0x14
        CHeader*      PrevContiguous;           // +0x18
        CHeader*      NextContiguous;           // +0x1c
    };
protected:
    virtual void MoveResource(void* resource, long handle, void* source, void* destination, unsigned long size);   // slot 0, 0x00C28220
    virtual bool CanMoveResource(void* resource, long handle);  // slot 1, 0x00C29CD0
    virtual void ValidateResource(void* resource);              // slot 2, 0x00C29CE0
public:
    void*         Buffer;                       // +0x04
    void*         UnalignedBuffer;              // +0x08
    long          PoolSize;                     // +0x0c
    long          AllocatedSize;                // +0x10
    long          NumAllocations;               // +0x14
    long          LockCount;                    // +0x18
    long          LargestFreeArea;              // +0x1c
    unsigned char _pad20[0x43c - 0x20];         // +0x20 (defrag words, the embedded failure handler +0x34, free lists +0x3c)
    std::vector<CHeader*> Handles;              // +0x43c (Dinkumware: _Myfirst +0x440)
    long          FirstUnusedHandle;            // +0x44c
    void Free(long handle);
private:
    void InsertFreeArea(CHeader* area);                 // 0x00C27EF0
    CHeader* CombineAdjacentAreas(CHeader* area);       // 0x00C282E0
    void InternalFreeHandle(long handle);               // 0x00C28610
};

void CHandleBasedMemoryPool::Free(long handle)
{
    CHeader* header = Handles[handle];
    header->Status = 'N';
    InsertFreeArea(header);
    NumAllocations++;
    AllocatedSize -= header->Size + header->Padding + sizeof(CHeader);
    CombineAdjacentAreas(header);
    InternalFreeHandle(handle);
}
