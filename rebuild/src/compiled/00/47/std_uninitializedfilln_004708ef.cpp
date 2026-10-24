// std::uninitialized_fill_n(pair<unsigned long, CSoundPair>* first, unsigned int n, const pair<unsigned long, CSoundPair>& x) -- STLport: copy-construct n copies of x from first on, return the end
// Leaf: ??$uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x004708EF, 41 bytes, `ret 4` (__fastcall: first in ecx, n in edx, x on the stack).
// Identity: the catalog's "_Fill_n<unsigned long*, ...>" is propagated. The single caller is vector::_M_fill_insert
//   @ 0x00470778 (stride 12), reached through vector::resize(n, x) @ 0x004706F6 from CSoundMap::TransferBinaryIn
//   @ 0x0047042A: the vector is CSoundMap::SoundMap's CVectorMap<unsigned long, CSoundPair> storage, element
//   pair<unsigned long, CSoundPair> (Ego_r.pdb CSoundPair = {long FirstSound, long LastSound}; the landed
//   CSoundMap::TransferBinaryOut 0x0047034B writes Key, FirstSound, LastSound).
// Model: STLport 4.x `uninitialized_fill_n(first, n, x)` with the `__uninitialized_fill_n(.., __false_type)` loop and
//   _Construct inlined: `for (; n > 0; --n, ++cur) new (&*cur) T(x)` -- the placement-new null test, then pair's
//   member-wise copy constructor (three dword moves through esi). Exceptions off (no unwind frame).
// Shape: size TU (#pragma optimize("s", on)), no frame pointer; the unsigned `n > 0` test is the `jbe`.
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

struct CSoundPair {
    long FirstSound;                                // +0x00
    long LastSound;                                 // +0x04
};

namespace std {
struct __false_type { };

template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    __forceinline pair(const pair<T1, T2>& other) : first(other.first), second(other.second) { }
};

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template _SoundEntry* __fastcall uninitialized_fill_n(_SoundEntry* first, unsigned int n, const _SoundEntry& x);
}