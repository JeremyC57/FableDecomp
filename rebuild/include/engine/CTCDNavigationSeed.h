#ifndef FABLE_ENGINE_CTCDNAVIGATIONSEED_H
#define FABLE_ENGINE_CTCDNAVIGATIONSEED_H
// CTCDNavigationSeed (chunk 864, row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): Ego_r.pdb struct_layouts_egor.tsv
// `CTCDNavigationSeed 12 0` -- no members of its own over the 0xC CTCDriverBase (labels: ctor 0x004D41F6, Alloc 0x004D858D,
// StatGetClassTypeName 0x004D41DF, GetClassTypeName 0x004D4212, the slot-28 driver-type virtual 0x004D4222 `cmp [esp+4],15h ; sete al`,
// scalar deleting dtor 0x004D85AA). Interface type TCI_NAVIGATION_SEED = 88 (engine/ETCInterfaceType.h); driver type 21 (0x15),
// the value 0x004D4222 compares with and the rough-area TCD scan 0x005185F0 pushes (`push 15h`).
#include "engine/CTCDriverBase.h"
#include "engine/ETCInterfaceType.h"
class CTCDNavigationSeed : public CTCDriverBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_NAVIGATION_SEED; }
#ifdef FABLE_RETAIL_CTCDNAVIGATIONSEED_VSLOTS
// agentFAM05 (2026-11-11): retail virtual override this declaration lacks, declaration only (needs the polymorphic FABLE_RETAIL_CTCBASE; no layout
// change). Slot 23 of the TLC vtable 0x0123B944 (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
    virtual bool IsComponentSerialisable() const;                        // slot 23 0x004D422F
#endif // FABLE_RETAIL_CTCDNAVIGATIONSEED_VSLOTS
#ifdef FABLE_RETAIL_CTCD_STATGETDRIVERTYPE
    static unsigned long __fastcall StatGetDriverType() { return 21; }
#endif
#ifdef FABLE_RETAIL_CTCDNAVIGATIONSEED_DRIVERSLOTS
// agentFAM13 (2026-11-12): CTCDriverBase virtuals this driver overrides but this declaration lacks (TLC vtable slot 28 IsDriverType,
// slot 29 CreateAdditionalTCs, as engine/CTCDriverBase.h); spellings from the Ego_r / FableWin name sets. Declaration only.
public:
    virtual bool IsDriverType(long type) const;
    virtual void CreateAdditionalTCs();
#endif // FABLE_RETAIL_CTCDNAVIGATIONSEED_DRIVERSLOTS
};
FABLE_STATIC_ASSERT(sizeof(CTCDNavigationSeed) == 0xc);
#endif
