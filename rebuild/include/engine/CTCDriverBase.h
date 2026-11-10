#ifndef FABLE_ENGINE_CTCDRIVERBASE_H
#define FABLE_ENGINE_CTCDRIVERBASE_H
// Layout: complete original members from both PDB type records; retail STLport maps use12bytes.
// Evidence: 00783a30/NAMING_EVIDENCE.md; opt-in preserves the previous default.
#ifdef FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#include "engine/CTCBase.h"
class CEnginePrimitivePrepareInfo;
class CTCDriverBase:public CTCBase {
public:
 CTCDriverBase(const CTCDriverBase&);
 CTCDriverBase(CThing&);
 virtual bool IsDriver()const;
 virtual bool IsDriverType(long)const;
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
 virtual void CreateAdditionalTCs();
 virtual ~CTCDriverBase();
 CTCDriverBase& operator=(const CTCDriverBase&);
};
FABLE_STATIC_ASSERT(sizeof(CTCDriverBase)==12);
#else
#include "engine/CTCBase.h"
// Original Ego_r0x19b45; no added storage. Native CTCDExplosion base offsets agree.
class CTCDriverBase : public CTCBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
public: CTCDriverBase(CThing&);
#endif
#ifdef FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE
    // Retail opt-in (chunk 864, row 0x0050A650 via the CThingSearchTools TCD rough-area body): the one virtual every driver adds
    // after CTCBase's 28 slots (slot 28, +0x70). PDB name: `?IsDriverType@CTCD<X>@@UBE_NJ@Z` for every driver in
    // ghidra_out/egor_pdb_names.tsv (CTCDRegionExit 0x00737480, CTCDNavigationSeed 0x004F1D73; J = long), so `bool (long) const`.
    // Each driver compares the argument with its own driver type (retail CTCDRegionExit 0x00779940 `cmp ecx,19h ; sete al`,
    // CTCDNavigationSeed 0x004D4222 `cmp [esp+4],15h ; sete al`); the rough-area TCD scans call it (0x00518900 `push 19h ; call [edx+70h]`).
public:
    virtual bool IsDriverType(long type) const;                          // slot 28 (+0x70)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCDriverBase)==0xc);
#endif
#endif
