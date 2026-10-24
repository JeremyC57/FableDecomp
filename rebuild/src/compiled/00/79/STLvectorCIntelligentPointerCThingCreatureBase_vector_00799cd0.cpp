// _STL::vector<CIntelligentPointer<CThingCreatureBase> >::vector(unsigned int n) -- construct a vector of n null intelligent creature pointers
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799CD0, 110 bytes (+ int3 padding to 112), __fastcall `ret 4`, returns this.
// Identity: the element is the 8-byte CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7D8, as the landed
//   CIntelligentPointer<CThingCreatureBase> constructor 0x0078E5B0 stores); the fill callee 0x0074CD10 is STLport
//   `__uninitialized_fill_n` for that element (`ret 8`, /Gr: first in ecx, n in edx). No direct callers (a data-referenced
//   or tail-called instantiation in the CTCUnholyPower TU).
// Callees: malloc 0x00BFEA0E, CBaseIntelligentPointer() 0x00A01B10, __uninitialized_fill_n 0x0074CD10,
//   ~CBaseIntelligentPointer 0x00A01C10.
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(
//   _M_start, n, _Tp()); }` -- the base zeroes the three words, `allocate(n)` is `n != 0 ? malloc(n * 8) : 0` (the zero
//   register doubles as the null result), end_of_storage = start + n; the default element temporary is built in the
//   frame and destroyed after the fill, the front end's empty `__false_type` tag is homed in the dead `n` slot. The
//   allocator must be STLport's `_STLP_alloc_proxy` member (end_of_storage + allocate on it): a free allocator temporary
//   schedules `mov esi,ecx` after the `cmp`.
// Label correction: catalogued as `Vector_Initialize_WithIntelligentPtr`; the body is the vector's size constructor.
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E

class CBaseObject;
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct __false_type { __false_type() { } };

typedef CIntelligentPointer<CThingCreatureBase> CCreaturePointer;

CCreaturePointer* __fastcall __uninitialized_fill_n(CCreaturePointer* first, unsigned int n, const CCreaturePointer& x,
                                                    const __false_type&);   // 0x0074CD10

template <class T> inline T* uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> struct allocator {
    T* allocate(unsigned int n) const { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {   // STLport: the allocator carrying end_of_storage
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>&, T* p) : _M_data(p) { }
};

template <class T> struct _Vector_base {
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> struct vector : _Vector_base<T> {
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template struct vector<CCreaturePointer>;