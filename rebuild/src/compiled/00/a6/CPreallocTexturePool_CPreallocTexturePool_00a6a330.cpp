// CPreallocTexturePool::CPreallocTexturePool() -- CResourceList base, this vtable, Count and MaxSize zero, PoolIndex -1, an empty Textures array
// Leaf: ??0CPreallocTexturePool@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6A330, 45 bytes. Name: Ego_r.pdb ??0CPreallocTexturePool@@QAE@XZ -- the vtable it
//   stores, 0x0129DC50, is CPreallocTexturePool's (rebuild/manifest/rtti-vtables.tsv), and the landed CTextureManager
//   constructor 0x00A6A360 inlines this same body sixteen times. The catalog label
//   "CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream" is wrong (that class is 0x24 bytes in the PDB).
// Layout: Ego_r.pdb CPreallocTexturePool (retail 0x54: CResourceList 0x3C, Count +0x3C, MaxSize +0x40, PoolIndex +0x44,
//   CArray<CIVCountedPointer<CManagedTextureData> > Textures +0x48 = STLport vector start/finish/eos).
// Callee: CResourceList::CResourceList() 0x009FC570 (landed).
// Shape: the landed CTextureManager pool model: an initialiser list after the base, the array built by STLport's
//   `_Vector_base(n, a)` with n = 0 (the end-of-storage proxy word is written, then rewritten as start + n, which is the
//   +0x50, +0x48, +0x50, +0x4C store order), all through one zero register.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (folded away: the count is the constant 0)
class CResourceList {                                   // vtable 0x0129C808, 0x3C bytes
public:
    virtual ~CResourceList();
    unsigned char _body[0x3c - 4];
    CResourceList();                                    // 0x009FC570
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc()) { this->_M_finish = this->_M_start; }
};
}
template <class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned int count = 0) : std::vector<T>(count) { }
};
class CManagedTextureData;
template <class T> class CIVCountedPointer {
public:
    T* Object;                                          // +0x00
};

class CPreallocTexturePool : public CResourceList {    // vtable 0x0129DC50
public:
    unsigned long                                  Count;       // +0x3C
    unsigned long                                  MaxSize;     // +0x40
    long                                           PoolIndex;   // +0x44
    CArray<CIVCountedPointer<CManagedTextureData> > Textures;    // +0x48
    CPreallocTexturePool();
    virtual ~CPreallocTexturePool();
};

CPreallocTexturePool::CPreallocTexturePool()
    : Count(0), MaxSize(0), PoolIndex(-1)
{
}