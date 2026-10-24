// std::__uninitialized_copy(const CCombatRing* first, const CCombatRing* last, CCombatRing* result, const __false_type&) -- copy-construct combat rings into raw storage (the vector copy-constructor copy)
// Leaf: ??$__uninitialized_copy@PBVCCombatRing@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4140, 72 bytes, `ret 8` (/Gr).
// Label correction: catalog "_Move<CMusicEntry*,CMusicEntry*>" is propagated; this is
//   std::__uninitialized_copy<const CCombatRing*, CCombatRing*> (the vector copy constructor's copy; byte-identical to
//   0x006F40A0 apart from the call displacement).
// Callees: vector<CProng>::vector(const vector&) 0x006F20A0.
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
struct __false_type { __false_type() {} };

template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    vector(const vector<T, Alloc>& x);
};
}

class CDefendingCombatantInfo {
public:
    class CProng;
    class CCombatRing {                                   // Ego_r.pdb (retail 0x1c: 12-byte STLport vector)
    public:
        float RotationAngle;                              // +0x00
        float DeltaRotation;                              // +0x04
        long Index;                                       // +0x08
        float RingThickness;                              // +0x0c
        std::vector<CProng> Prongs;                       // +0x10
    };
};

namespace std {
template<> vector<CDefendingCombatantInfo::CProng>::vector(const vector<CDefendingCombatantInfo::CProng>& x);   // 0x006F20A0
}

namespace std {
template<class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template CDefendingCombatantInfo::CCombatRing* __fastcall __uninitialized_copy(const CDefendingCombatantInfo::CCombatRing* first, const CDefendingCombatantInfo::CCombatRing* last,
                                                                               CDefendingCombatantInfo::CCombatRing* result, const __false_type&);
}