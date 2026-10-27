// CControlsDef::Alloc() -- the def class's static allocator: `new CControlsDef` (inlined default constructor), or null when the allocation fails
// Leaf: ?Alloc@CControlsDef@@SIPAVCDefClassBase@@XZ
// Retail: Fable.exe @ 0x00432180, 82 bytes, static __fastcall (no arguments), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Alloc@CControlsDef@@SIPAVCDefClassBase@@XZ; the object it builds stores vtable 0x01230E84
// (RTTI .?AVCControlsDef@@, bases CParentDefClassBase / CDefClassBase / CDefPointeeBase / CResource), and the
// `operator new(0x50)` size matches the class (Ego_r.pdb CControlsDef 0x58 less 8 (the 16-byte debug vector + tail padding)). Callers: CompileDefsPlatformSpecific (0x0042F6DE), CStartupDefinitionManager::InitAndCompilePlatformSpecific (0x00433C0C) and CGameDefinitionManager::InitAndCompile (0x0044D57F) register it as the def class's allocator.
// Callees: operator new @ 0x00BFEA1A (MSVCR71 scalar new; the `cmp esi,ebx ; je` is the new-expression's null guard),
// CResource::CResource() @ 0x009FBEC0.
// Shape: the inlined constructor chain, then the most-derived vtable, then Controls' empty STLport vector (+0x3C/+0x40/+0x44 zeroed from ebx); the Toggle* flags stay uninitialised.
// Model: the def-base chain of the landed CEngineDef::CEngineDef 0x00433F90 (engine/CResource.h, CDefClassBase.h and
// CParentDefClassBase.h flatten the vptr, so the polymorphic bases are written out locally; NOTES.md "Header debt").
void* __cdecl operator new(unsigned int size);           // 0x00BFEA1A
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefClassBase() : PDefManager(0)
    {
        Setup = false;
        Template = false;
        DefaultValsApplied = false;
    }
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString {
public:
    CDefString() : TablePos(-1) { }
    long TablePos;
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc> class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CVectorMap() : Dirty(false) { }
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
class C2DVector { public:
    float X, Y;
};
class CActionInputControl { public:                      // 0x1C bytes (landed 0x0040ED10 / 0x00405DB0 layout)
    long GameAction;                                     // +0x00
    long ControllerType;                                 // +0x04
    long KeyboardKey;                                    // +0x08
    long XboxButton;                                     // +0x0C
    long MouseButton;                                    // +0x10
    C2DVector ControlDirection;                          // +0x14
};
class CControlsDef : public CParentDefClassBase {        // vtable 0x01230E84
public:
    CControlsDef() { }
    static CDefClassBase* __fastcall Alloc();
    std::vector<CActionInputControl, std::allocator<CActionInputControl> > Controls;   // +0x3C (Ego_r +0x40)
    bool ToggleZTarget;                                  // +0x48 (Ego_r +0x50)
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4A
    bool ToggleExpressionMenu;                           // +0x4B
    bool ToggleExpressionShift;                          // +0x4C
    bool FlourishNeedsAttackButtonHeld;                  // +0x4D
};

CDefClassBase* __fastcall CControlsDef::Alloc()
{
    return new CControlsDef;
}