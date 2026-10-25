// std::vector<CCardPositionsDef>::_M_clear() -- STLport: destroy [_M_start, _M_finish) and free the block
// Leaf: ?_M_clear@?$vector@VCCardPositionsDef@@V?$allocator@VCCardPositionsDef@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBD8D, 35 bytes, plain `ret` (__thiscall).
// Identity: the element is CCardPositionsDef: the element copy-constructor 0x004EBDF1 (landed CCardPositionsDef::CCardPositionsDef) stores vtable 0x012439DC, whose RTTI locator is `.?AVCCardPositionsDef@@`; the retail element stride is 0x48
// (Ego_r.pdb CCardPositionsDef is 80 bytes with its debug-build containers). The destroy callee 0x004DA61A walks the range in 0x48
// steps calling virtual slot 0 (the scalar deleting destructor, flag 0). Callers: operator= 0x004EBCA9 (call 0x004EBCF2) and the vector's binary loader 0x004F9FA3 (call 0x004FA06A). The old label
// (an "OnReadFinished" / "StdList_Dtor" propagation) described only the body.
// Body: `_Destroy(_M_start, _M_finish)` = `__destroy_aux(first, last, __false_type())` (0x004DA61A; the tag lives in the
// high byte of the pushed ecx slot, `lea eax,[esp+7]`), then the malloc allocator's deallocate: free(_M_start) when non-null.
// Shape: `_Destroy` is __forceinline here; as a plain inline the size TU keeps it as an out-of-line two-argument call.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CCardPositionsDef {
public:
    virtual ~CCardPositionsDef();
    unsigned char _body[0x44];
};

namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004DA61A

template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _Tp* _M_end_of_storage;                              // +0x08
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start);
    }
};

template void vector<CCardPositionsDef>::_M_clear();
}
