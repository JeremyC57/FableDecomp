// CTCElectrocutedEffect::OnInitialActivate() -- mark the parent thing rendered this frame (reason 1) and frame-updated
// Leaf: ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ
// Naming wave 31 (family srtf): own row is the PDB virtual member on engine/CTCBase.h (was the free __fastcall OnInitialActivate over
//   `struct Sub`, bare Leaf); its callees bind ?SetAsRenderedThisFrame@CThing@@QAEXK@Z and ?SetAsFrameUpdated@CThing@@QAEXXZ (were ?SetActive@Sub@@QAEHH@Z /
//   ?Refresh@Sub@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE370. Name: Ego_r.pdb / FableWin.pdb ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ
//   (CTCBase slot 6, engine/CTCBase.h retail branch).
// Callees: CThing::SetAsRenderedThisFrame(unsigned long) 0x004C7590, CThing::SetAsFrameUpdated() 0x004C7C40 (both PDBs) on ParentThing.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CThing.h"

class CTCElectrocutedEffect : public CTCBase { public:
    virtual void OnInitialActivate();
};

void CTCElectrocutedEffect::OnInitialActivate()
{
    ParentThing->SetAsRenderedThisFrame(1);
    ParentThing->SetAsFrameUpdated();
}
