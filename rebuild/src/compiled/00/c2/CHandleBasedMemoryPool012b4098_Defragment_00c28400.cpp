// CHandleBasedMemoryPool_012b4098::Defragment() -- full defragment of the libpk pool: unless it is locked, slide every allocated area down to the next free offset (moving its resource bytes through the virtual MoveResource and re-pointing its handle), rebuild the contiguous chain, clear the free lists, and turn the tail of the buffer into one free area
// Leaf: ?Defragment@CHandleBasedMemoryPool_012b4098@@QAEXXZ
// Flags:
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28400, 310 bytes, thiscall `ret`. The oracle's 373-byte extent also holds two
//   handle-table accessors, 0x00C28540 (`Handles[handle]->Resource = resource`, `ret 8`) and 0x00C28560 (`return
//   Handles[handle]->Resource`, `ret 4`), neither called nor referenced: function starts to add to the manifest (graded
//   against the first 310 bytes).
// Identity: the libpk build of the engine's CHandleBasedMemoryPool::Defragment (Ego_r.pdb `?Defragment@CHandleBasedMemoryPool@@QAEXXZ`,
//   engine copy 0x00A02730, the same walk) on this library's pool (vtable 0x012B4098, owner decision 2026-10-31): no critical
//   section, no shutdown / enable flags, the free lists cleared by ClearFreeLists 0x00C28200 and the tail free area made by
//   CreateArea 0x00C27EA0 + InsertFreeArea 0x00C27EF0. Callers: Allocate 0x00C28C20 (landed, binds this spelling).
// Callees: the virtual MoveResource(void*, long, void*, void*, unsigned long) (slot 0 of this copy's vtable),
//   ClearFreeLists @ 0x00C28200, CreateArea(void*, unsigned long, CHeader*, CHeader*) @ 0x00C27EA0, InsertFreeArea(CHeader*)
//   @ 0x00C27EF0 (the landed spelling InsertFreeArea_00c27ef0).
// Layout: the landed Allocate / MoveArea / Free model (Buffer +4, PoolSize +0xc, AllocatedSize +0x10, LockCount +0x18,
//   free lists +0x3c, the Dinkumware handle vector at +0x43c whose _Myfirst is +0x440).
// Shape: the handle table is a Dinkumware vector<CHeader*>; operator[] = `*(begin() + _Pos)` over the one-pointer iterator,
//   which VC7.1 keeps as `lea edx,[handle*4] ; add` (and a dead add after the second store); the running offset lives in ebx
//   and the next header in ebp, both spilled around the move (whose resource and handle take those registers); `prev` is
//   memory-homed; the header fields are written in retail's order. The tail free area's address is a named local inside the
//   size test: as a c2 candidate it takes eax ahead of `remaining` (ecx), and the reloaded `prev` takes edx -- retail's
//   registers; passed as an expression it is a rotation temporary and the three registers come out rotated by one.
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
    void ClearFreeLists();                                  // 0x00C28200
    CHeader* CreateArea(void* at, unsigned long size, CHeader* prev, CHeader* next);   // 0x00C27EA0
    void InsertFreeArea_00c27ef0(CHeader* area);            // 0x00C27EF0
    void Defragment();
};

void CHandleBasedMemoryPool_012b4098::Defragment()
{
    if (LockCount > 0)
        return;
    CHeader* prev = 0;
    unsigned long offset = 0;
    CHeader* area = (CHeader*)Buffer;
    while (area) {
        CHeader* next = area->NextContiguous;
        if (!area->RefAvailable) {
            CHeader* dest = (CHeader*)((char*)Buffer + offset);
            long size = area->Size;
            unsigned char locked = area->Locked;
            if (area != dest || area->Padding != 0) {
                long handle = area->Handle;
                void* resource = area->Resource;
                Handles[handle] = 0;
                MoveResource(resource, handle, area + 1, dest + 1, size);
                Handles[handle] = dest;
                dest->Resource = resource;
                dest->Handle = handle;
                dest->Size = size;
                dest->Padding = 0;
                dest->Status = 'T';
                dest->Locked = locked;
                dest->RefAvailable = 0;
                dest->NextAvailable = 0;
                dest->PrevContiguous = prev;
                dest->NextContiguous = 0;
                if (prev)
                    prev->NextContiguous = dest;
                prev = dest;
            }
            offset += size + sizeof(CHeader);
        }
        area = next;
    }
    AllocatedSize = offset;
    ClearFreeLists();
    long remaining = PoolSize - offset;
    if (remaining > (long)sizeof(CHeader)) {
        CHeader* tail = (CHeader*)((char*)Buffer + offset);
        InsertFreeArea_00c27ef0(CreateArea(tail, remaining, prev, 0));
    }
}