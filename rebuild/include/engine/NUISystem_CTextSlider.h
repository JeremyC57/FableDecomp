#ifndef FABLE_ENGINE_NUISYSTEM_CTEXTSLIDER_H
#define FABLE_ENGINE_NUISYSTEM_CTEXTSLIDER_H
// NUISystem::CTextSlider (new, chunk 749): RTTI .?AVCTextSlider@NUISystem@@, single direct base CChangingStateComponent
// (ghidra_out/rtti_bases.tsv: CUIInterface@0, CObserver@4, CActionParam@24). Vtables 0x01248A8C (+0, 144 slots),
// 0x01248A68 (+4, CObserver: slot 1 ProcessEvent 0x00549440), 0x01248A60 (+0x18, CActionParam). New virtuals after
// CChangingStateComponent's 141: slot 142 RevertValue 0x005491A0, slot 143 GetValueFromProfile 0x00548790.
// Name set: Ego_r.pdb / FableWin.pdb (??0..QAE@J@Z, ??1..UAE@XZ, AddAction, AddActionOnBack, AddActionOnSelected,
// AdjustValueToProfile, ChangeState(K), GetDataFromDef, GetMax, GetValueFromProfile (MAE), Initialise, OnActivate /
// OnDeactivate (MAE), ProcessEvent, RevertValue, SetPAcceptButton, Update).
// Layout: Ego_r.pdb (sizeof 468) minus 0x30 up to Actions and minus 0x34 after it (STLport vector 0xC, PDB 0x10); retail
// offsets from the landed ctor 0x00549F60 / dtor 0x00549A00 / RevertValue 0x005491A0 and ProcessEvent 0x00549440.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_EActionType.h"
namespace NUISystem {
class CAction;
class CTextSlider : public CChangingStateComponent {
public:
    CTextSlider(long);
    CTextSlider(const CTextSlider&);
    virtual ~CTextSlider();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ChangeState(unsigned long);
    virtual void SetPAcceptButton(CComponent*);
    virtual void AddAction(CCountedPointer<CAction>&);
    virtual void AddActionOnSelected(CCountedPointer<CAction>&);
    virtual void AddActionOnBack(CCountedPointer<CAction>&);
    virtual void ProcessEvent(EEvent);
    virtual void RevertValue();                                          // slot 142
    void AdjustValueToProfile();
    void GetDataFromDef();
    unsigned long GetMax();
protected:
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void GetValueFromProfile();                                  // slot 143
public:
    unsigned long SelectedChild;                                         // +0x15c
    unsigned long PreviouslySelectedChild;                               // +0x160
    unsigned long StartSelectedChild;                                    // +0x164
    bool Move;                                                           // +0x168
    std::vector<CCountedPointer<CAction> > Actions;                      // +0x16c
    std::list<CCountedPointer<CAction> >* PActionsOnBack;                // +0x178
    std::list<CCountedPointer<CAction> >* PActionsOnSelect;              // +0x17c
    float LastInputTime;                                                 // +0x180
    float InputDelay;                                                    // +0x184
    EActionType PDefAction;                                              // +0x188
    CComponent* PLeftArrow;                                              // +0x18c
    CComponent* PRightArrow;                                             // +0x190
    unsigned long OriginalSelectedChild;                                 // +0x194
    CComponent* PAcceptButton;                                           // +0x198
    unsigned long MaxChild;                                              // +0x19c
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CTextSlider) == 0x1a0);
#endif
