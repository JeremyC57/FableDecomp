// CTCJackShield::OnInitialActivate() -- turns the shield off and, outside the editor, hides the parent thing (CThing::SetAsDrawable(false))
// Leaf: ?OnInitialActivate@CTCJackShield@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00731970, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCJackShield's vtable 0x01266EFC (the slot holds this body in 1 vtable(s), owner CTCJackShield); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCJackShield@@UAEXXZ; `ret` matches the signature.
// Was: CTCJackShield::vfunc_6 (rebuild/src/compiled/00/73/CTCJackShield_vfunc6_00731970.cpp).
// No shared header for CTCJackShield: minimal local definition on its RTTI base CTCBase, Ego_r.pdb member at its offset
// (?EnableShield@CTCJackShield@@QAEX_N@Z = 0x00731680, Ego_r.pdb name set).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CMainGameComponent.h"
#include "engine/CIntelligentPointer.h"
class CTCJackShield : public CTCBase {
public:
    virtual void OnInitialActivate();                       // slot 6
    void EnableShield(bool);                                // 0x00731680
private:
    CIntelligentPointer<CThing> PEmitter;                   // +0x0c
};
void CTCJackShield::OnInitialActivate()
{
    EnableShield(false);
    if (!CMainGameComponent::Get()->IsEditorActive())
        ParentThing->SetAsDrawable(false);
}
