#ifndef FABLE_ENGINE_CANIMATIONENTRY_H
#define FABLE_ENGINE_CANIMATIONENTRY_H
// Hand-written (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): one animation of an animation bank. Ego_r.pdb layout
// (16 bytes): BankIndex +0x00, AnimName +0x04 / GroupName +0x08 (CDefString), Components +0x0c (CIVCountedPointer<CAnimComponentMap>).
// The three registries are static members (Ego_r.pdb `dynamic initializer for 'CAnimationEntry::Registered...'`):
//   RegisteredComponentsByToken 0x013B8C1C  map<CCharString, CIVCountedPointer<CAnimComponentBase> > (STLport _Rb_tree, 12 bytes);
//   RegisteredComponentsByType  0x013B8C28  CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > (0x10 bytes);
//   RegisteredComponentDefaults 0x013B8C38  the same type (reloc_bindings.tsv spells the two CVectorMaps).
// Needs FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
class CAnimComponentMap;
class CAnimationEntry {
public:
#ifdef FABLE_RETAIL_CANIMATIONENTRY_SEAT_ASSIGN_PASS27
    CAnimationEntry& operator=(const CAnimationEntry&);
#endif
#ifdef FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
    // Both PDBs original const out-pointer template, retail00867fe0 for FollowingOnAnim.
    template<class T> void GetComponent(const T** out)const;
#endif
    // Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CAnimationEntry@@SIXXZ (static __fastcall; retail 0x005DD670, one caller 0x0044E907).
#ifdef FABLE_RETAIL_EXPL_RESPONSE_PASS29
    template<class T> void GetComponent(T** out);
    static void __fastcall StatRegisterClass();
#else
    static void __fastcall StatRegisterClass();
#endif
    static std::map<CCharString, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentsByToken;          // 0x013B8C1C
    static CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentsByType;      // 0x013B8C28
    static CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentDefaults;     // 0x013B8C38
#ifdef FABLE_RETAIL_CANIMATIONENTRY_TRANSITION_CTOR
    // NF71 opt-in (round 7, row 0x006A0150 CCreatureAction_UnsheatheItemFromInventory ctor): Ego_r.pdb / FableWin.pdb
    // ??0CAnimationEntry@@QAE@ABV0@J@Z (retail 0x005DEC70, called out of line: the copy with a new transition-in time). Declaration only.
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime);   // 0x005DEC70
#endif
#ifdef FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
    // Retail opt-in (chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): Ego_r.pdb / FableWin.pdb
    // ?GetAnimateThingFlags@CAnimationEntry@@QBEKXZ (retail 0x005DC390) and ?GetTransitionTime@CAnimationEntry@@QBEJXZ (0x005DC1D0),
    // both out of line. Declarations only.
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionTime() const;                            // 0x005DC1D0
#endif
#ifdef FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
    // Opt-in (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): Ego_r.pdb / FableWin.pdb ?GetDelayTime@CAnimationEntry@@QBEJXZ
    // (out of line, 0x005DC1F0) and ?GetTimeToBlendIn@CAnimationEntry@@QBEJABVCThing@@@Z (0x005DC230; the creature itself is pushed).
    // Declarations only.
    long GetDelayTime() const;                                 // 0x005DC1F0
    long GetTimeToBlendIn(const class CThing& thing) const;    // 0x005DC230
#endif
#ifdef FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS
    // Retail opt-in (agentNF9, row 0x00663140 CThingCreatureBase::PreloadAnims): Ego_r.pdb / FableWin.pdb ?GetFlags@CAnimationEntry@@QBEJXZ
    // (public const, out of line at 0x005DC210; the row tests `& 4` on its result). Declaration only.
    long GetFlags() const;                                     // 0x005DC210
#endif
#ifdef FABLE_RETAIL_CANIMATIONENTRY_GETROTATION
    // Retail opt-in (row 0x00754050): Ego_r.pdb / FableWin.pdb ?GetRotationInAnimation@CAnimationEntry@@QBEXAAM@Z (out of line
    // 0x005DBFD0). Declaration only.
    void GetRotationInAnimation(float& rotation) const;       // 0x005DBFD0
#endif
    long                                   BankIndex;     // +0x00
    CDefString                             AnimName;      // +0x04
    CDefString                             GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap>   Components;    // +0x0c
};
FABLE_STATIC_ASSERT_AT(58, sizeof(CAnimationEntry) == 0x10);
#if defined(FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT) && defined(FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY)
// p25E opt-in (row 0x00859EF0 CCombatActionBase::OnAnimationLoaded): the GetComponent<T> template body, visible in the TU as Lionhead's
// header template was (retail calls the instances out of line: 0x00669CB0 <CAnimComponentCombatHandedness>, 0x00738A00
// <CAnimComponentCombatMisc>, the in-TU 0x0085AD30 <CAnimComponentStrikeSpeedMultiplier>, family G012). With the body visible VC7.1 knows the
// caller's out-pointer local does not escape and shares its frame word (0x0085A151 / 0x00859FA2 both `lea edx,[esp+1Ch]`); with the
// declaration alone each out-local keeps its own word (frame 0x28 v retail 0x20). The body is the landed G012 shape: the own map's Find,
// else RegisteredComponentDefaults' Find, then the component through the out pointer; compiled from this header with
// FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND (the non-const Find's original positive-arm form) the <CAnimComponentCombatHandedness> and
// <CAnimComponentCombatMisc> instances grade RELOCATION_MATCH against retail 0x00669CB0 / 0x00738A00 (125 bytes each). Ego_d.pdb's
// record of the instance names typ / panim_comp_base / it, pp_component the parameter.
// CAnimComponentMap: Ego_d.pdb LF_CLASS 0x19884 (field list 0x19883: public base CIVCountedPointeeBase, public Map +0x08, compiler-generated
// virtual dtor; Ego_r.pdb sizeof 28 with the 0x10 Dinkumware vector, retail 0x18). Needs engine/CAnimComponentBase.h (EAnimComponent, the
// RETAIL_ANIMCOMPONENT StatGetType of T) and FABLE_RETAIL_CIVCOUNTEDPOINTER_GET.
class CAnimComponentMap : public CIVCountedPointeeBase {
public:
    CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > Map;     // +0x08
};
template<class T> void CAnimationEntry::GetComponent(const T** pp_component) const
{
    EAnimComponent typ = T::StatGetType();
    CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::const_iterator it;
    if (Components.IsNull() || (it = Components->Map.Find(typ)) == Components->Map.end())
        it = RegisteredComponentDefaults.Find(typ);
    const CAnimComponentBase* panim_comp_base = it->second.Get();
    *pp_component = static_cast<const T*>(panim_comp_base);
}
#endif
#ifdef FABLE_RETAIL_EXPL_RESPONSE_PASS29
template<class T> void CAnimationEntry::GetComponent(T** pp_component) {
    EAnimComponent typ=T::StatGetType();
    CVectorMap<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >::iterator it;
    if (Components.IsNull() || (it=Components->Map.Find(typ))==Components->Map.end())
        it=RegisteredComponentDefaults.Find(typ);
    CAnimComponentBase* panim_comp_base=it->second.Get();
    *pp_component=static_cast<T*>(panim_comp_base);
}
#endif
#endif // FABLE_ENGINE_CANIMATIONENTRY_H
