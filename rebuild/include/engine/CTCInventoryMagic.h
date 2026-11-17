#ifndef FABLE_ENGINE_CTCINVENTORYMAGIC_H
#define FABLE_ENGINE_CTCINVENTORYMAGIC_H
#ifdef FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22
// Retail opt-in (Codex pass 22): original CTCInventoryMagic, with both original bases.
// Ego_r lists InventoryMagicPrimitiveHandles at +0x14c after the donor bases.
// Native ctor 0072DB90 establishes base offsets 0/+0x130 and the vector at +0x134.
// Native primary vtable 012668EC slot31 is CloseInventory 0072DC40; both PDBs agree.
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CEnginePrimitiveHandle.h"
class CTCInventoryMagic : public NInventory::CTCInventoryBase,
                          public NHeroInformationScreens::CBase {
public:
#ifdef FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22_DTOR
    // Original PDB methods and native primary/secondary vtable overrides.
    virtual ~CTCInventoryMagic();
    virtual void OnCreate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual void OpenInventory();
    virtual void OnItemSelected(long, bool&);
    virtual void OnChangeCategory();
    virtual CWideString GetScreenName() const;
    virtual EPlayerMode GetPlayerMode() const;
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
#endif
    virtual void CloseInventory();
#ifdef FABLE_RETAIL_CTCINVENTORYMAGIC_CTOR
    // Opt-in (NF52, row 0x0072DB90): Ego_r.pdb / FableWin.pdb ??0CTCInventoryMagic@@QAE@AAVCThing@@@Z, out of line in retail at 0x0072DB90
    // (called by the factory 0x004D2F46). Declaration only, no layout change.
    CTCInventoryMagic(CThing& thing);                                    // 0x0072DB90
#endif
private:
    std::vector<CEnginePrimitiveHandle> InventoryMagicPrimitiveHandles;
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMagic) == 0x140);
#endif
#endif
