// dynamic atexit destructor for 'CAsyncLoadingMemoryPool::LoadingCallbacks' -- frees the static callback vector's
// storage at exit: the inlined STLport ~vector<CAsyncLoadingProcessCallback*> (`if (_M_start) free(_M_start)`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122AA40, oracle length 17 bytes.
// Retail bytes:  mov eax,[0x13d2770] ; test eax,eax ; je R ; push eax ; call free ; pop ecx ; R: ret
// Registered by the object's dynamic initializer 0x012242A0 (`push 0x0122AA40; call _atexit`).
// Name: Ego_r.pdb 0x0101E730 / FableWin.pdb 0x040129B0 `dynamic atexit destructor for 'CAsyncLoadingMemoryPool::
//   LoadingCallbacks'` (and the initializer 0x0101AE50 / 0x0400B4E0); the landed CAsyncLoadingMemoryPool::ProcessLoading
//   0x00A14900 and AddLoadingCallback 0x00A14990 use the same static vector at 0x013D2770 (start) / 0x013D2774 (finish).
//   Was modelled as `DisplayProgress` over g_AutoTinyOptionalObject_013d2770 (a propagated manifest label).
// Shape: the static member's definition makes VC7.1 emit the initializer `_$E1` (0x012242A0: the zeroing STLport
//   constructor folds into the static's zero-initialisation, leaving only the atexit registration) and this thunk `_$E2`,
//   with the STLport _Vector_base destructor and the malloc allocator's deallocate (free @ 0x00BFEA14) inlined. The same
//   TU is the source of 0x012242A0 (one retail TU, two catalog rows).
// Leaf: _$E2
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
