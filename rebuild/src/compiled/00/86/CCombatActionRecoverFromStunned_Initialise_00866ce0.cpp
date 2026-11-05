// CCombatAction_RecoverFromStunned::Initialise() -- resumes a player creature's suspended animation, resets its physics component (slot 44), removes mode 29 from its mode manager, then runs the knockdown-recover base Initialise
// Leaf: ?Initialise@CCombatAction_RecoverFromStunned@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00866CE0, 160-byte slot (157 bytes of code). Initialise is vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, four times through one `lea esi,[edi+8]`),
//          CThingAICreature::SuspendAI(bool, long) @ 0x00833A10, the physics component's virtual slot 44
//          (`call [eax+0B0h]`, no arguments; CTCPhysicsBase @ 0x006B1120 is empty, CTCPhysicsStandard @ 0x00723A50
//          forwards to its slot 43), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31),
//          CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
//          CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise @ 0x00865EB0 (direct base call).
// Shape: as RecoverFromDazed (0x008ED8C0): the creature fetched for SuspendAI is a named local (retail
// calls GetPItem before the pushes); the physics component pointer is read through an inline accessor
// (`mov ecx,[eax+60h]` straight into ecx; the bare member goes through eax) of the thing's member at +0x60 (PhysicsTC,
// PDB +0x70; the landed JackFloatUp::Initialise reads its position through the same member); the GetInterface
// result is not tested, so RemoveMode + the base call are duplicated on both paths.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43();
    virtual void ResetMovement();           // slot 44 (unnamed in the manifest: CTCPhysicsBase::vfunc_44)
};
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:
    virtual void Initialise();              // 0x00865EB0
};

class CCombatAction_RecoverFromStunned : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase { public:
    virtual void Initialise();
};

void CCombatAction_RecoverFromStunned::Initialise()
{
    if (PCreature.GetPItem()->GetType() == 1) {
        CThing* pCreature = PCreature.GetPItem();   // named: retail fetches it before the argument pushes
        ((CThingAICreature*)pCreature)->SuspendAI(false, 0);
    }
    PCreature.GetPItem()->GetPhysicsTC()->ResetMovement();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->RemoveMode(29);
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise();
}