// _STL::vector<CProng>::vector(const vector& x) -- STLport vector copy constructor (element CDefendingCombatantInfo::CProng) for a combat ring's 28-byte prongs: allocate exactly x.size() elements (none when empty), then __uninitialized_copy x's range into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F20A0, 104 bytes (112 with int3 padding), thiscall, `ret 4`, returns this.
// Identity: the copy routine is the landed __uninitialized_copy<CDefendingCombatantInfo::CProng*> 0x006F14A0, so the
//   element is CProng (Ego_r.pdb, 28 bytes) and this is the vector<CProng> copy constructor (CCombatRing's Prongs
//   member +0x10). Callers: 0x006F3A24, 0x006F4091, 0x006F40D1, 0x006F4121, 0x006F4171, 0x006F47E0, 0x006F4960.
//   Label correction: the catalog's "vector_copy_CTargetPoint" is propagated (as 0x006F14A0's was).
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`); __uninitialized_copy(const CProng*, const CProng*, CProng*,
//   const __false_type&) 0x006F14A0 (`__fastcall`: first ecx / last edx, result + tag on the stack).
// Shape: as the landed 0x004D08A0 (28-byte pair) copy constructor: x.size() is the 0x92492493 reciprocal divide, the
//   zero register is loaded with `mov eax,0` so the `n != 0` flags of the divide survive for `allocate(n)`'s test;
//   the empty `__false_type()` tag is homed in the dead `x` argument slot (`lea ecx,[esp+10h]`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBaseObject;
struct CThingCreatureBase;

struct CBaseIntelligentPointer {
    void* PData;                                            // +0x04
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };

#pragma pack(push, 1)
struct C3DVector {                                          // 1-aligned in retail (CProng::Prong sits at +1)
    float X, Y, Z;
};
#pragma pack(pop)

struct CProng {                                             // CDefendingCombatantInfo::CProng, 0x1c bytes
    bool                                    Blocked;              // +0x00
    C3DVector                               Prong;                // +0x01
    long                                    ProngBlockageTimer;   // +0x10
    CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
};

namespace _STL {

struct __false_type { __false_type() { } };

CProng* __fastcall __uninitialized_copy(const CProng* first, const CProng* last, CProng* result,
                                        const __false_type&);   // 0x006F14A0

template <class T> struct allocator {                       // STLport allocator<T>: empty
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> struct _Vector_base {
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> struct vector : _Vector_base<T> {
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start,
                                               __false_type());
    }
};

template struct vector<CProng>;
}