// CBlurredAttitudesDef::CBlurredAttitudesDef() -- the definition's default constructor: the def bases, then the empty member vector
// Leaf: ??0CBlurredAttitudesDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067C4D0, 60 bytes, thiscall, plain `ret`.
// Identity (proven): stores vtable 0x0125B8AC (RTTI .?AVCBlurredAttitudesDef@@); Ego_r.pdb CBlurredAttitudesDef is CParentDefClassBase plus Attitudes (vector<EOpinionAttitudeType>).
// Callees: CResource::CResource() 0x009FBEC0.
// Shape (family M023, representative 0x00430430): CDefClassBase and CParentDefClassBase are `__declspec(novtable)` with
//   inline constructors (their stores precede the vtable store), packed (pack(1): CDefString at +0x25, the CVectorMap at
//   +0x29 with Dirty at +0x36); the vector member's three zero stores follow the vtable store.
// Header debt: engine/CDefClassBase.h / CParentDefClassBase.h / CBlurredAttitudesDef.h are flat layouts without the base chain or
//   constructors, so the chain is declared here.
class CDefinitionManager;
class CResource {
public:
    CResource();                                          // 0x009FBEC0
    virtual ~CResource();
    unsigned char _body[0x18];
};
#pragma pack(push, 1)
class __declspec(novtable) CDefClassBase : public CResource {
public:
    class CSubDefInfo { public: unsigned long Members[2]; };
    CDefClassBase() : PDefManager(0) { Setup = 0; Template = 0; DefaultValsApplied = 0; }
    CDefinitionManager* PDefManager;                      // +0x1c
    unsigned long GlobalIndex;                            // +0x20
    unsigned char Setup : 1;                              // +0x24
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString { public: CDefString() : TablePos(-1) { } long TablePos; };
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    CVectorMap() : Dirty(false) { }
    C Compare;                                            // +0x0c
    bool Dirty;                                           // +0x0d
};
#pragma pack(push, 1)
class __declspec(novtable) CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                         // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };
class CBlurredAttitudesDef : public CParentDefClassBase {
public:
    CBlurredAttitudesDef();
    std::vector<EOpinionAttitudeType> Attitudes;          // +0x3c
};
CBlurredAttitudesDef::CBlurredAttitudesDef()
{
}