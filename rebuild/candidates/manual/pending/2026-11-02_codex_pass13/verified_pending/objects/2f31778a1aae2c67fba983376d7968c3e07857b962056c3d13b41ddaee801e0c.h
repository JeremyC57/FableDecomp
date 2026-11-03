#ifdef FABLE_RETAIL_REGION_CONNECTIVITY_TYPES
#ifndef FABLE_ENGINE_CTCDRIVERBASE_H
#define FABLE_ENGINE_CTCDRIVERBASE_H
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CTCDriverBase : public CTCBase {
public: virtual bool IsDriver() const;
public: virtual bool IsDriverType(long) const=0;
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public: virtual void CreateAdditionalTCs();
public: virtual ~CTCDriverBase();
};
FABLE_STATIC_ASSERT(sizeof(CTCDriverBase)==12);
#endif
#else
#ifndef FABLE_ENGINE_CTCDRIVERBASE_H
#define FABLE_ENGINE_CTCDRIVERBASE_H
#include "engine/CTCBase.h"
// Original Ego_r0x19b45; no added storage. Native CTCDExplosion base offsets agree.
class CTCDriverBase : public CTCBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
public: CTCDriverBase(CThing&);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCDriverBase)==0xc);
#endif
#endif
