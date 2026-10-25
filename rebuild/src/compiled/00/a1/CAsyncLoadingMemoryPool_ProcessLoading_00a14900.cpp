// CAsyncLoadingMemoryPool::ProcessLoading() -- call ProcessLoading() on every registered async-loading callback
// Leaf: ?ProcessLoading@CAsyncLoadingMemoryPool@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14900, 36 bytes, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb name `?ProcessLoading@CAsyncLoadingMemoryPool@@SIXXZ` -- a STATIC __fastcall
//   member taking nothing. Its siblings in the same TU are AddLoadingCallback (0x00A14990) and
//   RemoveLoadingCallback (0x00A14930), and all three work on the static
//   `CAsyncLoadingMemoryPool::LoadingCallbacks` (PDB "dynamic initializer for 'CAsyncLoadingMemoryPool::
//   LoadingCallbacks'"), an STLport vector<CAsyncLoadingProcessCallback*> at 0x013D2770 (start) / 0x013D2774
//   (finish) / 0x013D2778 (end of storage). Callers: CWorld::UpdateRegionLoad and CWorldMap::LoadRegion. The
//   catalog label "_global::ProcessLoading" lacks the class.
// Shape: a plain iterator loop over the vector; `end()` is re-read after every virtual call (the callee may touch
//   the static), the element is called through vtable slot 0 (CAsyncLoadingProcessCallback::ProcessLoading;
//   CEngineResourceManager overrides it in its +0x0C base, vtable 0x012A1B34 slot 0 = 0x00B3B8C0).
namespace std {
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // retail STLport vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};
}

class CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() = 0;                                     // slot 0
};

class CAsyncLoadingMemoryPool {
public:
    static void __fastcall ProcessLoading();
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;   // 0x013D2770
};

void __fastcall CAsyncLoadingMemoryPool::ProcessLoading()
{
    for (std::vector<CAsyncLoadingProcessCallback*>::iterator it = LoadingCallbacks.begin(); it != LoadingCallbacks.end(); ++it)
        (*it)->ProcessLoading();
}