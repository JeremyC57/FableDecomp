// CEngineManager::CEngineManager() -- the engine manager singleton: an empty engine list (STLport sentinel node) and an empty changed-area vector
// Leaf: ??0CEngineManager@@QAE@XZ
// Retail: Fable.exe @ 0x00436D30, 47 bytes (16-aligned /O2 entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01231584 is RTTI .?AVCEngineManager@@ (base CTBaseSingleton<CEngineManager>); PDB name set
// `??0CEngineManager@@QAE@XZ`. The vtable is stored only here and in the destructor (0x00436FCF) and at 0x005078C1 (the
// engine set-up that builds it in place). The catalog label CConsoleCommandBase::CConsoleCommandBase is propagated.
// Callee: malloc 0x00BFEA0E (the STLport list node allocator, 12-byte _List_node<CIEngine*>).
// Layout: Ego_r.pdb CEngineManager: Engines list<CIEngine*> +0x04 (retail STLport list = one node pointer),
// ChangedAreas vector<C2DBoxI> Ego_r +0x10 -> retail +0x08 (12 bytes).
// Shape: STLport 4.6 _List_base(a): _M_node(a, 0), then the sentinel allocated, self-linked, stored; vector() zeroes.
// CTBaseSingleton's inline constructor vptr store is dead and dropped by VC7.1.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> { public:
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) {}
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
};
}
template <class T> class CTBaseSingleton { public:
    virtual ~CTBaseSingleton() {}
    static T& __fastcall Get();
};
class CIEngine;
class C2DBoxI { public: long X0, Y0, X1, Y1; };
class CEngineManager : public CTBaseSingleton<CEngineManager> { public:
    CEngineManager();
    std::list<CIEngine*>    Engines;                     // +0x04
    std::vector<C2DBoxI>    ChangedAreas;                // +0x08
};
CEngineManager::CEngineManager()
{
}