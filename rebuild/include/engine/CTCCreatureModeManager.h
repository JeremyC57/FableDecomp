#if !defined(FABLE_ENGINE_CTCCREATUREMODEMANAGER_H) && !defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS)
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#include "engine/CAnimFilterOfGroup.h"
#endif
#ifdef FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#include "engine/NCreatureMode.h"
#endif
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY
#include "engine/CVectorMap.h"
#endif
// Raw TPI0x27a12 and nested0x28568; native0073a6e0 reads front mode of PModes+0c.
#ifdef FABLE_RETAIL_CMODEBASE_VIRTUALS
class CWorld; class CAnimateThing; class CAnimFilterBase; class C3DVector; enum EMovementType;   // chunk 802 opt-in: the slot types
#endif
class CTCCreatureModeManager : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23
    // Both original PDB complete type records: public CThing& constructor and virtual destructor.
    explicit CTCCreatureModeManager(CThing& thing);
    virtual ~CTCCreatureModeManager();
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    class CModeBase {
    public:
#ifdef FABLE_RETAIL_CMODEBASE_CTOR
        // Retail opt-in (NF39, row 0x00742460 NCreatureMode::CModeBasic::Alloc): Ego_r.pdb / FableWin.pdb
        // ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z (the only CModeBase constructor in either PDB), out of line in retail at
        // 0x0073EAC0 (the mode factories call it on the new object with the thing). Declaration only; no layout change.
        CModeBase(CThing& parent_thing);                                                         // 0x0073EAC0
#endif
        virtual ~CModeBase();
#ifdef FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
        virtual NCreatureMode::EMode GetType()const=0;
#else
        virtual unsigned long GetType() const=0;
#endif // Native slot1, used by006680f5.
#ifdef FABLE_RETAIL_CMODEBASE_VIRTUALS
        // Retail opt-in (chunk 802, row 0x008EACB0 NCreatureMode::CModeHoverBase::OnActivate): the rest of the CModeBase vtable
        // 0x012673CC (12 slots), Ego_r.pdb / FableWin.pdb spellings (all public virtual, UAE / UBE): slot 2 AnimUpdate, 3 FrameUpdate,
        // 4 GetAnimFilter, 5 OnActivate (0x0073AAC0, called by name from the derived OnActivate), 6 OnDeactivate, 7 GetMovementSpeed,
        // 8..11 the four bool queries (slots 8 and 11 share the body `xor al,al; ret`: which of IsIdleMode / IsRemoveOnDeactivate is
        // slot 8 is not proven by the bytes; slots 9 / 10 are overridden by name in derived modes). GetWorld is
        // ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ (out of line at 0x0073A680). Declarations only: no layout change.
        virtual void AnimUpdate(CAnimateThing& animation);                                  // slot 2
        virtual void FrameUpdate();                                                               // slot 3
        virtual const CAnimFilterBase& GetAnimFilter() const;                               // slot 4
        virtual void OnActivate();                                                                // slot 5, 0x0073AAC0
        virtual void OnDeactivate();                                                              // slot 6
        virtual float GetMovementSpeed(const C3DVector& direction, const C3DVector& facing, EMovementType type) const;   // slot 7
        virtual bool IsIdleMode() const;                                                          // slot 8
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;                                // slot 9
        virtual bool IsHoldWeaponWithBothHands() const;                                           // slot 10
        virtual bool IsRemoveOnDeactivate() const;                                                // slot 11
#ifdef FABLE_RETAIL_CMODEBASE_PARENTTHING
        CThing& GetParentThing(){return ParentThing;}
#endif
        CWorld& GetWorld() const;                                                                 // 0x0073A680
#endif
#ifdef FABLE_RETAIL_CMODEBASE_NOFRAMES
    // agentFAM09 opt-in (rows 0x0121F7D0 / 0x0121F800, their dynamic initializers): Ego_r.pdb / FableWin.pdb `dynamic initializer for
    // 'CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_{COMB_IDLE,IDLE,WAVE}_ANIMS'' (0x013BAFE4 / 0x013BAFE8 / 0x013BAFEC,
    // FPS*6 / *12 / *3); private static const long per Anniv360 Test PDB (third source) ?NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS@
    // CModeBase@CTCCreatureModeManager@@0JB. Static members: no layout change.
    private:
        static const long NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS;      // 0x013BAFE4
        static const long NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS;           // 0x013BAFE8
        static const long NO_FRAMES_BEFORE_PLAYING_WAVE_ANIMS;           // 0x013BAFEC
#endif
#ifdef FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER
        // Retail opt-in (row 0x00754050 CTCLook::UpdateTurningAnims): FableWin.pdb
        // ?GetDefaultAnimFilter@CModeBase@CTCCreatureModeManager@@QBEABVCAnimFilterBase@@XZ, inline in retail (the caller passes
        // mode+8, the WeaponBasedFilter CAnimFilterOfGroup). No layout change.
        const CAnimFilterBase& GetDefaultAnimFilter() const { return *(const CAnimFilterBase*)WeaponBasedFilter; }
#endif
#ifdef FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS
    // NF124 opt-in (q6453, row 0x008D6580): Ego_r.pdb 0x00751090 / FableWin.pdb 0x024C3200
    // ?GetNoFramesBeforePlayingCombinationIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z (protected), out of line
    // at 0x0073A6F0 (the landed self row defines this spelling). Needs engine/NCreatureMode.h (EMode) first. Declaration only.
    protected:
        long GetNoFramesBeforePlayingCombinationIdleAnims(NCreatureMode::EMode mode);           // 0x0073A6F0
#endif
    protected:
        CThing& ParentThing;
#ifdef FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
        CAnimFilterOfGroup WeaponBasedFilter;
#else
        unsigned char WeaponBasedFilter[12]; // Original CAnimFilterOfGroup.
#endif
        long FramesBeforeRemovingSelf;bool Active;
    };
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY
    // Opt-in (row 0x0073B130 RegisterModes): FableWin.pdb ??0CRegisteredModeInfo@CTCCreatureModeManager@@QAE@P6IPAVCModeBase@1@AAVCThing@@@Z@Z
    // (4 bytes: AllocFunc), ?RegisterModes@CTCCreatureModeManager@@SIXXZ (Ego_r.pdb / FableWin.pdb), and the static registry
    // RegisteredModes (Ego_r / FableWin "dynamic initializer for 'CTCCreatureModeManager::RegisteredModes'", 0x013BAD44).
    class CRegisteredModeInfo {
    public:
        CRegisteredModeInfo(CModeBase* (__fastcall* allocFunc)(CThing&)) : AllocFunc(allocFunc) {}
        CModeBase* (__fastcall* AllocFunc)(CThing&);
    };
    static void __fastcall RegisterModes();                              // 0x0073B130
    static CVectorMap<unsigned long, CRegisteredModeInfo> RegisteredModes;   // 0x013BAD44
