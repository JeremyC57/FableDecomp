// CPreallocTexturePool::`scalar deleting destructor'(unsigned int flags) -- destroy the pool's texture-pointer vector (release every CIVCountedPointer<CManagedTextureData>, free the storage), run the CResourceList base destructor, free the object when bit 0 of flags is set
// Leaf: ??_GCPreallocTexturePool@@
// Grade: RELOCATION_MATCH -- implicit destructor kept (no vtable reset) and inlined into the ??_G because the TU also
//   destroys a local CPreallocTexturePool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6BFC0, 64 bytes, `ret 4`. CPreallocTexturePool vtable 0x0129DC50 slot 0.
// Callees: 0x00A6B700 std::__destroy_aux(first, last, const __false_type&) (out of line, __fastcall, `ret 4`; the
//   landed "Destroy_00a6b700"), 0x00BFEA14 free (the vector storage), 0x009FC5C0 CResourceList::~CResourceList (catalog
//   "Stack<unsigned long,64>::~Stack" is propagated; the landed CTextureManager dtor 0x00A6A440 names it), 0x00BFE9BC
//   operator delete.
// Label: the manifest's "CDataBankGroupEntry_Destructor" is descriptive and wrong: the only reference is the slot-0 entry
//   of the CPreallocTexturePool vtable 0x0129DC50 (rtti-vtables.tsv).
// Layout (retail 0x54 bytes, as the landed CTextureManager ctor/dtor 0x00A6A360/0x00A6A440 model it): CResourceList
//   base 0x3C (vptr +0), Count +0x3C, MaxSize +0x40, PoolIndex +0x44, Textures +0x48 (12-byte STLport vector; the
//   Ego_r.pdb CArray is 16, so the PDB size is 0x58).
// Shape: the class declares no destructor (VC7.1's generated destructor drops the vtable reset); the TU also destroys a
//   local pool (DestroyLocalPool), and with that the generated destructor is inlined into the deleting destructor: the member
//   vector's STLport destructor -- `_Destroy(_M_start, _M_finish)` down to an out-of-line `__destroy_aux(first, last,
//   __false_type())` whose tag temporary is homed in the LIVE flags slot (`lea eax,[esp+8]`) -- then _Vector_base's
//   `if (_M_start) free(_M_start)` (start re-read after the call), and the base destructor out of line.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A (only for the emitting helper below)

class CManagedTextureData {                             // CIVCountedPointeeBase-derived: vptr +0, count +4
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1: destroy at count zero
    long RefCount;                                      // +0x04
    void DecRef()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->DecRef();
            Object = 0;
        }
    }
};

namespace std {

class __false_type { public: __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x00A6B700
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline void _Destroy(_Tp* __first, _Tp* __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            free(_M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}

class CResourceList {                                   // vtable 0x0129C808, 0x3C bytes
public:
    CResourceList();                                    // 0x009FC570
    virtual ~CResourceList();                           // 0x009FC5C0
    unsigned char _body[0x3C - 4];
};

class CPreallocTexturePool : public CResourceList {     // vtable 0x0129DC50, retail 0x54 bytes
public:
    unsigned long Count;                                // +0x3C
    unsigned long MaxSize;                              // +0x40
    long PoolIndex;                                     // +0x44
    std::vector<CIVCountedPointer<CManagedTextureData> > Textures;   // +0x48
    CPreallocTexturePool() : Count(0), MaxSize(0), PoolIndex(-1) { }
};

void DestroyLocalPool() { CPreallocTexturePool pool; }
CPreallocTexturePool* NewPreallocTexturePool()          // emits the vtable and with it the deleting destructor
{
    return new CPreallocTexturePool;
}