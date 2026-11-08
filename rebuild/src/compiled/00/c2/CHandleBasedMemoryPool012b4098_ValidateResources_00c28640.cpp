// CHandleBasedMemoryPool_012b4098::ValidateResources() -- for every slot of the handle table that holds a taken area ('T'), hand the area's resource to the pool owner's virtual ValidateResource
// Leaf: ?ValidateResources@CHandleBasedMemoryPool_012b4098@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28640, 75 bytes, thiscall, plain `ret`. No caller in Fable.exe (a debug walk the
//   linker kept); the libpk build of the engine's CHandleBasedMemoryPool::ValidateResources (Ego_r.pdb name set
//   `?ValidateResources@CHandleBasedMemoryPool@@QAEXXZ`, engine copy 0x00A02AD0 landed), on this library's pool
//   (vtable 0x012B4098, owner decision 2026-10-31: class + vtable address).
// Callees: this pool's virtual slot 2 ValidateResource(void* resource) (+0x08; slots 0 MoveResource, 1 CanMoveResource as
//   the landed TestIfAreaIsGrowable / MoveArea rows).
// Layout: this library's pool keeps the handle table as a VC7.1 Dinkumware vector<CHeader*> at +0x43C (_Myfirst +0x440,
//   _Mylast +0x444) after the free lists at +0x3C, one word above engine/CHandleBasedMemoryPool.h (landed MoveArea
//   0x00C28580 / InternalAllocateHandle 0x00C28A80), so the pool is modelled locally.
// Shape: Dinkumware's size() is `_Myfirst == 0 ? 0 : _Mylast - _Myfirst` (the `test ecx,ecx ; jne ; xor eax,eax` head),
//   compared signed (a long index: an unsigned one folds the empty-table arm into one `je`), re-read every iteration
//   because the virtual call may change the table; the slot is subscripted three times (`Handles[index]` in the test, the
//   status test and the call argument), which is what gives retail's folded load plus the dead `lea eax,[eax+edi*4]`
//   (Dinkumware operator[] = `*(begin() + _Pos)`) and the `mov eax,ecx ; mov ecx,[eax]` resource load; a named header
//   local loads through the lea instead (72v75).
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
    void ValidateResources();
};

void CHandleBasedMemoryPool_012b4098::ValidateResources()
{
    for (long index = 0; index < (long)Handles.size(); ++index) {
        if (Handles[index] && Handles[index]->Status == 'T')
            ValidateResource(Handles[index]->Resource);
    }
}