#endif
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE
    // Opt-in (NF62, row 0x0073A8E0): Ego_r.pdb ?AllocateMode@CTCCreatureModeManager@@IAE?AV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@K@Z
    // (protected). Declaration only.
protected:
    CCountedPointer<CModeBase> AllocateMode(unsigned long mode_type);   // 0x0073A8E0
public:
#endif
    CModeBase& GetCurrentMode() const;
    bool IsModeActive(unsigned long) const;
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER
    // Chunk 755 (row 0x00946190): FableWin.pdb ?AddMode@CTCCreatureModeManager@@QAE_NK@Z / ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z.
    bool AddMode(unsigned long mode);                                    // 0x0073AE10
    void RemoveMode(unsigned long mode);                                 // 0x0073ABA0
#endif
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
    // Chunk 764 (row 0x0096DBC0): Ego_r.pdb ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z
    // (the counted mode by value), out of line in retail at 0x0073ACC0.
    bool AddMode(CCountedPointer<CModeBase> mode);                       // 0x0073ACC0
#endif
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES
    // NF120 opt-in (PRIORITY q3944, row 0x0073AE70): Ego_r.pdb 0x00751960 / FableWin.pdb 0x024C2F70 ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ
    // (public; Ego_r order AddMode(K), RemoveAllModes, ~, ctor = retail 0x0073AE10, 0x0073AE70, 0x0073AEA0, 0x0073AF00). Declaration only.
    void RemoveAllModes();                                               // 0x0073AE70
#endif
private:
    std::list<CCountedPointer<CModeBase> > PModes;
    long TimeLeftForCurrentMode,TimeForNextIdleMode;
    unsigned char AvailableIdleModes[12]; // std::vector<NCreatureMode::EMode>.
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
protected:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007442A0
public:
#endif
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_FRAMEUPDATE
// agentFAM04 (2026-11-11): retail CTCBase slot 9 override (Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCCreatureModeManager@@MAEXXZ,
// protected virtual; TLC vtable 0x01268854 slot 9 = 0x0073A880). Declaration only (no layout change).
protected:
    virtual void FrameUpdate();                                          // slot 9, 0x0073A880
