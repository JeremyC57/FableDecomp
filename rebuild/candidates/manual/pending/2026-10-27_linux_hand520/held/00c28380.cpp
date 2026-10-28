// CHandleBasedMemoryPool::GetPointerFromHandle(long handle) -- the start of the area behind a handle (just past its 32-byte header)
// Leaf: ?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28380, 21 bytes, thiscall, `ret 4`.
// Identity: the libpk build of the engine's pool (landed siblings 0x00C283A0 GetSizeFromHandle, 0x00C283C0 Lock, ctor
//   0x00C28AE0 storing vtable 0x012B4098). Ego_r.pdb ?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z (donor
//   0x00B6D5E0, directly before GetSizeFromHandle 0x00B6D600: retail 0x00C28380 directly before 0x00C283A0); the engine
//   build's own copy is 0x00A026B0 (same body over its STLport handle array at +0x438).
//   Label correction: the binding "?Compute@SubObjectAt360@@QAEHH@Z" (caller 0x00C26240) is invented.
// Layout: libpk is Dinkumware: the handle vector<CHeader*> sits at +0x43C (_Myfirst +0x440), so `this` is modelled
//   locally (engine/CHandleBasedMemoryPool.h is the engine build's layout; Header debt in NOTES.md).
// Shape: the landed GetSizeFromHandle model (Dinkumware `operator[]` = `*(begin() + _Pos)`).
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
    void* GetPointerFromHandle(long handle);
};

void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle)
{
    return Handles[handle] + 1;
}
