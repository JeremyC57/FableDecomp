// std::__uninitialized_copy(pair<CCharString, CRegisteredPSCInfo>* first, pair<CCharString, CRegisteredPSCInfo>* last, pair<CCharString, CRegisteredPSCInfo>* result, const __false_type&) -- STLport copy path of the registered-PSC vector: copy-construct [first, last) into raw storage at result and return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@PAU12@@std@@YIPAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2BA0, length 94 bytes, __fastcall `ret 8` (first/last in ecx/edx,
//   result and the empty tag on the stack).
// Identity: the only callers are the landed vector<pair<CCharString, NParticleEngine::CRegisteredPSCInfo> >::
//   _M_insert_overflow 0x00AF37F0 (calls 0x00AF383B / 0x00AF38A4: the prefix and suffix moves), whose template model this
//   row now shares (that source spells the namespace `std`; this row follows its applied fill_n twin 0x00AF2C00 in `std`
//   until the owner settles the STL namespace); element as in that twin (Ego_r.pdb CVectorMap<CCharString,
//   NParticleEngine::CRegisteredPSCInfo> entry). Catalog "UninitCopy_PairCharStringRegisteredPSCInfo" and manifest "_Move<..>" were a shape
//   name and a propagation.
// Shape: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` with the null-tested placement
//   copy (pair user copy constructor, the info's memberwise copy).
// Callees: CCharString copy constructor 0x0099EC30 (twice per element).
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E (allocator::allocate, unused here)
inline void* operator new(unsigned int, void* p) { return p; }

class CPSCBase;

class CCharString {
public:
    void* PStringData;                                      // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class stdP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    stdP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AF2BA0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
template CPSCPair* __fastcall std::__uninitialized_copy<CPSCPair*, CPSCPair*>(CPSCPair*, CPSCPair*, CPSCPair*, const std::__false_type&);
