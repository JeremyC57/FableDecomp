// CObjectAugmentationsDef::CObjectAugmentationsDef() -- def default constructor: CSubDefClassBase base, vtable, the particle map, two vectors emptied, the two sound maps and the trail attachment-point string
// Leaf: ??0CObjectAugmentationsDef@@QAE@XZ
// Retail: Fable.exe @ 0x004EBBA3, 73 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01243974 is RTTI .?AVCObjectAugmentationsDef@@; PDB name set
// `??0CObjectAugmentationsDef@@QAE@XZ`; only caller the def factory thunk at 0x004EC537. The catalog label
// CEngineVSConstantLayoutLightsBones is propagated.
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0; map<EObjectAugmentationType,CObjectAugmentationParticleSet>::map()
// 0x004EBAB0; map<EObjectAugmentationType,CDefString>::map() 0x004DF8AD (twice); CCharString::CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CObjectAugmentationsDef, retail = Ego_r - 4 per STLport map/vector before the member:
// AugmentationParticles +0x30, EmptySlotName +0x3C, InitialAugmentations +0x40, SlotUpgradeCosts +0x4C, InitSounds +0x58,
// LoopingSounds +0x64, the six effect indices +0x70..+0x84, WeaponTrailAttachmentPoint +0x88.
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
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:                                      // STLport 4.6 map: one _Rb_tree (header, node count, compare), 12 bytes
    map();                                               // out of line
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
};
}
#pragma pack(push, 1)
class CSubDefClassBase { public:                         // packed 0x25 bytes in retail (CDefClassBase ends in a bit-field byte)
    CSubDefClassBase();                                  // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _pad04[0x21];                          // CResource / CDefClassBase members, packed to 0x25
};
#pragma pack(pop)
class CCharString { public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();
    void* PStringData;
};
enum EObjectAugmentationType { };
class CObjectAugmentationParticleSet;
class CDefString { public: long TablePos; };
class CObjectAugmentationsDef : public CSubDefClassBase { public:
    CObjectAugmentationsDef();
    long                                                               NumberOfSlots;                          // +0x28
    long                                                               MaximumNumberOfSlots;                   // +0x2C
    std::map<EObjectAugmentationType, CObjectAugmentationParticleSet>  AugmentationParticles;                  // +0x30
    unsigned long                                                      EmptySlotName;                          // +0x3C
    std::vector<long>                                                  InitialAugmentations;                   // +0x40
    std::vector<long>                                                  SlotUpgradeCosts;                       // +0x4C
    std::map<EObjectAugmentationType, CDefString>                      InitSounds;                             // +0x58
    std::map<EObjectAugmentationType, CDefString>                      LoopingSounds;                          // +0x64
    long                                                               FireProjectileTrailEffect;              // +0x70
    long                                                               LightningProjectileTrailEffect;         // +0x74
    long                                                               FireAndLightningProjectileTrailEffect;  // +0x78
    long                                                               FireProjectileOnHitEffect;              // +0x7C
    long                                                               LightningProjectileOnHitEffect;         // +0x80
    long                                                               FireAndLightningProjectileOnHitEffect;  // +0x84
    CCharString                                                        WeaponTrailAttachmentPoint;             // +0x88
};
CObjectAugmentationsDef::CObjectAugmentationsDef()
{
}