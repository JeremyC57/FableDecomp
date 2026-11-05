// CCreatureAction_DragonStrafeInto::Initialise() -- animation "STRAFE_INTO", base Initialise, then (mode manager present) push a new CModeDragonStrafe(owner, Timer, Speed, Timer * 0.125, Timer * 0.75) onto the owner's mode manager
// Leaf: ?Initialise@CCreatureAction_DragonStrafeInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4480, length 233 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("STRAFE_INTO" @ 0x01288514), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0x58 bytes),
// CModeDragonStrafe::CModeDragonStrafe(CThing*, float, float, float, float) 0x008DDFB0, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*)
// 0x00718A00, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as CCreatureAction_DragonBurninateInto::Initialise 0x008F4320 (in-chunk): the owner argument goes through the inline
// `operator CThing*()`, so its GetPItem call runs before the members are loaded and pushed; the two Timer fractions (0.125 @
// 0x0123095C, 0.75 @ 0x01238174) are named float locals computed before the allocation; the manager stays in ebx.
// Layout: Timer / Speed at retail +0x114 / +0x118 (PDB +0x12c / +0x130).
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

struct CModeBase { };
#include "engine/CCountedPointer.h"
struct CModeDragonStrafe : CModeBase {
    unsigned char _pad[0x58];
    CModeDragonStrafe(CThing* owner, float timer, float speed, float rampUp, float rampDown);   // 0x008DDFB0
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
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

class CCreatureAction_DragonStrafeInto : public CCombatActionBase {
public:
    float Timer;                                // +0x114 (PDB +0x12c)
    float Speed;                                // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_DragonStrafeInto::Initialise()
{
    {
        CCharString name("STRAFE_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager)) {
        float rampUp = Timer * 0.125f;
        float rampDown = Timer * 0.75f;
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeDragonStrafe(PCreature, Timer, Speed, rampUp, rampDown)));
    }
}