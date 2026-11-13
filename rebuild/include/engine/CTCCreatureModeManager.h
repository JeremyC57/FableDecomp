#ifndef FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#include "engine/CTCBase.h"
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
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_MODE_MANAGER; }
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const=0; // Native slot1, used by006680f5.
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
    protected:
        CThing& ParentThing;
        unsigned char WeaponBasedFilter[12]; // Original CAnimFilterOfGroup.
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
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager)==0x24);

#endif
