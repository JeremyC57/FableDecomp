// CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsRoundDef>::Resize(long n) -- resize the wrapped vector, new elements default-constructed
// Leaf: ?Resize@?$CPersistContext_TransferableVectorOfSubComponents@VCHeroSoulsRoundDef@@@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f35970, 99 bytes, thiscall, `ret 4` (virtual, wrapper vtable slot 1).
// Identity (proven): slot 1 of the transferable-vector wrapper vtable 0x01301C7C (the landed Transfer<...> rows hand that {vtable, vector*} wrapper to the sub-component text loader 0x009BC120); Ego_r.pdb names the slot ?Resize@?$CPersistContext_TransferableVectorOfSubComponents@VCHeroSoulsRoundDef@@@@EAEXJ@Z; the default element it builds stores the def vtable 0x012FB430 (CHeroSoulsRoundDef).
// Callees: CDefClassBase::CDefClassBase() 0x0042D9E0, _STLP_alloc_proxy<CHeroSoulsRoundDef...>(const allocator&, pointer) 0x00F359E0 (the member vector's
//   end-of-storage proxy), vector<CHeroSoulsRoundDef>::resize(size_type, const CHeroSoulsRoundDef&) 0x00F35450, vector<CHeroSoulsWaveDef>::~vector 0x00F26E20,
//   CResource::~CResource 0x009FC550 (after the inline CDefPointeeBase vtable store 0x01230BA0).
// Shape (family L149): T's implicit default constructor and the member vector's constructor chain are inline down to the
//   allocator proxy, whose (declared, out-of-line) constructor retail calls; the vector pointer is read before the
//   temporary is built (a named local); the temporary's implicit destructor is inline as in family L148 (no user
//   destructor: a declared one stores the derived vtable first).
class CResource {                                   // RTTI base of CDefPointeeBase
public:
    virtual ~CResource();                            // 0x009FC550
    unsigned char _res[0x18];
};
class CDefPointeeBase : public CResource {           // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {       // Ego_r.pdb 0x25 bytes
public:
    CDefClassBase();                                 // 0x0042D9E0
    CDefinitionManager* PDefManager;                 // +0x1c
    unsigned long       GlobalIndex;                 // +0x20
    unsigned char       Flags;                       // +0x24
};
class CHeroSoulsWaveDef;
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p);   // out of line (0x00F359E0)
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector();                                        // out of line
    void resize(size_type __new_size, const _Tp& __x);   // out of line
};
}
class CHeroSoulsRoundDef : public CDefClassBase {                 // retail 0x38 bytes, vtable 0x012FB430
public:
    long  NumWaves;                                   // +0x28
    std::vector<CHeroSoulsWaveDef> Waves;                     // +0x2c
};
class CPersistContext;
class CPersistContext_TransferableVectorBase {
public:
    virtual long Size() = 0;                         // slot 0
};
template <class T> class CPersistContext_TransferableVectorOfSubComponents : public CPersistContext_TransferableVectorBase {
public:
    std::vector<T>* PVector;                         // +0x04
private:
    virtual long Size();
    virtual void Resize(long n);                     // slot 1
};
template <class T> void CPersistContext_TransferableVectorOfSubComponents<T>::Resize(long n)
{
    std::vector<T>* vec = PVector;
    vec->resize(n, T());
}
template void CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsRoundDef>::Resize(long);