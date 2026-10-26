// std::__destroy_aux(std::pair<unsigned long, CArrayHolder>* first, std::pair<unsigned long, CArrayHolder>* last, const __false_type&) -- destroy a range of {key, CBaseClass-derived array holder} entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5E900, length 55 bytes.
// Leaf: __destroy_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The stride is 0x14; the
// inlined element destructor frees a vector block at +0x08 and then calls 0x0099A2E0 on +0x04, which only stores vtable
// 0x01231710 = CBaseClass (rtti-vtables): the value is a CBaseClass-derived class holding one CArray, destroyed by its
// compiler-generated destructor (which drops its own vtable reset). 25 callers (CPatchTesselationEdgeStrip area,
// 0x00A91074 ...); the value class is not identified, so it is modelled as CArrayHolder.
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: free @ 0x00BFEA14 (vector block), CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: as 0x009B8070 -- the real STLport ~vector / ~_Vector_base chain gives the strength-reduced element pointer
// (`lea esi,[edi+8]`, the base destructor at `[esi-4]`).
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
class CBaseClass {
public:
    virtual ~CBaseClass();                  // 0x0099A2E0 (stores the CBaseClass vtable 0x01231710)
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class T> inline void __destroy_trivial(T*, T*, const __true_type&) { }
template <class T> inline void _Destroy(T* first, T* last) { __destroy_trivial(first, last, __true_type()); }
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class T> class CArray : public std::vector<T> { public: };
class CArrayHolder : public CBaseClass {    // 16 bytes: vptr +0, array +4
public:
    CArray<long> m_array;
};
typedef std::pair<unsigned long, CArrayHolder> SArrayHolderEntry;

namespace std {
void __fastcall __destroy_aux(SArrayHolderEntry* first, SArrayHolderEntry* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}