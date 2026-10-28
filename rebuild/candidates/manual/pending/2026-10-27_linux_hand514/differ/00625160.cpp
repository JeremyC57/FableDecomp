// NUISystem::CUIStateDef::`scalar deleting destructor'(unsigned int flags) -- destroy a UI state def (its children-not-affected list, the parent def's sub-def map, then CResource) and free it when asked
// Grade: DIFFER(30v70) -- the implicit CUIStateDef/CParentDefClassBase/CDefClassBase destructors (no vptr reset, as retail) are not inlined into the compiler-generated ??_G (it calls ??1, whose body byte-matches retail's inlined part); user-written __forceinline destructors inline but add the CUIStateDef and CParentDefClassBase vptr stores retail lacks (82v70); the 0x0046CDFC qualified-destructor device and /O1, /Ob2 do not change it
// Leaf: ??_GCUIStateDef@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625160, 70 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x0125871C, whose RTTI names .?AVCUIStateDef@NUISystem@@ (bases CParentDefClassBase,
//   CDefClassBase, CDefPointeeBase, CResource, CIVCountedPointeeBase); Ego_r.pdb ??_GCUIStateDef@NUISystem@@UAEPAXI@Z. The body is the
//   landed plain destructor 0x00624DC0 inlined plus `if (flags & 1) delete this`. The catalog label
//   "CCompressedMemoryDataInputStream::`vector_deleting_destructor'" is propagated (a single flag test: scalar, not vector).
// Callees: free 0x00BFEA14 (the two vectors' storage), CResource::~CResource() 0x009FC550, operator delete 0x00BFE9BC.
// Layout (retail, byte packed defs; Ego_r.pdb names, PDB - 4): CParentDefClassBase SubDefInfoMap (CVectorMap, its STLport vector at
//   +0) +0x29; ChildrenNotAffected (vector<unsigned long>) +0x6c. The CDefPointeeBase vptr store 0x01230BA0 is the inlined empty
//   base destructor's.
// Shape: as the landed scalar deleting destructors (0x00838500): an inline constructor plus a creator emit the vtable, and the
//   implicit destructors (VC7.1 resets the vptr only in user-written ones: here CDefPointeeBase) are defined first by qualified
//   explicit destructor calls, so they inline into the compiler's ??_G (the landed 0x0046CDFC device).
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

class C2DVector { public: float X, Y; };
class CRGBFloatColour { public: float R, G, B, A; };

namespace NUISystem {
class CUIStateDef : public CParentDefClassBase {   // vtable 0x0125871C
public:
    unsigned long              GraphicIndex;       // +0x3c
    unsigned long              StateChangeFlag;    // +0x40
    long                       StateChangeType;    // +0x44 EStateChangeType
    C2DVector                  Position;           // +0x48
    C2DVector                  Zoom;               // +0x50
    CRGBFloatColour            Colour;             // +0x58
    float                      UpdateTime;         // +0x68
    std::vector<unsigned long> ChildrenNotAffected;   // +0x6c
    bool                       LinearChange;       // +0x78
    CUIStateDef() { }
};
}

// the qualified explicit destructor calls make VC7.1 define the implicit destructors before it generates the deleting
// destructor, so the chain inlines (the landed 0x0046CDFC device); not part of the retail function
void KillDefBase_00625160(CDefClassBase* p) { p->CDefClassBase::~CDefClassBase(); }
void KillParentDef_00625160(CParentDefClassBase* p) { p->CParentDefClassBase::~CParentDefClassBase(); }
void KillUIStateDef_00625160(NUISystem::CUIStateDef* p) { p->~CUIStateDef(); }

// emits the vtable (and with it the compiler's scalar deleting destructor); not part of the retail function
NUISystem::CUIStateDef* Create_CUIStateDef_00625160()
{
    return new NUISystem::CUIStateDef;
}
