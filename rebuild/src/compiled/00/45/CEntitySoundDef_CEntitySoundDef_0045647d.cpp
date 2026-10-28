// CEntitySoundDef::CEntitySoundDef() -- def default constructor: CSubDefClassBase base, vtable, the sound map's two vector maps emptied, then the sound identifier string
// Leaf: ??0CEntitySoundDef@@QAE@XZ
// Retail: Fable.exe @ 0x0045647D, 52 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01233A44 is RTTI .?AVCEntitySoundDef@@; PDB name set `??0CEntitySoundDef@@QAE@XZ`; only
// caller the def factory thunk at 0x0045CDFE.
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0; CCharString::CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CEntitySoundDef: SoundMap (CSoundMap) +0x28 = SoundMap CVectorMap<unsigned long,CSoundPair> +0x28 and
// CriteriaMap CVectorMap<unsigned long,CDefString> +0x38 (retail CVectorMap 0x10: CArray +0, Compare +0xC, Dirty +0xD),
// SoundIdentifier +0x48 (Ego_r +0x50), VoicePitchOverride +0x4C, AnimCriteriaClipDistance +0x50.
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
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
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
template <class T> class CArray : public std::vector<T> { public: };
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class TCompare> class CVectorMap { public:   // retail 0x10 bytes
    CVectorMap() : Dirty(false) { }
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                  Compare;                   // +0x0C
    bool                      Dirty;                     // +0x0D
};
class CSoundPair;
class CDefString { public: long TablePos; };
class CSoundMap { public:
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> > SoundMap;       // +0x00
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> > CriteriaMap;    // +0x10
};
class CEntitySoundDef : public CSubDefClassBase { public:
    CEntitySoundDef();
    CSoundMap   SoundMap;                                // +0x28
    CCharString SoundIdentifier;                         // +0x48
    float       VoicePitchOverride;                      // +0x4C
    long        AnimCriteriaClipDistance;                // +0x50
};
CEntitySoundDef::CEntitySoundDef()
{
}