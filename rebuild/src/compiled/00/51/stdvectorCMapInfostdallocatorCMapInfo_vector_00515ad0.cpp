// std::vector<CMapInfo,std::allocator<CMapInfo> >::vector(unsigned int n) -- STLport vector(n) of the world map's 0x48-byte map records: allocate n records, copy-construct each from a default (cleared) record, destroy the default record
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515AD0, 213 bytes, `ret 4` (thiscall: n).
// Callers: 0x00506740 and 0x0051A1B8 (CWorldMap's Maps vector sized from the map count). The catalog's
//   "CWorldMap_Constructor" is descriptive: `this` is the vector, not the world map.
// Element: Ego_r.pdb CMapInfo, 0x48 bytes (PDB = retail, as the landed CWorldMap rows): Area (C2DBoxI) +0, PMap
//   (CCountedPointer<CMap>) +0x10, LevelName +0x18, LevelScriptName +0x1C, MapUID +0x20, five bools +0x24..+0x28,
//   SerialiseScript +0x2C, LevStamp +0x30, TngStamp +0x38, IsSea +0x40. Its default constructor zeroes the counted
//   pointer inline, default-constructs the three strings out of line and then calls Clear() in the body.
// Callees: malloc @ 0x00BFEA0E; CCharString::CCharString() @ 0x0099E4B0 (x3); CMapInfo::Clear() @ 0x004FDDE0;
//   CMapInfo::CMapInfo(const CMapInfo&) @ 0x005110B0; CCharString::~CCharString() @ 0x0099EAE0 (x3); the counted
//   block's destroy function (`mov ecx,[blk+8] ; call [blk+4]`); operator delete @ 0x00BFE9BC.
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` with `__uninitialized_fill_n(.., __false_type)` inlined here (the
//   placement-new null test, the out-of-line copy constructor, `_M_start` re-read after the Clear call); the default
//   record fills the 0x48-byte frame and its implicit destructor runs inline (strings last member first, then the
//   counted release re-reading its block after the decrement; the dead clears of a dying temporary are dropped).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void __cdecl operator delete(void* block);             // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    ~CCharString();                                 // 0x0099EAE0
};

struct SCountBlock {
    long  Count;                                    // +0
    void (__fastcall* Destroy)(void* object);       // +4
    void* Object;                                   // +8
};

template <class T> class CCountedPointer {
public:
    T*           Object;                            // +0
    SCountBlock* Info;                              // +4
    CCountedPointer() : Object(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
        Object = 0;
        Info = 0;
    }
};

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI() { }
};

class CMap;

class CMapInfo {                                    // Ego_r.pdb, 0x48 bytes
public:
    C2DBoxI               Area;                     // +0x00
    CCountedPointer<CMap> PMap;                     // +0x10
    CCharString           LevelName;                // +0x18
    CCharString           LevelScriptName;          // +0x1C
    unsigned long         MapUID;                   // +0x20
    bool                  Used;                     // +0x24
    bool                  Placed;                   // +0x25
    bool                  Activated;                // +0x26
    bool                  Loading;                  // +0x27
    bool                  LoadedOnPlayerProximity;  // +0x28
    CCharString           SerialiseScript;          // +0x2C
    unsigned __int64      LevStamp;                 // +0x30
    unsigned __int64      TngStamp;                 // +0x38
    bool                  IsSea;                    // +0x40
    CMapInfo() { Clear(); }
    CMapInfo(const CMapInfo& other);                // 0x005110B0
    void Clear();                                   // 0x004FDDE0
};

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template class vector<CMapInfo, allocator<CMapInfo> >;

}