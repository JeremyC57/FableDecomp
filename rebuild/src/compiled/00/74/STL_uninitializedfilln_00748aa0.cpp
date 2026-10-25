// std::__uninitialized_fill_n(pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, unsigned int n, const pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>& x, const __false_type&) -- copy-construct n copies of one danger-thing entry into raw storage; returns the end of the copies
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748AA0, 74 bytes (+6 int3), `ret 8` (link-time __fastcall: first in ecx, n in edx,
//   the value reference and the __false_type reference on the stack).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90
//   (the inlined CIntelligentPointer<const CThing> copy constructor, vtable 0x01237F1C).
// Caller: the DangerThings vector's _M_insert_overflow 0x00749420 (0x007494C4: the fill of the inserted run).
// Label: the manifest's "CVector_Construct_Range" is descriptive; the body is STLport `__uninitialized_fill_n(first, n,
//   x, __false_type)` over CTCStealth's 16-byte danger-thing pairs.
// Shape: `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- n is the unsigned size_type (`test edx,edx ;
//   jbe`, then `dec ebx ; jne`), placement new tests the destination, the pair copy constructor is inlined.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CThing;

class CBaseIntelligentPointer {
public:
    void* PData;                                         // +0x04 (CBaseObjectPointer*)
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    void* GetPItem() const;                              // 0x00A01B50
    void SetPItem(const void* item);                     // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    virtual void v00();                                  // own vtable (CIntelligentPointer<const CThing> 0x01237F1C)
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                                    // Ego_r.pdb 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long          Timer;                                 // +0x04
};
}

namespace std {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;   // 16 bytes

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template TDangerPair* __fastcall __uninitialized_fill_n(TDangerPair*, unsigned int, const TDangerPair&, const __false_type&);
}