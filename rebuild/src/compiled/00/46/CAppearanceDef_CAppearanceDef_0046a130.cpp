// CAppearanceDef::CAppearanceDef() -- default constructor: the CSubDefClassBase base out of line, the two CEngineGraphic members zeroed inline, the animation set constructed out of line; the remaining members are left for the def loader
// Leaf: ??0CAppearanceDef@@QAE@XZ
// Retail: Fable.exe @ 0x0046A130, 68 bytes, __fastcall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012354DC (RTTI .?AVCAppearanceDef@@, bases CSubDefClassBase / CDefClassBase /
// CDefPointeeBase / CResource); its only reference is CAppearanceDef::Alloc's tail jump at 0x0046CDF4 (`push 78h ; call
// operator new ; ... ; mov ecx,eax ; jmp 0x0046A130`); Ego_r.pdb / FableWin.pdb ??0CAppearanceDef@@QAE@XZ.
// Callees: CSubDefClassBase::CSubDefClassBase() @ 0x0044C0C0 (stores 0x01231D54), CAnimationSet::CAnimationSet() @ 0x005DD2E0
// (Animation, +0x34).
// Layout: the model of the landed CAppearanceDef ??_G 0x0046CDFC (Ego_r.pdb members rebased after the 16-byte retail
// CVectorMap: Graphic +0x25 (packed 14-byte CEngineGraphic), Animation +0x34, FadeOutWhenCloseGraphic +0x5C; size 0x78).
// Shape: size-optimised TU (#pragma optimize("s", on), as the CAppearanceDef ??_G 0x0046CDFC / Alloc 0x0046CDE6 of the
// same class): /Os zeroes the float members with `fldz ; fstp` (the speed build stores integer zeroes). Member initialisation in declaration order (CEngineGraphic(): BankIndex, AnimStep, RenderSizeX, AdditiveAlpha,
// Type = 0); VC7.1 schedules the vtable store and the x87 zero stores (`fldz ; fstp`) ahead of the integer zeroes held
// in ebx.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }                       // user-written, inline: stores 0x01230BA0, then ~CResource
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC (implicit destructor)
public:
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members, implicit destructor)
public:
    CSubDefClassBase();                                  // 0x0044C0C0
};
class CEngineGraphic {                                   // Ego_r.pdb 14 bytes, packed
public:
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
    bool          AdditiveAlpha;                         // +0x0C
    unsigned char Type;                                  // +0x0D (EEngineGraphicType)
};
#pragma pack(pop)
class CAnimationEntry;
class CAnimationSet {                                    // Ego_r.pdb 36 bytes, retail 32 (the CVectorMap 16)
public:
    CAnimationSet();                                     // 0x005DD2E0
    CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > Anims;   // +0x00
    long       DefaultFlags;                             // +0x10
    long       DefaultTransitionTime;                    // +0x14
    long       DefaultDelay;                             // +0x18
    CDefString DefaultGroup;                             // +0x1C
};
enum ELightingChannel { };                               // Ego_r.pdb (enumerators not needed)
class CAppearanceDef : public CSubDefClassBase {         // vtable 0x012354DC, retail 0x78 (size slot 20)
public:
    CEngineGraphic   Graphic;                            // +0x25
    CAnimationSet    Animation;                          // +0x34
    CAppearanceDef();
    unsigned char    OutlineEffectAlphaRef;              // +0x54 (Ego_r +0x58)
    unsigned char    OutlineEffectAlphaBias;             // +0x55
    bool             OverrideLightingChannel;            // +0x56
    ELightingChannel LightingChannel;                    // +0x58
    CEngineGraphic   FadeOutWhenCloseGraphic;            // +0x5C
    bool             HasFadeOutWhenCloseGraphic;         // +0x6A
    float            FadeOutGraphicRenderFadeDistance;   // +0x6C
    float            FadeOutGraphicNearRenderFadeDistance;   // +0x70
    float            FadeOutGraphicNearRenderClipDistance;   // +0x74
};
CAppearanceDef::CAppearanceDef()
{
}