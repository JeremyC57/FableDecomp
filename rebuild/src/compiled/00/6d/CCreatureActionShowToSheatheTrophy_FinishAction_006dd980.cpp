// CCreatureAction_ShowToSheatheTrophy::FinishAction() -- finish showing the trophy, then hand the creature a stack-built "sheathe trophy" action (sheathe item to inventory, kind 1002)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DD980, length 153 bytes (+ int3 padding).
// Callees: CCreatureAction_ShowTrophy::FinishAction @ 0x00845850 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// twice), CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) @ 0x0069FFD0,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0 (bool result dropped), the count block's destroy function (`call [eax+4]`),
// operator delete @ 0x00BFE9BC, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated).
// Shape: a 0xb0-byte CCreatureAction_SheatheTrophy local (vtable 0x0125FB14, rtti-vtables.tsv): its inline constructor evaluates the owner first,
// calls the base constructor with (owner, 0, 1002) and stores its vtable (scheduled after the next argument push). At scope end the IMPLICIT
// destructors run with no vtable stores: the follow-up action's counted pointer (+0xa8 object, +0xac block) is released inline (`dec [block]`,
// block re-read, destroy + delete at zero, then object and block zeroed in that order) and ~CCreatureActionBase is called out of line.
// &PCreature rides in esi (`add esi,8` once `this` is dead).
struct CThing;
struct CThingCreatureBase;
struct CCreatureActionBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CThing {
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
};

struct SCountBlock {                        // the shared count block of a CCountedPointer
    long m_references;                      // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;         // +0x08
};
struct CCountedPointer_CCreatureActionBase {   // {object, count block}
    CCreatureActionBase* m_pObject;         // +0x00
    SCountBlock*         m_pCount;          // +0x04
    ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    ~CCreatureActionBase();                 // 0x00693EF0
};
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {   // retail sizeof 0xb0
    CCountedPointer_CCreatureActionBase PNextAction;   // +0xa8 follow-up action
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
};
struct CCreatureAction_SheatheTrophy : CCreatureAction_SheatheItemToInventory {   // vtable 0x0125FB14
    virtual void v00();
    CCreatureAction_SheatheTrophy(CThingCreatureBase* owner) : CCreatureAction_SheatheItemToInventory(owner, 0, 1002) { }
};

struct CCreatureAction_ShowTrophy : CCreatureActionBase {
    virtual void FinishAction();            // 0x00845850
};
struct CCreatureAction_ShowToSheatheTrophy : CCreatureAction_ShowTrophy {
    virtual void FinishAction();
};

void CCreatureAction_ShowToSheatheTrophy::FinishAction()
{
    CCreatureAction_ShowTrophy::FinishAction();
    CCreatureAction_SheatheTrophy sheathe((CThingCreatureBase*)PCreature.GetPItem());
    PCreature.GetPItem()->SetCurrentAction(&sheathe);
}