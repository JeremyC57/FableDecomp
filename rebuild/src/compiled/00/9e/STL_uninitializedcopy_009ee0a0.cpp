// std::__uninitialized_copy(const CGenericVar*, const CGenericVar*, CGenericVar*, const __false_type&) -- copy-construct a range of generic variables into raw storage, return the end of the copies
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EE0A0, 86 bytes (96 with padding), __fastcall (first in ecx, last in edx, result
//   and the tag reference on the stack, `ret 8`). Catalog label "_Copy_backward<CExplosionRing*,CExplosionRing*>" is
//   a propagated name: the vtable stored is 0x0129C3E4 = CGenericVar and the walk is forward.
// Callee: CCharString copy ctor 0x0099EC30.
// Shape: STLport 4.x __uninitialized_copy(first, last, result, const __false_type&): placement-new null test, the implicit
//   CGenericVar copy (vtable, Type, string, then the four scalars with the bool as a byte), cur returned in eax. Sibling
//   of the landed __uninitialized_fill_n<CGenericVar*> 0x009EE100.
// Layout: Ego_r.pdb CGenericVar (0x1C).
// Leaf: ??$__uninitialized_copy@PBVCGenericVar@@PAV1@@std@@YIPAVCGenericVar@@PBV1@0PAV1@ABU__false_type@0@@Z
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

class CGenericVar {                             // vtable 0x0129C3E4, 28 bytes
public:
    EGenericVarType Type;                       // +0x04
    CCharString     ValString;                  // +0x08
    long            ValSLONG;                   // +0x0C
    unsigned long   ValULONG;                   // +0x10
    bool            ValBool;                    // +0x14
    float           ValFloat;                   // +0x18
    virtual ~CGenericVar();
};

namespace std {

struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CGenericVar* __fastcall __uninitialized_copy(const CGenericVar*, const CGenericVar*, CGenericVar*, const __false_type&);

} // namespace std