#ifndef FABLE_ENGINE_CBRAINDEF_H
#define FABLE_ENGINE_CBRAINDEF_H
#if defined(FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY) && defined(FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26)
#error "FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY cannot be combined with FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26 (the layout view)"
#endif
#if defined(FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY)
// Add-only per-TU opt-in (NF451, 2026-11-25; row 0x00459AA2, the scalar deleting destructor, which calls the out-of-line ~CBrainDef
// 0x00459ABE): the class with its retail base and the destructor declared (Ego_r.pdb / FableWin.pdb ??1CBrainDef@@UAE@XZ; virtual
// through ~CParentDefClassBase), WITHOUT its members: they are STLport vectors and stlport/fable_containers.h adds
// /DEFAULTLIB:"libcp" to the object's .drectve. Use it only to name the class or call the destructor (a qualified call); never for
// its layout, and not together with FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26 (the layout block below). Declarations only.
#include "engine/CParentDefClassBase.h"
class CBrainDef : public CParentDefClassBase {
public:
    virtual ~CBrainDef();                                                // 0x00459ABE
};
#endif
#ifdef FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26
#include "engine/CParentDefClassBase.h"
#include "engine/CBrainBehaviour.h"
#include "stlport/fable_containers.h"
class CBrainUpdateZone;
class CBrainDef : public CParentDefClassBase {
public:
    std::vector<CBrainBehaviour> Behaviours;
    std::vector<CDefString> DisabledBehaviours;
    std::vector<CBrainUpdateZone> UpdateZones;
};
// Native 0088BF30 indexes 12-byte behaviours from +3C and disabled names from +48.
FABLE_STATIC_ASSERT_AT(881,offsetof(CBrainDef,Behaviours)==0x3c);
FABLE_STATIC_ASSERT_AT(882,offsetof(CBrainDef,DisabledBehaviours)==0x48);
#endif
#endif
