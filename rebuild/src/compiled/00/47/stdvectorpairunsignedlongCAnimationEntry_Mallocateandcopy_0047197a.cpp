// std::vector<pair<unsigned long, CAnimationEntry> >::_M_allocate_and_copy(unsigned int n, const pair<unsigned long, CAnimationEntry>* first, const pair<unsigned long, CAnimationEntry>* last) -- STLport: malloc n 20-byte animation-map entries (none for zero), copy-construct [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047197A, 84 bytes, `ret 0Ch` (thiscall: n, first, last; the empty allocator `this` is unused).
// Identity: the catalog's "AnimationVector_AllocCopy" is behavioural. The single caller is the vector operator= @ 0x00471865
//   (catalog "CAnimationSet::AssignAnimations") of CAnimationSet::Anims, a CVectorMap<unsigned long, CAnimationEntry>
//   (Ego_r.pdb): element pair<unsigned long, CAnimationEntry>, 4 + 16 bytes.
// Callees: malloc @ 0x00BFEA0E, CAnimationEntry::CAnimationEntry(const CAnimationEntry&) @ 0x00471954 (landed; out of line).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
//   _Construct inlined (`new (p) T(value)`: the null test, then pair's member-wise copy constructor -- the key dword, then
//   the CAnimationEntry copy constructor on +4). CAnimationEntry is local (no engine header; PDB 0x10 bytes: BankIndex,
//   AnimName, GroupName, CIVCountedPointer<CAnimComponentMap> Components).
// Shape: `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer; pair's copy constructor must be
//   `__forceinline` in this TU.
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CAnimComponentMap;
class CAnimationEntry {
public:
    unsigned long      BankIndex;                   // +0x00
    long               AnimName;                    // +0x04 CDefString
    long               GroupName;                   // +0x08 CDefString
    CAnimComponentMap* Components;                  // +0x0C CIVCountedPointer<CAnimComponentMap>
    CAnimationEntry(const CAnimationEntry& other);  // 0x00471954
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

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    struct : A { T* _M_data; } _M_end_of_storage;   // +0x08
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);
};

template <class T, class A> T* vector<T, A>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last)
{
    T* result = _M_end_of_storage.allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}

typedef pair<unsigned long, CAnimationEntry> _AnimEntry;
template _AnimEntry* vector<_AnimEntry, allocator<_AnimEntry> >::_M_allocate_and_copy(unsigned int n, const _AnimEntry* first,
                                                                                      const _AnimEntry* last);
}