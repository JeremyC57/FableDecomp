// std::_Transform(CThing** first, CThing** last, back_insert_iterator<vector<CIntelligentPointer<CThing> > > dest, CToIntelligentPointer func) -- wrap every raw thing pointer of [first, last) in an intelligent pointer and push it onto the destination vector, return the advanced back inserter
// Leaf: _Transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008099B0, length 181 bytes (+ int3 padding), `ret 0Ch`.
// Callers: CTCTargetingPlayer::PostProcessTargetedThing @ 0x00803B20 (call 0x00803BFD: `ecx` = the returned inserter's
//   slot, `edx` = first, then last, the back inserter and the one-byte functor pushed).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
//   ~CBaseIntelligentPointer @ 0x00A01C10; the vector's _M_insert_overflow @ 0x004AC940 (thiscall: position, x, the
//   __false_type tag, 1, true). Vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Identity: Ego_r.pdb names `std::_Transform<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >,
//   `anonymous namespace'::CToIntelligentPointer>` as `__fastcall` (`YI`), returning the inserter.
// Shape: `for (; _First != _Last; ++_First, ++_Dest) *_Dest = _Func(*_First); return (_Dest);` in a /Gr TU: the hidden
//   result pointer in ecx (spilled), first in edx, last/dest/func on the stack. The functor returns a CIntelligentPointer
//   built straight in the frame temporary (CBaseIntelligentPointer(), the derived vtable store, SetPItem(thing)); the
//   inserter's `container->push_back(temp)` is STLport's inline push_back -- in place through the placement-new null test
//   and the copy constructor (base ctor, vtable, `SetPItem(other.GetPItem())`), `++_M_finish`, else
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` with the tag homed in a frame byte -- then the temporary's
//   destructor. The 8-byte temporary gives VC7.1's `and esp,-8` frame.
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                 // 0x00A01B10
    virtual ~CBaseIntelligentPointer();        // 0x00A01C10
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void SetPItem(CBaseObject* item);          // 0x00A01B90
    void* PData;                               // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };

namespace std {
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                               // +0x0
    T* _M_finish;                              // +0x4
    T* _M_end_of_storage;                      // +0x8
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
template <class C> class back_insert_iterator {
public:
    C* container;
    explicit back_insert_iterator(C& cont) : container(&cont) { }
    back_insert_iterator& operator=(const typename C::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};
template <class InIt, class OutIt, class Fn1>
OutIt __fastcall _Transform(InIt _First, InIt _Last, OutIt _Dest, Fn1 _Func)
{
    for (; _First != _Last; ++_First, ++_Dest)
        *_Dest = _Func(*_First);
    return (_Dest);
}
}

namespace {
struct CToIntelligentPointer {
    CIntelligentPointer<CThing> operator()(CThing* thing) const { return CIntelligentPointer<CThing>(thing); }
};
}

template std::back_insert_iterator<std::vector<CIntelligentPointer<CThing> > > __fastcall std::_Transform(CThing**, CThing**, std::back_insert_iterator<std::vector<CIntelligentPointer<CThing> > >, CToIntelligentPointer);