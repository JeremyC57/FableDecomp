#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONBASE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONBASE_H
// NThingShot::CTCShotDefinitionBase (new, chunk 770): RTTI .?AVCTCShotDefinitionBase@NThingShot@@ (class key V, pdb_class_keys.tsv),
// vtable 0x0126E1A4; base CTCBase (0xC bytes, engine/CTCBase.h).
// Layout (Ego_r.pdb names; retail offsets: the STLport vector is 12 bytes, so from PTargetThing on every member sits 4 below the PDB;
// the landed GetInitialVelocity 0x007BEFB0 reads PShotDef at +0x38, the row 0x007BEFF0 PWeapon at +0x30):
//   PEffect +0x0C, HitThingsList +0x14, PTargetThing +0x20, PShooter +0x28, PWeapon +0x30, PShotDef +0x38. Only the members up to
//   PWeapon are modelled (no sizeof assertion); virtual overrides are not declared.
// Name set (Ego_r.pdb, donor order CanBeHitByMeleeStrike / GetInitialVelocity / GetPWeapon = retail 0x007BEFA0 / 0x007BEFB0 /
//   0x007BEFF0): ?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ.
// Includers define FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTHING and FABLE_RETAIL_CINTELLIGENTPOINTER first.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase {
public:
    CThing* GetPWeapon() const;                                         // 0x007BEFF0
protected:
    CIntelligentPointer<CThing> PEffect;                                // +0x0c
    std::vector<CIntelligentPointer<const CThing> > HitThingsList;      // +0x14
    CIntelligentPointer<const CThing> PTargetThing;                     // +0x20
    CIntelligentPointer<CThing> PShooter;                               // +0x28
    CIntelligentPointer<CThing> PWeapon;                                // +0x30
};
}
#endif
