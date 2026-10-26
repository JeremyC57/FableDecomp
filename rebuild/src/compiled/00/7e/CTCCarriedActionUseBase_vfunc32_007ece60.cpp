// CTCCarriedActionUseBase::vfunc_32(CThingCreatureBase* user) -- default "use" of a carried item: make the user sheathe it back into the inventory (kind 100)
//
// Retail 0x007ECE60, 123 bytes (`ret 4`). Callees: CCreatureAction_SheatheItemToInventory::
// CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) @ 0x0069FFD0,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0 (bool result dropped), the
// count block's destroy function (`call [eax+4]`), operator delete @ 0x00BFE9BC,
// CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo").
// Shape: the 0xb0-byte action is a temporary built in the frame and passed straight from the
// constructor's returned `this` (`push eax`); its implicit destructor releases the follow-up-action
// counted pointer (+0xa8 object, +0xac block) inline and calls ~CCreatureActionBase out of line
// (same model as the landed CCreatureAction_ShowToSheatheTrophy::FinishAction 0x006DD980).
class CCreatureActionBase;

class CThingCreatureBase { public:
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
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

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                 // 0x00693EF0
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:   // retail sizeof 0xb0
    CCountedPointer_CCreatureActionBase PNextAction;   // +0xa8 follow-up action
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
};

class CTCCarriedActionUseBase { public:
    virtual void v00();
    void vfunc_32(CThingCreatureBase* user);
};

void CTCCarriedActionUseBase::vfunc_32(CThingCreatureBase* user)
{
    user->SetCurrentAction(&CCreatureAction_SheatheItemToInventory(user, 0, 100));
}