public:
#endif // FABLE_RETAIL_CTCCREATUREMODEMANAGER_FRAMEUPDATE
};
FABLE_STATIC_ASSERT_AT(106, sizeof(CTCCreatureModeManager)==0x24);
#if defined(FABLE_RETAIL_CMODEBASE_VIRTUALS) && defined(FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE)
// NF90 opt-in (round 7, q11189: rows 0x00739E60 and 0x008E74F0): the one body of CModeBase::OnDeactivate (Ego_r.pdb 0x0051C5E0 / FableWin.pdb
// ?OnDeactivate@CModeBase@CTCCreatureModeManager@@UAEXXZ, slot 6), inline in retail: NCreatureMode::CModeIntangible::OnDeactivate 0x008E74F0
// expands its qualified base call (`mov byte ptr [esi+18h],0` between its first two calls), and the out-of-line copy 0x00739E60 is the
// pick-any COMDAT the mode vtables reference (`mov byte ptr [ecx+18h],0 ; ret`). No layout change.
inline void CTCCreatureModeManager::CModeBase::OnDeactivate() { Active = false; }
#endif

#if defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE) && defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE_BODY)
// NF70 opt-in (round 7, row 0x0073AE10 AddMode): the one header body of AllocateMode 0x0073A8E0 (the landed row's text), for the rows of its
// retail object that call it out of line: with the body visible VC7.1 knows it only writes the returned counted pointer, so AddMode keeps the
// pointer's count block in a register (esi) across the AddMode(CCountedPointer) call and its release. `inline __declspec(noinline)`: a COMDAT
// that is never expanded. Needs engine/GFRandom.h, CWorld.h and CVectorMap.h (UPPERBOUND / COUNT) included first. No layout change.
inline __declspec(noinline) CCountedPointer<CTCCreatureModeManager::CModeBase> CTCCreatureModeManager::AllocateMode(unsigned long mode_type)
{
    long count = RegisteredModes.Count(mode_type);
    unsigned long index = GFRandom(count, PeekWorld().GetWorldSeed());
    CThing& thing = GetParentThing();
    return CCountedPointer<CModeBase>(RegisteredModes.LowerBound(mode_type)[index].second.AllocFunc(thing));
}
#endif
#endif
#ifdef FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS
// Add-only per-TU opt-in (NF462, 2026-11-26, ruling r460a on r451a / r459d): the class above without STLport, for the TUs that only
// name CTCCreatureModeManager::CModeBase or derive a mode from it (the NCreatureMode::CMode*::Alloc factories: ?Alloc@<Mode>@NCreatureMode@@
// SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z). The typed view's std::list<CCountedPointer<CModeBase> > PModes
// (stlport/fable_containers.h -> <new>, which adds /DEFAULTLIB:"libcp" to the object's .drectve; retail's objects never had it, r450a) keeps
// its name and offset here as 4 opaque bytes (TLC STLport list); every other member, the nested CModeBase (vptr, ~CModeBase, pure GetType,
// ParentThing / WeaponBasedFilter / FramesBeforeRemovingSelf / Active) and StatGetInterfaceType are the typed view's. Carries only the
// declaration-only FABLE_RETAIL_CMODEBASE_CTOR opt-in; the typed view's other opt-ins are refused.
#if defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23) \
 || defined(FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26) \
 || defined(FABLE_RETAIL_CMODEBASE_VIRTUALS) \
 || defined(FABLE_RETAIL_CMODEBASE_NOFRAMES) \
 || defined(FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER) \
 || defined(FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_VSLOTS) \
 || defined(FABLE_RETAIL_CTCCREATUREMODEMANAGER_FRAMEUPDATE)
#error "FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS carries only FABLE_RETAIL_CMODEBASE_CTOR (use the typed view for the other opt-ins)"
#endif
#ifndef FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
class CTCCreatureModeManager : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    class CModeBase {
    public:
#ifdef FABLE_RETAIL_CMODEBASE_CTOR
        CModeBase(CThing& parent_thing);                                                         // 0x0073EAC0
#endif
        virtual ~CModeBase();
        virtual unsigned long GetType() const=0;
    protected:
        CThing& ParentThing;
        unsigned char WeaponBasedFilter[12]; // Original CAnimFilterOfGroup.
        long FramesBeforeRemovingSelf;bool Active;
    };
    CModeBase& GetCurrentMode() const;
    bool IsModeActive(unsigned long) const;
private:
    unsigned char PModes[0x4];                                           // std::list<CCountedPointer<CModeBase> >
    long TimeLeftForCurrentMode,TimeForNextIdleMode;
    unsigned char AvailableIdleModes[12]; // std::vector<NCreatureMode::EMode>.
};
FABLE_STATIC_ASSERT_AT(216, sizeof(CTCCreatureModeManager)==0x24);
FABLE_STATIC_ASSERT_AT(217, sizeof(CTCCreatureModeManager::CModeBase)==0x1c);
#endif
#endif // FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS
