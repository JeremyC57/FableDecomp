// NF392 probe: 00728a40 in the 00728d10 binder model (CArray<T> : vector<T>, no own special members)
// Leaf: ??$__uninitialized_fill_n@
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CEngineTextureMorph;
namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void __fastcall _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

// Same-TU stand-in bodies of the out-of-line helpers (the STLport text of 0x00728B10 / 0x00728A40 / 0x00722130): VC7.1
// learns that the empty tags are never read and homes the __false_type temporaries in the atend slot, as retail.
#pragma auto_inline(off)
template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)
template <class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};
template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;      // +0x08
};
template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    vector(const vector<T, Alloc>& x);                     // 0x005BBDF1 for T = CEngineTextureMorph
    ~vector();                                              // 0x005BC0D3 for T = CEngineTextureMorph
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
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
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
    public:
};

}

template <class T> class CArray : public std::vector<T> {   // retail: CArray<T> is an STLport vector (12 bytes); its copy is vector's
public:
};
class CEngineTextureMorphSet {                              // Ego_r.pdb (retail 12 bytes)
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
};
class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                          // Ego_r.pdb (retail 16 bytes)
        public:
            unsigned long          MeshIndex;               // +0x00
            CEngineTextureMorphSet TextureMorphs;           // +0x04
        };
    };
};

typedef CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts TMeshMorphSetParts;
template TMeshMorphSetParts* __fastcall std::__uninitialized_fill_n(TMeshMorphSetParts*, unsigned int, const TMeshMorphSetParts&, const std::__false_type&);
