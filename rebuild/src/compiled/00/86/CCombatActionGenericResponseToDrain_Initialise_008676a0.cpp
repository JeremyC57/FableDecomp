// CCombatAction_GenericResponseToDrain::Initialise() -- GENERIC_RESPONSE_DRAIN animation; with a live drainer the creature is turned to face away from it (flat direction, up = Z) and given a CModeDrained(creature, drainer) through its mode manager; then the base Initialise
// Leaf: ?Initialise@CCombatAction_GenericResponseToDrain@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008676A0, 379 bytes (384 with padding), virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("GENERIC_RESPONSE_DRAIN" 0x012771D0), SetCombatAnimation 0x008574D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise @ 0x00A14440 (catalog
//   label C3DVector::GetScaled), the physics virtual slot 32 (+0x80, orientation from a CRightHandedSet), CVectorMap::LowerBound
//   0x0040F020 (inlined GetInterface 0x31; the absent arm reads the never-set out pointer from its shared frame word),
//   operator new 0x00BFEA1A (0x80 bytes), NCreatureMode::CModeDrained(CThing*, CThing*) 0x00868220 (landed),
//   CCountedPointer<CModeBase>(CModeBase*) 0x00718A00, CTCCreatureModeManager::AddMode 0x0073ACC0, CCombatActionBase::Initialise
//   0x00857520 (called last, on both paths).
// Shape: the landed IceTrollIcicleAssaultInto::Initialise model (scoped animation name, the owner through the intelligent
//   pointer's inline operator CThing*() so it is fetched before the target) plus the CastAssassinRush orientation idiom
//   (physics reference fetched through an inline GetPhysics() before the set, Up member-wise, Forward copied).
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +0x08, PTarget +0xa8); CThing PhysicsTC +0x60.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up, Forward; };
class CTCPhysicsBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 32 (+0x80)
};

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};

namespace NCreatureMode {
struct CModeBase { };
struct CModeDrained : CModeBase {               // 0x80 bytes
    unsigned char _pad[0x80];
    CModeDrained(CThing* thing, CThing* drainer);   // 0x00868220
};
}
#include "engine/CCountedPointer.h"
class CTCCreatureModeManager { public:                 // interface type 0x31
    bool AddMode(CCountedPointer<NCreatureMode::CModeBase> mode);   // 0x0073ACC0
};
class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCombatAction_GenericResponseToDrain : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_GenericResponseToDrain::Initialise()
{
    {
        CCharString name("GENERIC_RESPONSE_DRAIN");
        SetCombatAnimation(name);
    }
    if (PTarget.GetPItem() && !((CThing*)PTarget.GetPItem())->IsDead()) {
        CThing* creature = ((CThing*)PCreature.GetPItem());
        const C3DVector& drainerPosition = ((CThing*)PTarget.GetPItem())->GetPos();
        const C3DVector& position = creature->GetPos();
        C3DVector away(position.X - drainerPosition.X, position.Y - drainerPosition.Y, 0.0f);
        away.AccurateNormalise();
        CTCPhysicsBase& physics = *((CThing*)PCreature.GetPItem())->GetPhysicsTC();
        CRightHandedSet set;
        set.Up.X = 0; set.Up.Y = 0; set.Up.Z = 1;
        set.Forward = away;
        physics.SetOrientation(set);
        CTCCreatureModeManager* modes;
        ((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->AddMode(CCountedPointer<NCreatureMode::CModeBase>(new NCreatureMode::CModeDrained(PCreature, ((CThing*)PTarget.GetPItem()))));
    }
    CCombatActionBase::Initialise();
}