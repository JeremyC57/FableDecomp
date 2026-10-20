// Grade: DIFFER(261v282) -- block layout: retail places the 'if (!action) return 0' join straight after the pool block and gives the error arm its own return-0 tails; ours puts the join after the error arm and zero-initialises action (xor esi,esi)
// CCombatActionManager_00c3d610::CreateEnvironmentCombatAction(long arg) -- EH-framed action factory: with a positive budget at +0x6c an action is taken from the primary pool at +0x1c (dropped when it reports itself secondary); with a budget of -1 a 0x1bc-byte environment action is allocated through the module allocator; an empty pool or any other budget records error -9 once (reported through the global hook) and yields 0; the action is initialised with `arg` (vslot 3) or recycled, the pool touched for a positive budget and the action registered and returned
// Leaf: CreateEnvironmentCombatAction
// Flags: /EHsc
//
// Retail 0x00C3DA60, 288 bytes incl. padding (`ret 4`, fs:[0] frame, state 0 around the constructor). Identity: the catalog
// label "CTCHeroStats::CreateEnvironmentCombatAction" is the module's propagated class; the owner is the combat-action
// manager of the 0x00C3Cxxx module (landed RecycleAction 0x00C3D610, siblings 0x00C3DCD0 / 0x00C3DDE0 landed).
// Callees: CCombatActionPool::Take @ 0x00C3C9A0 (label "DrawGetEnvironment" propagated), the action's vslot 1
// (IsSecondaryAction) and vslot 3 (Init(long), returns bool), the module allocator operator new @ 0x00C3C360 (label
// "InformOfEvent" propagated), the environment action constructor @ 0x00C44580 (out of line, label "CIcicleData::Initialize"
// propagated), RecycleAction @ 0x00C3D610, CCombatActionPool::Touch @ 0x00C3CA30 (label "TransferFromGui" propagated),
// CCombatActionPool::PushActive @ 0x00C3C9B0. Globals: last error 0x0143BB48, the `__stdcall` report hook 0x0143BBD4.
// Shape: an empty pool jumps (`goto fail`) into the error arm of the budget if/else-if/else; the secondary test is `IsSecondaryAction() ? 0 : taken` (`neg al ; sbb ; not ; and`).
typedef void (__stdcall* PFNREPORT_00c3da60)(long error);
extern long g_lastError_0143bb48;                           // 0x0143BB48
extern PFNREPORT_00c3da60 g_pfnReport_0143bbd4;             // 0x0143BBD4

struct CPooledCombatAction {
    virtual void v00();
    virtual bool IsSecondaryAction();                       // slot 1 (+0x04)
    virtual void v02();
    virtual bool Init(long arg);                            // slot 3 (+0x0c)
};
struct CEnvironmentCombatAction_00c3da60 : CPooledCombatAction {    // 0x1bc bytes
    unsigned char _body[0x1bc - 4];
    CEnvironmentCombatAction_00c3da60();                    // 0x00C44580
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // the allocator's free (EH cleanup only)
};
struct CCombatActionPool_00c3c9b0 {                         // 20 bytes
    unsigned char _body[0x14];
    CPooledCombatAction* Take();                            // 0x00C3C9A0
    void Touch();                                           // 0x00C3CA30
    void PushActive(CPooledCombatAction* action);           // 0x00C3C9B0
};
struct CCombatActionManager_00c3d610 {
    unsigned char _pad[0x1c];
    CCombatActionPool_00c3c9b0 m_primaryPool;               // +0x1c
    unsigned char _pad30[0x6c - 0x30];
    long m_primaryBudget;                                   // +0x6c
    void RecycleAction(CPooledCombatAction* action);        // 0x00C3D610
    CPooledCombatAction* CreateEnvironmentCombatAction(long arg);
};

CPooledCombatAction* CCombatActionManager_00c3d610::CreateEnvironmentCombatAction(long arg)
{
    CPooledCombatAction* action = 0;
    if (m_primaryBudget > 0) {
        action = m_primaryPool.Take();
        if (!action)
            goto fail;
        action = action->IsSecondaryAction() ? 0 : action;
    } else if (m_primaryBudget == -1) {
        action = new CEnvironmentCombatAction_00c3da60;
    } else {
fail:
        if (g_lastError_0143bb48 == 0) {
            g_lastError_0143bb48 = -9;
            if (g_pfnReport_0143bbd4)
                g_pfnReport_0143bbd4(-9);
        }
    }
    if (!action)
        return 0;
    if (!action->Init(arg)) {
        RecycleAction(action);
        action = 0;
    }
    if (m_primaryBudget > 0)
        m_primaryPool.Touch();
    m_primaryPool.PushActive(action);
    return action;
}
