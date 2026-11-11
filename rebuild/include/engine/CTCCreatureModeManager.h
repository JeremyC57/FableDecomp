#ifndef FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#define FABLE_ENGINE_CTCCREATUREMODEMANAGER_H
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
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
    protected:
        CThing& ParentThing;
        unsigned char WeaponBasedFilter[12]; // Original CAnimFilterOfGroup.
        long FramesBeforeRemovingSelf;bool Active;
    };
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
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager)==0x24);

#endif
