// std::__uninitialized_fill_n(CCombatRing* first, unsigned int n, const CCombatRing& x, const __false_type&) -- copy-construct n combat rings from x into raw storage
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F40F0, 68 bytes, `ret 8` (/Gr).
// Label correction: catalog "fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>" is propagated; this is
//   std::__uninitialized_fill_n<CCombatRing*, unsigned int, CCombatRing>.
// Callees: vector<CProng>::vector(const vector&) 0x006F20A0.
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` (`test edx,edx ; jbe` for the unsigned count).
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
template<class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template CDefendingCombatantInfo::CCombatRing* __fastcall __uninitialized_fill_n(CDefendingCombatantInfo::CCombatRing* first, unsigned int n,
                                                                                 const CDefendingCombatantInfo::CCombatRing& x, const __false_type&);
}