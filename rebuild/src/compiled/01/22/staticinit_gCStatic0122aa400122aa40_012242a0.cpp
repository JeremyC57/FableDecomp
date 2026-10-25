// dynamic initializer for 'CAsyncLoadingMemoryPool::LoadingCallbacks' -- the static callback vector's initializer: the
// zeroing STLport vector<CAsyncLoadingProcessCallback*> constructor folds into the static's zero-initialisation, so only
// the atexit registration of its destructor thunk 0x0122AA40 remains.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012242A0, oracle length 12 bytes.
// Retail bytes:  push 0x122aa40 ; call _atexit ; pop ecx ; ret
// Name: Ego_r.pdb 0x0101AE50 / FableWin.pdb 0x0400B4E0 `dynamic initializer for 'CAsyncLoadingMemoryPool::LoadingCallbacks'`
//   (and the atexit destructor 0x0101E730 / 0x040129B0, retail 0x0122AA40 = `mov eax,[0x13D2770]; if (eax) free(eax)`);
//   the landed CAsyncLoadingMemoryPool::ProcessLoading 0x00A14900 / AddLoadingCallback 0x00A14990 use the vector at
//   0x013D2770. Was modelled as `g_CStatic_0122aa40_0122aa40` "@ 0x0122AA40" (the thunk's address, not the object's).
// Shape: the static member's definition makes VC7.1 emit this initializer `_$E1` and the thunk `_$E2` (0x0122AA40);
//   one retail TU, two catalog rows.
// Leaf: _$E1
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CAsyncLoadingProcessCallback;

namespace std {
template <class _Tp>
class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _Tp* _M_end_of_storage;                        // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start != 0)
            _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start);
    }
};
}

class CAsyncLoadingMemoryPool {
public:
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;   // 0x013D2770
};

std::vector<CAsyncLoadingProcessCallback*> CAsyncLoadingMemoryPool::LoadingCallbacks;
