// std::__uninitialized_fill_n<pair<float, CCharString>*, unsigned int, pair<float, CCharString> >(first, n, x, const __false_type&) -- STLport: copy-construct n animation-tag entries from x into raw storage, returning the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@MVCCharString@@@std@@IU12@@std@@YIPAU?$pair@MVCCharString@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CF990, length 64 bytes (50 + int3 padding), __fastcall (/Gr TU: ecx = first,
// edx = n, stack: &x and the tag reference), `ret 8`.
// Identity: the 8-byte value is pair<float, CCharString> -- CSubtitledMessage's AnimationTags element (CVectorMap<float,
// CCharString>, Ego_r.pdb); the single caller 0x004D19D4 is that vector's `_M_insert_overflow` 0x004D1940 (the `n != 1`
// arm; the `n == 1` arm inlines one `_Construct`). The catalog "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is
// propagated.
// Callees: CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (no EH: no _STLP_TRY). n is unsigned
// (`jbe`); _Construct is the placement new of the pair (null-tested), whose user copy ctor copies the float as a dword
// and copy-constructs the string.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

typedef pair<float, CCharString> _AnimationTagEntry;
template _AnimationTagEntry* __fastcall __uninitialized_fill_n<_AnimationTagEntry*, unsigned int, _AnimationTagEntry>(
    _AnimationTagEntry*, unsigned int, const _AnimationTagEntry&, const __false_type&);
}