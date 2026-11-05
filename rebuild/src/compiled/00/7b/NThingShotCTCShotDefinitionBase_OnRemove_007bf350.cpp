// NThingShot::CTCShotDefinitionBase::OnRemove() -- when the shot component is removed: stop the owning thing's augmentation trail effect and kill the shot's effect thing if it is still alive
// Leaf: ?OnRemove@CTCShotDefinitionBase@NThingShot@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BF350, 113 bytes (+15 int3 padding).
// Identity: CTCBase vtable slot 2 (+0x08) is OnRemove (CThing::RemoveTC @ 0x004C9840 calls it); this body sits in slot 2
//          of EVERY shot vtable read from the exe -- CTCShotDefinitionBase 0x0126E1A4, Throw 0x0123CDC4, ThrowPredictive
//          0x0123CE9C, TearInGround 0x0123CF9C, DragonFireball 0x0123D0CC, Projectile 0x01240CCC, MultiArrow 0x0126DF34 --
//          so it is the base class's override (FableWin.pdb has ?OnRemove@CTCShotDefinitionBase@NThingShot@@UAEXXZ and no
//          OnRemove for CTCShotDefinitionThrow); the manifest's "CTCShotDefinitionThrow::vfunc_2" is the first vtable hit.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0xBF = CTCObjectAugmentations: the sign bit
//          of the bitset dword +0x34), CTCObjectAugmentations::StopTrailEffect @ 0x007664E0 (manifest "CTCKraken::
//          EndHeadEffects" is a byte-twin propagation; the body kills ProjectileTrailEffect at +0x65, PDB
//          ?StopTrailEffect@CTCObjectAugmentations@@QAEXXZ), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls),
//          CThing::Kill(bool) @ 0x004C9B80.
// Layout: CTCBase {vptr, PThing +4}; CTCShotDefinitionBase PEffect (CIntelligentPointer<CThing>) +0x0C (Ego_r.pdb);
//          CThing UsingComponents +0x20, Components +0x44, the dead bit +0x91 bit 0.
// Shape: the landed IS_THING_ALIVE(pointer) inline (GetPItem() null test, then the dead bit through a second GetPItem()),
//          then a third GetPItem() for the Kill(true) call.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCObjectAugmentations { public:                    // interface 0xBF
    void StopTrailEffect();                        // 0x007664E0
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01();
    virtual void OnRemove();                       // slot 2 (+0x08)
    CThing* PThing;                                // +0x04
    bool    PendingRemoval;                        // +0x08
};

namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase {
public:
    virtual void OnRemove();
    CBaseIntelligentPointer PEffect;               // +0x0C
};

void CTCShotDefinitionBase::OnRemove()
{
    CTCObjectAugmentations* augmentations;
    if (PThing->GetTC(TCI_OBJECT_AUGMENTATIONS, &augmentations))
        augmentations->StopTrailEffect();
    if (IS_THING_ALIVE(PEffect))
        ((CThing*)PEffect.GetPItem())->Kill(true);
}
}