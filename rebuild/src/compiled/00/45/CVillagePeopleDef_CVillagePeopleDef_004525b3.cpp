// CVillagePeopleDef::CVillagePeopleDef() -- def default constructor: CSubDefClassBase base, vtable, then the two guard-respawn vectors emptied
// Leaf: ??0CVillagePeopleDef@@QAE@XZ
// Retail: Fable.exe @ 0x004525B3, 48 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01232C4C is RTTI .?AVCVillagePeopleDef@@ (rtti-vtables.tsv); PDB name set
// `??0CVillagePeopleDef@@QAE@XZ`; its only caller is the def factory thunk at 0x00453AF1 (`jmp`). The catalog
// label CAIStateGroup_HoverHornetQueen is propagated across a byte-similar body.
// Callee: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Layout: Ego_r.pdb CVillagePeopleDef: GuardRespawnType vector<long> +0xA8, GuardRespawnQuantity vector<long> Ego_r
// +0xB8 -> retail +0xB4 (STLport vector 12 bytes). The scalar members are left uninitialised by the default ctor.
// Shape: STLport 4.6 vector() -> _Vector_base(a): _M_start(0), _M_finish(0), _M_end_of_storage(a, 0); inlined
// through each vector's `this` (lea eax,[esi+N]).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
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
#pragma pack(push, 1)
class CSubDefClassBase { public:                         // packed 0x25 bytes in retail (CDefClassBase ends in a bit-field byte)
    CSubDefClassBase();                                  // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _pad04[0x21];                          // CResource / CDefClassBase members, packed to 0x25
};
#pragma pack(pop)
class CVillagePeopleDef : public CSubDefClassBase { public:
    CVillagePeopleDef();
    unsigned char      _pad25[0x83];                     // Enable .. NumGhosts (+0x25 .. +0xA7)
    std::vector<long>  GuardRespawnType;                 // +0xA8
    std::vector<long>  GuardRespawnQuantity;             // +0xB4
    long               GuardRespawnMaxRenownBasedOffset; // +0xC0
};
CVillagePeopleDef::CVillagePeopleDef()
{
}