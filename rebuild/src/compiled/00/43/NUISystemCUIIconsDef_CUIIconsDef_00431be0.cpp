// NUISystem::CUIIconsDef::CUIIconsDef() -- default constructor of a CParentDefClassBase-derived def: CResource base out of line, then the inlined def bases (PDefManager 0, the three flag bits cleared, InstantiationName -1, an empty SubDefInfoMap), then this class's vtable
// Leaf: ??0CUIIconsDef@NUISystem@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431BE0, 51 bytes (+ int3 padding to 64), thiscall, plain `ret`, returns this.
// Identity (proven): NUISystem::CUIIconsDef::CUIIconsDef() (FableWin.pdb name set: ??0CUIIconsDef@NUISystem@@QAE@XZ); evidence: RTTI of the stored vtable 0x01230F54 is .?AVCUIIconsDef@NUISystem@@ (bases CParentDefClassBase, CDefClassBase, CDefPointeeBase, CResource, CIVCountedPointeeBase); its size slot 20 (0x00431C20) returns 0xBC = Ego_r.pdb CUIIconsDef 0xC0 less 4
// Label: the manifest's "CBlurredAttitudesDef" is propagated (family J048: five byte-similar def constructors; the stored vtable names each class; CBlurredAttitudesDef's own constructor would also build its Attitudes vector).
// Callees: CResource::CResource() 0x009FBEC0 (??0CResource@@QAE@XZ, catalog; it stores the CResource vtable and IVRefCount 1).
// Layout: Ego_r.pdb CDefClassBase (0x25, packed: PDefManager +0x1C, GlobalIndex +0x20, Setup/Template/DefaultValsApplied bits +0x24) and
//   CParentDefClassBase (InstantiationName +0x25, SubDefInfoMap +0x29: retail CVectorMap 0x10 = CArray Data, Compare +0xC, Dirty +0xD);
//   the derived def at natural alignment, so its first member lands at 0x3C (Ego_r +0x40 less the 4 bytes of the larger debug STLport vector).
//   Members per Ego_r.pdb CUIIconsDef (retail offset = Ego_r offset - 4); retail sizeof 0xBC (vtable slot 20).
// Shape: no intermediate vtable stores survive (each inlined base constructor's store is dead before the next); the members are zeroed from
//   one register and the most-derived vtable is stored last.
// Model: local -- engine/CResource.h, CDefClassBase.h and CParentDefClassBase.h flatten the vptr (data-only), so the polymorphic bases are written out here.
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
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefClassBase() : PDefManager(0)
    {
        Setup = false;
        Template = false;
        DefaultValsApplied = false;
    }
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString {
public:
    CDefString() : TablePos(-1) { }
    long TablePos;
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc> class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CVectorMap() : Dirty(false) { }
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
namespace NUISystem {
class CUIIconsDef : public CParentDefClassBase {               // vtable 0x01230F54
public:
    CUIIconsDef();
    unsigned long  IconFriendRequestReceived;               // +0x3C (Ego_r +0x40)
    unsigned long  IconFriendRequestReceivedOn;             // +0x40 (Ego_r +0x44)
    unsigned long  IconFriendRequestSent;                   // +0x44 (Ego_r +0x48)
    unsigned long  IconFriendRequestSentOn;                 // +0x48 (Ego_r +0x4C)
    unsigned long  IconGameInviteReceived;                  // +0x4C (Ego_r +0x50)
    unsigned long  IconGameInviteReceivedOn;                // +0x50 (Ego_r +0x54)
    unsigned long  IconGameInviteSent;                      // +0x54 (Ego_r +0x58)
    unsigned long  IconGameInviteSentOn;                    // +0x58 (Ego_r +0x5C)
    unsigned long  IconMute;                                // +0x5C (Ego_r +0x60)
    unsigned long  IconMuteOn;                              // +0x60 (Ego_r +0x64)
    unsigned long  IconOnline;                              // +0x64 (Ego_r +0x68)
    unsigned long  IconOnlineOn;                            // +0x68 (Ego_r +0x6C)
    unsigned long  IconPasscodeBlank;                       // +0x6C (Ego_r +0x70)
    unsigned long  IconPasscodeFilled;                      // +0x70 (Ego_r +0x74)
    unsigned long  IconTV;                                  // +0x74 (Ego_r +0x78)
    unsigned long  IconTVOn;                                // +0x78 (Ego_r +0x7C)
    unsigned long  IconVoice;                               // +0x7C (Ego_r +0x80)
    unsigned long  IconVoiceOn;                             // +0x80 (Ego_r +0x84)
    unsigned long  IconWait1;                               // +0x84 (Ego_r +0x88)
    unsigned long  IconWait2;                               // +0x88 (Ego_r +0x8C)
    unsigned long  IconWait3;                               // +0x8C (Ego_r +0x90)
    unsigned long  IconWait4;                               // +0x90 (Ego_r +0x94)
    unsigned long  IconProgress;                            // +0x94 (Ego_r +0x98)
    unsigned long  IconProgressOn;                          // +0x98 (Ego_r +0x9C)
    unsigned long  IconA;                                   // +0x9C (Ego_r +0xA0)
    unsigned long  IconB;                                   // +0xA0 (Ego_r +0xA4)
    unsigned long  IconX;                                   // +0xA4 (Ego_r +0xA8)
    unsigned long  IconY;                                   // +0xA8 (Ego_r +0xAC)
    unsigned long  IconBlank;                               // +0xAC (Ego_r +0xB0)
    unsigned long  IconUpArrow;                             // +0xB0 (Ego_r +0xB4)
    unsigned long  IconDownArrow;                           // +0xB4 (Ego_r +0xB8)
    unsigned long  IconListHighlight;                       // +0xB8 (Ego_r +0xBC)
};
CUIIconsDef::CUIIconsDef()
{
}
}   // namespace NUISystem