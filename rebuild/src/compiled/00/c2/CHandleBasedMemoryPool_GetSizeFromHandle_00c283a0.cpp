// CHandleBasedMemoryPool_012b4098::GetSizeFromHandle(long handle) -- the size of the area behind a handle (its header's Size, +8)
// Leaf: ?GetSizeFromHandle@CHandleBasedMemoryPool_012b4098@@QAEKJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C283A0, 22 bytes, thiscall, `ret 4`.
// Identity: the libpk build of the engine's pool (see 0x00C280E0); Ego_r.pdb names
//   ?GetSizeFromHandle@CHandleBasedMemoryPool_012b4098@@QAEKJ@Z (donor 0x00B6D600, after GetPointerFromHandle 0x00B6D5E0), and
//   the libpk Lock 0x00C283C0 / Unlock 0x00C283E0 that follow this row read the same handle table. Only caller
//   0x00C221A4 (the sound-event set, on its pool at +0x334).
// Layout: the libpk pool keeps its handles in a Dinkumware vector<CHeader*> at +0x43C (_Myfirst +0x440, landed Lock
//   0x00C283C0); libpk is Dinkumware, so `this` is modelled locally (engine/CHandleBasedMemoryPool.h has the engine layout).
// Shape: retail forms the slot address (`lea eax,[eax+ecx*4]`) after the load through it and never uses it: that dead
//   `lea` is the Dinkumware `operator[]` (`*(begin() + _Pos)` over the _Ptrit iterator class). A raw `CHeader**` table
//   indexed directly, or through a slot pointer/reference, drops it (19v22).
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
    unsigned long GetSizeFromHandle(long handle);
};

unsigned long CHandleBasedMemoryPool_012b4098::GetSizeFromHandle(long handle)
{
    return Handles[handle]->Size;
}