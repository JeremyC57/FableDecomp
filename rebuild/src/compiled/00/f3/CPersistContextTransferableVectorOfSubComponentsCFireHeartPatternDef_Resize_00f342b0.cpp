// CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize(long count) -- `TransVector->resize(count)`: build a default CFireHeartPatternDef, hand it to the out-of-line vector resize(n, x), destroy it
// Leaf: ?Resize@?$CPersistContext_TransferableVectorOfSubComponents@VCFireHeartPatternDef@@@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F342B0, 114 bytes (+ int3 padding), __fastcall `ret 4`; slot 1 of the no-RTTI
//   vtable 0x01301C40 (slots Size 0x00F33D00, Resize, AreDefaultValsApplied 0x00F33D20, SetDefaultValsApplied 0x00F33D40,
//   Transfer 0x00F33D70 -- all landed). Catalog "CRandomAppearanceMorph_TextureMorphs_Init" and the manifest's
//   vector<CVectorMap<..CTextureMorph..>>::resize label on 0x00F33DE0 are propagated: the 0x34 stride and the
//   vector<CFireHeartPatternDef>::erase 0x00F33E50 it calls make 0x00F33DE0 vector<CFireHeartPatternDef>::resize(n, x).
// Callees: CDefClassBase::CDefClassBase @ 0x0042D9E0 (landed), _STLP_alloc_proxy<bool*, bool, allocator<bool> >(const
//   allocator<bool>&, bool*) @ 0x0045036F (out of line, `mov [this],p ; ret 8`), vector<CFireHeartPatternDef>::resize(n,
//   const T&) @ 0x00F33DE0, free @ 0x00BFEA14 (the temporary's PadActive storage), CResource::~CResource @ 0x009FC550.
// Layout: the temporary is the 0x34-byte CFireHeartPatternDef (vtable 0x012CCC50): CDefClassBase 0x25 bytes byte-packed
//   (landed 0x0042D9E0 model: CResource <- CDefPointeeBase <- CDefClassBase) and PadActive, the retail 12-byte
//   vector<bool> (byte storage, landed CPlayerInventoryDef::operator= 0x00462E2D), at +0x28; the allocator temporary is
//   the byte at the top of the frame's first word (`lea eax,[esp+0Bh]`).
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`, `__forceinline` (plain inline, VC7.1 keeps it out of line and
//   Resize becomes a `jmp`); the temporary's implicit default constructor calls the
//   out-of-line CDefClassBase(), stores its vtable and zeroes _M_start/_M_finish around the proxy call; its implicit
//   destructor frees the vector storage and runs the inline user ~CDefPointeeBase() { } (vtable 0x01230BA0 store, then
//   ~CResource) -- the implicit destructors of CDefClassBase/CFireHeartPatternDef store no vtable.
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14

struct CDefinitionManager;
struct CPersistContext;

struct CResource {
    unsigned char _resource[0x18];                       // +0x04 resource list links, size, last used frame
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
};

struct CDefPointeeBase : CResource {                     // vtable 0x01230BA0
    CDefPointeeBase() { }
    virtual ~CDefPointeeBase() { }
};

#pragma pack(push, 1)                                    // def classes are byte packed (the bits at +0x24)
struct CDefClassBase : CDefPointeeBase {                 // vtable 0x01230BCC, 0x25 bytes
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24 bit 0
    unsigned char       Template : 1;                    // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;          // +0x24 bit 2
    CDefClassBase();                                     // 0x0042D9E0
};
#pragma pack(pop)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p);   // 0x0045036F (out of line in this TU)
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    void resize(size_type __new_size, const _Tp& __x);   // 0x00F33DE0 for CFireHeartPatternDef
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

}   // namespace std

class CFireHeartPatternDef : public CDefClassBase {      // vtable 0x012CCC50, 0x34 bytes in retail
public:
    std::vector<bool> PadActive;                        // +0x28
};

struct CPersistContext_TransferableVectorOfSubComponentsGenericType {
    virtual long Size() = 0;
    virtual void Resize(long count) = 0;
    virtual bool AreDefaultValsApplied(long index) = 0;
    virtual void SetDefaultValsApplied(long index, bool applied) = 0;
    virtual void Transfer(CPersistContext& context, long index) = 0;
};

template <class T> struct CPersistContext_TransferableVectorOfSubComponents : CPersistContext_TransferableVectorOfSubComponentsGenericType {
    std::vector<T>* TransVector;                        // +0x4
    virtual long Size();
    virtual void Resize(long count);
    virtual bool AreDefaultValsApplied(long index);
    virtual void SetDefaultValsApplied(long index, bool applied);
    virtual void Transfer(CPersistContext& context, long index);
};

template <class T>
void CPersistContext_TransferableVectorOfSubComponents<T>::Resize(long count)
{
    TransVector->resize(count);
}

template void CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize(long);