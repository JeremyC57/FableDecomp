// std::__uninitialized_fill_n(pair<EBodyLocation, CWoundMorphs::CEntry>* first, unsigned int n, const pair<EBodyLocation, CWoundMorphs::CEntry>& x, const __false_type&) -- STLport placement fill: copy-construct x into n raw slots, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@IU12@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758720, 49 bytes, /Gr `__fastcall` (first in ecx, n in edx; &x and the tag reference on the stack),
//   `ret 8`.
// Identity (proven): only caller the landed vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_insert_overflow 0x00759190 (its fill_len arm; it names this callee); the landed CWoundMorphs vector rows (erase 0x00758F50, _M_insert_overflow 0x00759190, _M_fill_insert 0x00759290) are vector<pair<EBodyLocation, CWoundMorphs::CEntry> > (Ego_r.pdb CWoundMorphs::Morphs), 44-byte stride with the 40-byte entry as 10 dwords.
// Shape (family K022, representative 0x00758720): STLport 4.6 `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
//   return __cur;` (unsigned n: `test edx,edx ; jbe`), the placement new's null test, STLport's memberwise pair copy
//   (the key word, then the trivially copyable record as one `rep movsd`).
inline void* operator new(unsigned int, void* where) { return where; }
enum EBodyLocation { };                                  // Ego_r.pdb (enumerators not needed)
class CWoundMorphs {
public:
    class CEntry {                                       // 40 bytes (Ego_r.pdb), trivially copyable
    public:
        EBodyLocation Location;                          // +0x00
        long ReplacingTextureIndex;                      // +0x04
        long ReplacingBumpMapIndex;                      // +0x08
        long WoundTextureIndex;                          // +0x0C
        long WoundBumpMapIndex;                          // +0x10
        long ScarTextureIndex;                           // +0x14
        long ScarBumpMapIndex;                           // +0x18
        long FadeToScarDuration;                         // +0x1C
        long TotalDuration;                              // +0x20
        float PermanentScarStrength;                     // +0x24
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
typedef pair<EBodyLocation, CWoundMorphs::CEntry> _Elem;
template _Elem* __fastcall __uninitialized_fill_n<_Elem*, unsigned int, _Elem>(_Elem*, unsigned int, const _Elem&, const __false_type&);
} // namespace std