// CAppearanceDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAppearanceDef inlined (the member's destructor, then the base chain down to the first user-written destructor), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAppearanceDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046CDFC, 44 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): CAppearanceDef::`scalar deleting destructor'; evidence: vtable 0x012354DC slot 0 (RTTI .?AVCAppearanceDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase)
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies (family J065: five deleting destructors of five unrelated classes).
// Callees: CAnimationSet::~CAnimationSet 0x00461F3B (Animation, +0x34), CResource::~CResource 0x009FC550 (after the inline user-written ~CDefPointeeBase stores 0x01230BA0), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CAppearanceDef members, rebased after the 16-byte retail CVectorMap inside CAnimationSet; retail size 0x78 from the def's size slot (vtable slot 20 returns 0x78).
// Shape: no vtable store for CAppearanceDef itself -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the stored
//   vtable 0x01230BA0 belongs to the first user-written (inline) base destructor. The deleting destructor is emitted with the vtable, i.e. in a TU
//   that constructs the class: MakeOne is that construction (a modelling device); the qualified explicit destructor calls make VC7.1 define the
//   implicit destructors before it generates the deleting destructor, so the chain inlines.
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
    CAnimationSet();
    ~CAnimationSet();                                    // 0x00461F3B
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
void* MakeOne()
{
    return new CAppearanceDef;
}
void KillDefBase(CDefClassBase* p) { p->CDefClassBase::~CDefClassBase(); }
void KillSubDef(CSubDefClassBase* p) { p->CSubDefClassBase::~CSubDefClassBase(); }
void KillOne(CAppearanceDef* p) { p->~CAppearanceDef(); }