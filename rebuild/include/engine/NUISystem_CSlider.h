#ifndef FABLE_ENGINE_NUISYSTEM_CSLIDER_H
#define FABLE_ENGINE_NUISYSTEM_CSLIDER_H
#ifndef FABLE_RETAIL_C2DVECTOR
#error "engine/NUISystem_CSlider.h needs the byte-packed retail C2DVector: define FABLE_RETAIL_C2DVECTOR"
#endif
// NUISystem::CSlider (new, chunk 752): RTTI .?AVCSlider@NUISystem@@, single direct base NUISystem::CSprite (rtti_bases.tsv:
// CUIInterface@0, CObserver@4, CActionParam@24). Vtables 0x01248D04 (+0), 0x01248CE0 (+4, CObserver: slot 0 the ??_E
// adjustor 0x0054C140, slot 1 ProcessEvent 0x0054AC90), 0x01248CD8 (+0x18, CActionParam: slot 0 the adjustor 0x0054C150).
// Overrides (primary vtable): 1 Update 0x0054BA20, 7 SetPAcceptButton 0x0054C130, 43 Initialise 0x0054A5D0, 48 ChangeState(K)
// 0x0054A990, 66 AddAction 0x0054C160, 67 AddActionOnSelected 0x0054B2D0, 68 AddActionOnBack 0x0054B240; new virtuals after
// CSprite's 148: 149 RevertValue 0x0054ABE0, 150 GetValueFromProfile (MAE) 0x0054BBB0.
// Name set: Ego_r.pdb / FableWin.pdb ??0CSlider@NUISystem@@QAE@J@Z, ??1..UAE@XZ, AddAction / AddActionOnBack / AddActionOnSelected,
// AdjustValueToProfile, ChangeState(K), GetDataFromDef, GetMax / GetMin / SetMax / SetMin, GetValueFromProfile (MAE), Initialise,
// ProcessEvent, RevertValue, SetPAcceptButton, Update.
// Layout: Ego_r.pdb CSlider (sizeof 0x220) minus 0x30 up to Actions (the retail CSprite is 0x184) and minus 0x34 after it (STLport
// vector 0xC); C2DVector is byte-packed (Move bool +0x1AC, OldValue +0x1AD, StartValue +0x1B5). Retail offsets from the landed
// ctor 0x0054C050 / dtor 0x0054B360 / RevertValue 0x0054ABE0 and ProcessEvent 0x0054AC90 (CObserver this, -4).
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_EActionType.h"
namespace NUISystem {
class CAction;
class CSlider : public CSprite {
public:
    CSlider(long);                                                       // 0x0054C050
    CSlider(const CSlider&);
    virtual ~CSlider();                                                  // 0x0054B360
    virtual void Initialise();
    virtual void Update(float);
    virtual void ChangeState(unsigned long);
    virtual void SetPAcceptButton(CComponent*);
    virtual void AddAction(CCountedPointer<CAction>&);
    virtual void AddActionOnSelected(CCountedPointer<CAction>&);
    virtual void AddActionOnBack(CCountedPointer<CAction>&);
    virtual void ProcessEvent(EEvent);                                   // CObserver slot 1
    virtual void RevertValue();                                          // slot 149
    void AdjustValueToProfile();
    void GetDataFromDef();
    float GetMax();
    float GetMin();
    void SetMax(float);
    void SetMin(float);
protected:
    virtual void GetValueFromProfile();                                  // slot 150
public:
    C2DVector Dimensions;                                                // +0x184
    C2DVector Min;                                                       // +0x18c
    C2DVector Max;                                                       // +0x194
    C2DVector Step;                                                      // +0x19c
    C2DVector Value;                                                     // +0x1a4
    bool Move;                                                           // +0x1ac
    C2DVector OldValue;                                                  // +0x1ad
    C2DVector StartValue;                                                // +0x1b5
    std::vector<CCountedPointer<CAction> > Actions;                      // +0x1c0
    std::list<CCountedPointer<CAction> >* PActionsOnBack;                // +0x1cc
    std::list<CCountedPointer<CAction> >* PActionsOnSelect;              // +0x1d0
    EActionType PDefAction;                                              // +0x1d4
    CComponent* PLeftArrow;                                              // +0x1d8
    CComponent* PRightArrow;                                             // +0x1dc
    C2DVector PreviousValue;                                             // +0x1e0
    CComponent* PAcceptButton;                                           // +0x1e8
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CSlider) == 0x1ec);
#endif
