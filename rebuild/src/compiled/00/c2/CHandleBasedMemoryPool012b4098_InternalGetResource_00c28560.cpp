// CHandleBasedMemoryPool_012b4098::InternalGetResource(long handle) -- the owner's resource pointer stored in the header of the area behind a handle
// Leaf: ?InternalGetResource@CHandleBasedMemoryPool_012b4098@@IAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28560, 21 bytes, thiscall `ret 4`. Not called or referenced in retail (an
//   inline-candidate member kept out of line).
// Identity: the libpk build of the engine pool; the Ego_r.pdb name set lists the protected pair
//   `InternalSetResource(long, void*)` / `InternalGetResource(long)` (?InternalSetResource@CHandleBasedMemoryPool@@IAEXJPAX@Z,
//   ?InternalGetResource@CHandleBasedMemoryPool@@IAEPAXJ@Z) between Defragment and MoveArea, the same order as retail here
//   (Defragment 0x00C28400, InternalSetResource 0x00C28540, this row, MoveArea 0x00C28580), and the arities fit (`ret 8` / `ret 4`).
// Layout: the libpk pool keeps its handles in a Dinkumware vector<CHeader*> at +0x43C (_Myfirst +0x440); modelled locally as
//   in the landed GetSizeFromHandle 0x00C283A0 (engine/CHandleBasedMemoryPool.h has the engine layout).
// Shape: the dead `lea eax,[eax+ecx*4]` after the slot load is Dinkumware's `operator[]` (`*(begin() + _Pos)` over _Ptrit).
struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader (32 bytes in front of every area)
    void*     Resource;                         // +0x00
    long      Handle;                           // +0x04
    long      Size;                             // +0x08
    short     Padding;                          // +0x0c
    unsigned char Status;                       // +0x0e
    unsigned char Locked;                       // +0x0f
    CHeader** RefAvailable;                     // +0x10
    CHeader*  NextAvailable;                    // +0x14
    CHeader*  PrevContiguous;                   // +0x18
    CHeader*  NextContiguous;                   // +0x1c
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

class CHandleBasedMemoryPool_012b4098 {                  // the libpk build
public:
    unsigned char _pad00[0x43c];
    std::vector<CHeader*> Handles;              // +0x43c (Dinkumware: _Myfirst +0x440)
protected:
    void* InternalGetResource(long handle);
};

void* CHandleBasedMemoryPool_012b4098::InternalGetResource(long handle)
{
    return Handles[handle]->Resource;
}