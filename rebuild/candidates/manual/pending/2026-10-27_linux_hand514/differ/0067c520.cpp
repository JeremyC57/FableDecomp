// CBlurredAttitudesDef::`scalar deleting destructor'(unsigned int flags) -- destroy a blurred-attitudes def (its attitude list, the parent def's sub-def map, then CResource) and free it when asked
// Grade: DIFFER(30v70) -- as its twin 0x00625160: the implicit def destructors (no vptr reset, as retail) are not inlined into the compiler-generated ??_G (it calls ??1); user-written __forceinline destructors inline but add vptr stores retail lacks
// Leaf: ??_GCBlurredAttitudesDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067C520, 70 bytes, thiscall, `ret 4`; byte twin of 0x00625160 (NUISystem::CUIStateDef) with the
//   derived vector at +0x3c.
// Identity (proven): slot 0 of vtable 0x0125B8AC, whose RTTI names .?AVCBlurredAttitudesDef@@ (bases CParentDefClassBase, CDefClassBase,
//   CDefPointeeBase, CResource, CIVCountedPointeeBase); Ego_r.pdb ??1CBlurredAttitudesDef@@UAE@XZ (its ??_G is compiler-generated).
//   The catalog label "CCompressedMemoryDataInputStream::`vector_deleting_destructor'" is propagated (a single flag test: scalar).
// Callees: free 0x00BFEA14 (the two vectors' storage), CResource::~CResource() 0x009FC550, operator delete 0x00BFE9BC.
// Layout (retail, byte packed defs; Ego_r.pdb names): CParentDefClassBase SubDefInfoMap +0x29; Attitudes
//   (vector<EOpinionAttitudeType>, PDB +0x40) +0x3c.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);             // 0x00BFE9BC

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
};
}

class CDefinitionManager;
class CResource {                                  // vtable root (CIVCountedPointeeBase below it)
public:
    CResource();
    virtual ~CResource();                          // 0x009FC550
    unsigned char _body[0x1c - 4];
};
class CDefPointeeBase : public CResource {         // vtable 0x01230BA0
public:
    CDefPointeeBase();
    __forceinline ~CDefPointeeBase() { }
};
#pragma pack(push, 1)                              // def classes are byte packed in retail (CParentDefClassBase is 0x39 bytes)
class CDefClassBase : public CDefPointeeBase {
public:
    struct CSubDefInfo { unsigned long Index; unsigned long Data; };
    CDefinitionManager* PDefManager;               // +0x1c
    unsigned long       GlobalIndex;               // +0x20
    unsigned char       Flags;                     // +0x24 Setup/Template/DefaultValsApplied bit fields
    CDefClassBase();
};
#pragma pack(pop)
template <class K, class V> struct CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    std::vector<std::pair<K, V> > Vector;          // +0x00
    C                             Compare;         // +0x0c
    bool                          Dirty;           // +0x0d
};
class CDefString { public: long TablePos; };
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                  // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
    CParentDefClassBase();
};
#pragma pack(pop)

enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE_0067c520 };
class CBlurredAttitudesDef : public CParentDefClassBase {   // vtable 0x0125B8AC
public:
    std::vector<EOpinionAttitudeType> Attitudes;   // +0x3c
    CBlurredAttitudesDef() { }
};

// the qualified explicit destructor calls make VC7.1 define the implicit destructors before it generates the deleting
// destructor (the landed 0x0046CDFC device); not part of the retail function
void KillDefBase_0067c520(CDefClassBase* p) { p->CDefClassBase::~CDefClassBase(); }
void KillParentDef_0067c520(CParentDefClassBase* p) { p->CParentDefClassBase::~CParentDefClassBase(); }
void KillBlurredAttitudesDef_0067c520(CBlurredAttitudesDef* p) { p->~CBlurredAttitudesDef(); }

// emits the vtable (and with it the compiler's scalar deleting destructor); not part of the retail function
CBlurredAttitudesDef* Create_CBlurredAttitudesDef_0067c520()
{
    return new CBlurredAttitudesDef;
}
