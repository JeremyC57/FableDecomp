// vector<CMapInfo>::reserve(unsigned int n) -- STLport reserve for CWorldMap's 72-byte map records: when the capacity is short, move the records into a fresh n-slot block (allocate-and-copy, destroy, free) or just allocate one
// Leaf: ?reserve@?$vector@VCMapInfo@@
//
// Retail 0x00519800, 164 bytes (+ int3 padding to 176), thiscall `ret 4`.  Callees: 0x00515020 vector<CMapInfo>::_M_allocate_and_copy(n, first,
//   last) (thiscall), 0x00515250 __destroy_aux<CMapInfo*> (landed as `_Destroy_range_CMapInfo`; /Gr __fastcall, the
//   tag on the stack), 0x00BFEA14 free, 0x00BFEA0E malloc.
// Caller: 0x0051B5B6 (CWorldMap::Maps growth, the landed CArray<CMapInfo> bodies around 0x0051B560).
// Layout: Ego_r.pdb CMapInfo, 72 bytes (Area, PMap, LevelName, LevelScriptName, MapUID, flags, SerialiseScript,
//   LevStamp, TngStamp, IsSea); only its size reaches this body (the `imul 38E38E39h / sar 4` divide by 72).
// Shape: STLport `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start,
//   _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }` -- capacity/size are
//   signed pointer differences compared unsigned; _M_clear = _Destroy(_M_start, _M_finish) (the `__false_type()` tag homed
//   in the dead n slot) + `if (_M_start) free(_M_start)`; allocate = `n ? malloc(n * 72) : 0`.
// Label: catalog "Vector_Reserve_CMapInfo".
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);

class CCharString {
public:
    const char* PString;
    ~CCharString();                            // 0x0099EAE0
};
class C2DBoxI {
public:
    long X0, Y0, X1, Y1;
};
class CMap;
template <class T> class CCountedPointer {
public:
    T*    Object;                              // +0x0
    void* Block;                               // +0x4 (count block)
    ~CCountedPointer();
};
class CMapInfo {                               // Ego_r.pdb, 72 bytes
public:
    C2DBoxI               Area;                // +0x00
    CCountedPointer<CMap> PMap;                // +0x10
    CCharString           LevelName;           // +0x18
    CCharString           LevelScriptName;     // +0x1C
    unsigned long         MapUID;              // +0x20
    bool                  Used;                // +0x24
    bool                  Placed;              // +0x25
    bool                  Activated;           // +0x26
    bool                  Loading;             // +0x27
    bool                  LoadedOnPlayerProximity;   // +0x28
    CCharString           SerialiseScript;     // +0x2C
    unsigned __int64      LevStamp;            // +0x30
    unsigned __int64      TngStamp;            // +0x38
    bool                  IsSea;               // +0x40
};

namespace std {
struct __false_type { __false_type() {} };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last, const __false_type&);   // 0x00515250
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
};
template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return unsigned int(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class _ForwardIterator>
    T* _M_allocate_and_copy(unsigned int n, _ForwardIterator first, _ForwardIterator last);   // 0x00515020
    public:
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    void reserve(unsigned int n);
};

template <class T> void vector<T>::reserve(unsigned int n)
{
    if (capacity() < n) {
        const unsigned int old_size = size();
        T* tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            tmp = this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}
}

template void std::vector<CMapInfo>::reserve(unsigned int);