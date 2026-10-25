// CScriptBase::CScriptBase() -- the script base: CBaseClassNonCopyable, its vtable, an empty Threads list, empty EntityScripts / EntityScriptBindings / persist maps, no active thread
// Leaf: ??0CScriptBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB8110, 82 bytes, thiscall: base ctor 0x0099A2F0, vtable 0x012C1648, ebx = 0,
//   Threads (+4) cleared, then its 16-byte sentinel from malloc 0x00BFEA0E linked to itself, the three CVectorMaps'
//   vectors and Dirty bytes (+8/+0x15, +0x18/+0x25, +0x30/+0x3d), PActiveThread (+0x2c) = 0; LastWorldUpdateFrame (+0x28)
//   is left alone; malloc's `add esp,4` is scheduled before the last byte store.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 "the CScriptBase ctor 0xCB8110, itself mislabelled CHeroMorphDef (fix the
//   root)"): Ego_r.pdb ??0CScriptBase@@QAE@XZ; reloc_bindings already binds ??0CScriptBase@@QAE@XZ -> 00cb8110 (from the
//   landed CQ_HangingTreeEvilScript ctor 0x00D68F00), and every NScript quest-script ctor / Alloc (0x00CEF950, 0x00DC24E0,
//   0x00CFAD30 ...) calls it first; the landed row named it CVtable_012c1648 after the vtable.
// Layout: Ego_r.pdb CScriptBase (84 bytes) with STLport containers: Threads list 4 bytes (PDB 12), each CVectorMap 16
//   (PDB 20), so LastWorldUpdateFrame +0x28, PActiveThread +0x2c, RuntimeScriptedEntityPersist +0x30, size 0x40 (PDB - 0x14).
//   Base: Ego_r.pdb ??0CBaseClassNonCopyable@@IAE@XZ (protected) / ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual).
//   Model: no engine/CScriptBase.h exists; the STLport pieces are the ones this ctor inlines.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (_STLP_USE_MALLOC node allocator, no OOM path)
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CCharString { public: void* m_pStringData; };
template <class T> class CCountedPointer { public: T* Data; void* Info; };   // 8 bytes
class CSpawnedFuncBase;
class CActiveEntityScriptBase;
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int n) { return (_Tp*)malloc(n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p) : _MaybeReboundAlloc(a), _M_data(p) { }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    explicit list(const _Alloc& a = _Alloc()) : _List_base<_Tp, _Alloc>(a) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& a = _Alloc()) : _Vector_base<_Tp, _Alloc>(a) { }
};
}
template <class K, class V> class CKeyPairCompareLess { };   // empty
template <class V> class CCompareString { };                 // empty
template <class K, class V, class C> class CVectorMap {
public:
    std::vector<std::pair<K, V> > Vector;                  // +0x00
    C    Compare;                                            // +0x0c
    bool Dirty;                                              // +0x0d
    CVectorMap() : Dirty(false) { }
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo { public: unsigned __int64 UID; CCharString PersistScript; };
    CVectorMap<unsigned __int64, CPersistInfo, CKeyPairCompareLess<unsigned __int64, CPersistInfo> > PersistScripts;
};
}
class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();
    virtual ~CScriptBase();
protected:
    std::list<CCountedPointer<CSpawnedFuncBase> > Threads;                                    // +0x04
    CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>,
               CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > EntityScripts;   // +0x08
    CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>,
               CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;              // +0x18
    unsigned long     LastWorldUpdateFrame;                                                    // +0x28
    CSpawnedFuncBase* PActiveThread;                                                           // +0x2c
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;                       // +0x30
};

CScriptBase::CScriptBase()
    : PActiveThread(0)
{
}
