// CThingMarkerDef::CThingMarkerDef() -- default constructor of a def whose only constructed member is its CEngineGraphic: the CThingBaseDef base out of line, this class's vtable, then Graphic zeroed (bank index, animation step, render size, additive-alpha flag, type)
// Leaf: ??0CThingMarkerDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454DF4, 39 bytes, thiscall, plain `ret`, returns this (a size-optimised /Os def module: unaligned entry).
// Identity (proven): CThingMarkerDef::CThingMarkerDef() (FableWin.pdb name set: ??0CThingMarkerDef@@QAE@XZ); evidence: RTTI of the stored vtable 0x01233264 is .?AVCThingMarkerDef@@ (base CThingBaseDef); it zeroes Graphic at +0x70 (Ego_r.pdb CThingMarkerDef::Graphic +116)
// Label: the manifest's "CSimVoice" is propagated (family J056: five byte-similar def constructors; the stored vtable names each class).
// Callees: CThingBaseDef::CThingBaseDef() 0x006772D0 (??0CThingBaseDef@@QAE@XZ, FableWin.pdb; it stores the CThingBaseDef vtable 0x0125B664).
// Layout: Ego_r.pdb CThingMarkerDef (Graphic +0x70, retail = Ego_r - 4 after the smaller retail CVectorMap); CEngineGraphic is a packed 14-byte class (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type).
// Model: local -- engine/CResource.h, CDefClassBase.h and CParentDefClassBase.h flatten the vptr (data-only); there is no header for this def or CThingBaseDef.
#pragma optimize("s", on)
class CResourceList;
class CDefinitionManager;
#pragma pack(push, 1)
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
class CDefString {
public:
    long TablePos;
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
#pragma pack(push, 1)
// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0);
//   FABLE_PDCB_TYPED_MEMBERS: InstantiationName / SubDefInfoMap are typed with this row's CDefString / CVectorMap.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
class CThingBaseDef : public CParentDefClassBase {       // vtable 0x0125B664, Ego_r.pdb 0x74 (retail 0x70), packed
public:
    CThingBaseDef();                                     // 0x006772D0
    unsigned char  Components[8];                    // +0x39 CThingComponentSet (CSmallVector<CThingComponentSet::CEntry, 8>)
    bool           AvailableInEditor : 1;               // +0x41 (Ego_r +0x45)
    bool           AlwaysFrameUpdate : 1;               // +0x41 (Ego_r +0x45)
    bool           StartKillLocked : 1;                 // +0x41 (Ego_r +0x45)
    bool           LevelLess : 1;                       // +0x41 (Ego_r +0x45)
    bool           IsDrawable : 1;                      // +0x41 (Ego_r +0x45)
    bool           IsDrawableInEditor : 1;              // +0x41 (Ego_r +0x45)
    bool           ForceNoSerialise : 1;                // +0x41 (Ego_r +0x45)
    bool           InMapSearches : 1;                   // +0x41 (Ego_r +0x45)
    bool           IsLoadableGlobal : 1;                // +0x42 (Ego_r +0x46)
    bool           GameStatePersisted : 1;              // +0x42 (Ego_r +0x46)
    bool           CanComeBetweenCameraAndObservedThing : 1; // +0x42 (Ego_r +0x46)
    bool           DrawWeaponTrails : 1;                // +0x42 (Ego_r +0x46)
    bool           DrawProjectileWeaponTrails : 1;      // +0x42 (Ego_r +0x46)
    bool           IsVulnerableToMelee : 1;             // +0x42 (Ego_r +0x46)
    bool           Damageable : 1;                      // +0x42 (Ego_r +0x46)
    bool           AlwaysIncludeInObjectStrikeScans : 1; // +0x42 (Ego_r +0x46)
    bool           DisableOcclusionTesting : 1;         // +0x43 (Ego_r +0x47)
    bool           DrawAfterWater : 1;                  // +0x43 (Ego_r +0x47)
    bool           ForceRenderedLastFrameOverride : 1;  // +0x43 (Ego_r +0x47)
    bool           AddToComboMultiplierOnHit : 1;       // +0x43 (Ego_r +0x47)
    bool           GiveHeroStatChangesOnBeingHit : 1;   // +0x43 (Ego_r +0x47)
    long           GroupDef;                            // +0x44 (Ego_r +0x48)
    float          RenderFadeDistance;                  // +0x48 (Ego_r +0x4C)
    long           PersistenceFlags;                    // +0x4C (Ego_r +0x50)
    float          Health;                              // +0x50 (Ego_r +0x54)
    float          MinHealth;                           // +0x54 (Ego_r +0x58)
    float          MeshHeightOffset;                    // +0x58 (Ego_r +0x5C)
    float          MeshHeight;                          // +0x5C (Ego_r +0x60)
    float          MeshRadius;                          // +0x60 (Ego_r +0x64)
    float          ApproxMaxMeshHeight;                 // +0x64 (Ego_r +0x68)
    CDefString     DefaultScriptName;                   // +0x68 (Ego_r +0x6C)
    CDefString     DefaultScriptData;                   // +0x6C (Ego_r +0x70)
};
#pragma pack(pop)
enum ESwitchTriggerType { };                      // Ego_r.pdb (enumerators not needed)
class CThingMarkerDef : public CThingBaseDef {                     // vtable 0x01233264
public:
    CThingMarkerDef();
    CEngineGraphic Graphic;                             // +0x70 (Ego_r +0x74)
    ESwitchTriggerType TriggerType;                         // +0x80 (Ego_r +0x84)
    unsigned long  Properties;                          // +0x84 (Ego_r +0x88)
};
CThingMarkerDef::CThingMarkerDef()
{
}