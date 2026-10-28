// CThingManager::OnLevelUnload(long, bool) -- unload one level: wrap it in a one-element level vector and run OnLevelsUnload
// Grade: DIFFER(105v74) -- retail has no zero stores of the three vector words, keeps the malloc block in esi across OnLevelsUnload and frees it with no null test or member reload; STLport 4.6 vector(1, level) gives all three
// Leaf: ?OnLevelUnload@CThingManager@@QAEXJ_N@Z
// Retail: Fable.exe @ 0x00525D90, 74 bytes (16-aligned /O2 entry), thiscall `ret 8`.
// Identity: Ego_r.pdb `?OnLevelUnload@CThingManager@@QAEXJ_N@Z`; caller CWorldMap::UnloadLevel 0x004FEDE0 (`push 1 ; push
// level` then the thing-manager getter (vtable +0xC) and this call: the two pushes are this call's arguments); the callee
// is the plural CThingManager::OnLevelsUnload(const vector<long>&, bool) 0x00525CD0 (PDB
// `?OnLevelsUnload@CThingManager@@QAEXABV?$vector@JV?$allocator@J@std@@@std@@_N@Z`; it loops over the levels calling
// SaveThingsOnLevelReadyForUnloading 0x005252B0). The catalog label CLandscapeGPUUpdateQueue::AddTask is propagated.
// Callees: malloc 0x00BFEA0E, OnLevelsUnload 0x00525CD0, free 0x00BFEA14.
// Shape: STLport 4.6 `vector<long> levels(1, level)`: _Vector_base(n) allocates n, finish = start, end_of_storage =
// start + n; then uninitialized_fill_n stores the one element; ~vector frees the block.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __x;
    return __first;
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    vector(unsigned int __n, const _Tp& __val, const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};
}
class CThingManager { public:
    void OnLevelsUnload(const std::vector<long>& levels, bool saveThings);   // 0x00525CD0
    void OnLevelUnload(long level, bool saveThings);
};
void CThingManager::OnLevelUnload(long level, bool saveThings)
{
    std::vector<long> levels(1, level);
    OnLevelsUnload(levels, saveThings);
}
