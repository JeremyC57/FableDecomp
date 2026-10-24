// std::vector<CCombatRing>::push_back(const CCombatRing& x) -- construct at finish, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4930, 87 bytes, `ret 4`.
// Label correction: catalog "CDefendingCombatantInfo_PushBackProng" -> STLport vector<CCombatRing>::push_back.
// Callees: vector<CProng>::vector(const vector&) 0x006F20A0, _M_insert_overflow 0x006F4750 -- given a same-TU noinline
//   STAND-IN body here (not the real grow code; only its call matters) so the empty __false_type temporary shares x's
//   argument slot as in retail.
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
#pragma auto_inline(off)
    // 0x006F4750 (separate function; same-TU stand-in so the empty tag can share x's slot)
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        if (atend && fill_len == 1 && position == _M_finish)
            _Construct(_M_start, x);
    }
#pragma auto_inline(on)
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

template void std::vector<CDefendingCombatantInfo::CCombatRing>::push_back(const CDefendingCombatantInfo::CCombatRing& x);