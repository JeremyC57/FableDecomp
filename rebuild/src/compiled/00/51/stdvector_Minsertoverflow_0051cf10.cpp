// std::vector::_M_insert_overflow(CRegion* position, const CRegion& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of the world map's region vector: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail unless at the end, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051CF10, 288 bytes (0x11A + int3 padding), thiscall `ret 14h`.
// Identity: the 0x58-byte element copies through CRegion::CRegion(const CRegion&) 0x0051A900 and dies through
//   CRegion::~CRegion() 0x0051A110 (both landed); 12 callers (vector<CRegion>::push_back / insert sites in CWorldMap
//   0x0051D1B4 / 0x0051D232 and the map UI 0x0060CA62..0x00621568). The catalog label "VectorReallocateWithRange" is
//   descriptive.
// Callees: malloc 0x00BFEA0E (inline allocate), CRegion copy constructor 0x0051A900 (head copy, single insert and
//   fill loop), __uninitialized_copy<CRegion*, CRegion*> 0x0051B420 (__fastcall, the tail copy), ~CRegion 0x0051A110
//   (the inline _M_clear loop), free 0x00BFEA14.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim with every helper a plain inline template (the 0x0051C110 relative
//   needed noinline bodies because there all three helpers stay out of line): VC7.1 itself inlines the head
//   __uninitialized_copy, the fill_n loop and __destroy_aux and keeps only the second __uninitialized_copy out of
//   line, which then needs `__fastcall` on the template (the retail TU is /Gr; a default-cdecl template pushes four
//   arguments). max takes const references (`lea ecx,[esp+1Ch] ; jb ; lea ecx,[esp+8]`), new_start is homed in the
//   fill_len slot and the tail copy's `__false_type()` in the atend slot.
// Layout: CRegion is modelled with the retail offsets of the landed copy constructor 0x0051A900 (engine/CRegion.h
//   carries the Ego_r 16-byte vectors, so it is not used); only sizeof (0x58) and the two out-of-line special members
//   matter to this body.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CCharString { public: void* PStringData; };
class C2DVector { public: float X, Y; };
template <class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; };
struct CRetailMapHeader { void* m_header; unsigned long m_node_count; unsigned long m_compare; };
struct CDefPointerRaw { void* Object; };

class CRegion {                                 // retail 0x58 bytes (see 0x0051A900)
public:
    CRetailVector<long> Maps;                   // +0x00
    CRetailVector<long> VisMaps;                // +0x0c
    CCharString Name;                           // +0x18
    CCharString DisplayName;                    // +0x1c
    CCharString RegionDefName;                  // +0x20
    CDefPointerRaw PDef;                        // +0x24 CDefPointer<const CRegionDef>
    CCharString MiniMapGraphic;                 // +0x28
    float MiniMapScale;                         // +0x2c
    C2DVector MiniMapOffset;                    // +0x30
    CRetailMapHeader MiniMapRegionExitTextOffsets;   // +0x38 map<CCharString, C2DVector>
    C2DVector WorldMapOffset;                   // +0x44
    C2DVector NameGraphicOffset;                // +0x4c
    bool AppearOnWorldMap;                      // +0x54
    bool CreatureGenerationEnabled;             // +0x55
    bool SoundThemesEnabled;                    // +0x56
    CRegion(const CRegion& other);              // 0x0051A900
    ~CRegion();                                 // 0x0051A110
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                  const __false_type&)   // 0x0051B420 when not inlined
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef T* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer s, pointer f, pointer e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend);
    public:
};

template <class T, class A>
void vector<T, A>::_M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend)
{
    const size_type old_size = size();
    const size_type len = old_size + (max)(old_size, fill_len);
    pointer new_start = this->_M_end_of_storage.allocate(len);
    pointer new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
}

template void std::vector<CRegion>::_M_insert_overflow(CRegion*, const CRegion&, const std::__false_type&,
                                                        unsigned int, bool);