// CHandleBasedMemoryPool::ValidateResources() -- let the pool validate the resource of every taken area (status 'T') through its ValidateResource virtual
// Leaf: ?ValidateResources@CHandleBasedMemoryPool@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28640, 75 bytes, thiscall, `ret`.
// Identity: the libpk build of the engine's pool (landed 0x00C283A0 / 0x00C283C0 / ctor 0x00C28AE0, vtable 0x012B4098).
//   Ego_r.pdb ?ValidateResources@CHandleBasedMemoryPool@@QAEXXZ; the engine build's own copy 0x00A02AD0 has the same body
//   (walk the handle table, `cmp byte [h+0Eh],54h`, call ValidateResource = engine vtable 0x0129C964 slot 3).
//   The libpk vtable has no destructor slot: 0 MoveResource 0x00C28220 (landed), 1 CanMoveResource 0x00C29CD0
//   (`mov eax,1 ; ret 8`), 2 ValidateResource 0x00C29CE0 (`ret 4`), the FableWin.pdb protected virtuals
//   ?CanMoveResource@CHandleBasedMemoryPool@@MAE_NPAXJ@Z / ?ValidateResource@CHandleBasedMemoryPool@@MAEXPAX@Z.
// Layout: libpk is Dinkumware: the handle vector<CHeader*> at +0x43C (_Myfirst +0x440, _Mylast +0x444).
// Shape: Dinkumware size() (`_Myfirst == 0 ? 0 : _Mylast - _Myfirst`) re-evaluated at the loop top; the signed `jge`
//   is a long counter against `(long)size()`; `operator[]` leaves retail's dead `lea`.
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
    void ValidateResources();
};

void CHandleBasedMemoryPool::ValidateResources()
{
    for (long i = 0; i < (long)Handles.size(); ++i) {
        if (Handles[i] && Handles[i]->Status == 'T')
            ValidateResource(Handles[i]->Resource);
    }
}
