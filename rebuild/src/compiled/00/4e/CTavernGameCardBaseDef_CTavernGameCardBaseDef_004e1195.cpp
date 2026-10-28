// CTavernGameCardBaseDef::CTavernGameCardBaseDef() -- def default constructor: CSubDefClassBase base, vtable, then the card-order and card-pack vectors emptied
// Leaf: ??0CTavernGameCardBaseDef@@QAE@XZ
// Retail: Fable.exe @ 0x004E1195, 40 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x0124258C is RTTI .?AVCTavernGameCardBaseDef@@; PDB name set
// `??0CTavernGameCardBaseDef@@QAE@XZ`; its only caller is the def factory thunk at 0x004E2DC3 (`jmp`). The catalog label
// CAIStateGroup_HoverHornetQueen is propagated.
// Callee: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Layout: Ego_r.pdb CTavernGameCardBaseDef: CardOrders vector<CCardPositionsDef> +0x6C, CardPack vector<CCardDef>
// Ego_r +0x7C -> retail +0x78 (STLport vector 12 bytes). engine/CTavernGameCardBaseDef.h is data-only (flattened
// vptr, Dinkumware vector sizes), so the class is modelled locally.
// Shape: STLport 4.6 vector() inlined twice; VC7.1 stores the first through esi and the second through `lea eax`.
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
class CCardPositionsDef;
class CCardDef;
class CTavernGameCardBaseDef : public CSubDefClassBase { public:
    CTavernGameCardBaseDef();
    unsigned char                     _pad25[0x47];     // ControlType .. PackOffset (+0x28 .. +0x6B)
    std::vector<CCardPositionsDef>    CardOrders;       // +0x6C
    std::vector<CCardDef>             CardPack;         // +0x78
};
CTavernGameCardBaseDef::CTavernGameCardBaseDef()
{
}