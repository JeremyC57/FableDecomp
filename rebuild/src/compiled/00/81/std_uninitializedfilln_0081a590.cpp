// std::uninitialized_fill_n(std::pair<unsigned long,CSoundPair>* first, unsigned int n, const std::pair<unsigned long,CSoundPair>& x) -- STLport placement-fill of n uninitialised 12-byte sound-pair map entries with copies of x, returning the end
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe @ 0x0081A590, 48 bytes, `ret 4`: __fastcall (the algorithm TUs are /Gr), first in ecx, the count in
// edx (unsigned: `test edx,edx ; jbe`), the value reference on the stack; returns the end pointer in eax.
// Caller: 0x0081CF18, the vector fill-insert 0x0081CE50 (`uninitialized_fill_n(old_finish, n - elems_after, x_copy)`;
// its sibling fill call 0x008192B0 is catalogued `_Fill<pair<unsigned long,CSoundPair>*,...>`).
// Label: catalogued `_Fill_n<unsigned long*,unsigned int,unsigned long>`, a propagated name: the stride here is 12, not 4.
// The 12-byte element is modelled as STLport pair<unsigned long,CSoundPair> (Ego_r.pdb CSoundPair: long FirstSound,
// long LastSound), as the sibling label names it; a 3-dword array member instead copies through esi/edi (measured), so
// the record is three scalar dwords copied memberwise.
// Shape: STLport `uninitialized_fill_n` -> `__uninitialized_fill_n` with the non-POD loop `for (; n > 0; --n, ++cur)
// _Construct(&*cur, x); return cur;` -- the placement new is the throw() form, so every slot is null-tested.
inline void* operator new(unsigned int, void* where) throw() { return where; }
class CSoundPair {
public:
    long FirstSound;                              // +0x0
    long LastSound;                               // +0x4
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};
}
typedef std::pair<unsigned long, CSoundPair> SElement12;
namespace std {
template <class T1, class T2>
inline void _Construct(T1* p, const T2& val) { new (p) T1(val); }

template <class ForwardIter, class Size, class Tp>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const Tp& x)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template SElement12* __fastcall uninitialized_fill_n(SElement12*, unsigned int, const SElement12&);
}