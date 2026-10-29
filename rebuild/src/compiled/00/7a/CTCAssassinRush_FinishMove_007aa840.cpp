// CTCAssassinRush::FinishMove(CThingCreatureBase* creature) -- finish the creature's current action, start the FinalAction when one is held, set StartTrailDelay and clean up the effects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AA840, length 49 bytes (+ int3 padding).
// Callees: CThingCreatureBase::FinishCurrentAction @ 0x00662650, CThingCreatureBase::SetCurrentAction
// (CCreatureActionBase*) @ 0x006644F0 (result dropped), CTCAssassinRush::CleanUpEffects @ 0x007AA370.
// Layout: Ego_r.pdb CTCAssassinRush StartTrailDelay +0x2c; FinalAction (CCountedPointer<CCreatureActionBase>)
// PDB +0x94 = retail +0x90 (ItemAlphaList is a 12-byte vector in retail), its object pointer first.
// Shape: the creature argument rides in edi through both calls; the flag store is scheduled after
// `mov ecx,esi` for the trailing call.
class CCreatureActionBase;

class CThingCreatureBase { public:
    void FinishCurrentAction();                             // 0x00662650
    bool SetCurrentAction(const CCreatureActionBase& action);     // 0x006644F0
};

struct SCountBlock;

struct CCountedActionPointer {                              // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* m_pObject;                         // +0
    SCountBlock*         m_pCount;                          // +4
};

class CTCAssassinRush { public:
    unsigned char         _pad00[0x2c];
    bool                  StartTrailDelay;                  // +0x2c
    unsigned char         _pad2d[0x90 - 0x2d];
    CCountedActionPointer FinalAction;                      // +0x90
    void CleanUpEffects();                                  // 0x007AA370
    void FinishMove(CThingCreatureBase* creature);
};

void CTCAssassinRush::FinishMove(CThingCreatureBase* creature)
{
    creature->FinishCurrentAction();
    if (FinalAction.m_pObject)
        creature->SetCurrentAction(*FinalAction.m_pObject);
    StartTrailDelay = true;
    CleanUpEffects();
}