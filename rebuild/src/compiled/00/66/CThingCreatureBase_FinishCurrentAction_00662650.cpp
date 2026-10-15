// CThingCreatureBase::FinishCurrentAction() -- tell an unfinished current action to finish, holding a counted reference to it and raising the "finishing action" flag for the duration of the call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662650, length 80 bytes.
// Callees: the action's virtual slot 4 (+0x10, the finish hook), the count block's destroy function (`mov ecx,[blk+8] ;
// call [blk+4]`), operator delete @ 0x00BFE9BC.
// Layout (retail, local model): the current action CCountedPointer {object +0x100, count block +0x104}; the action's
// "finished" byte +0x61; a flag byte +0x15d (bit 2 = finishing the current action, bit 7 = hidden on the mini map).
// engine/CThingCreatureBase.h is the PDB (Ego_r) layout (PCurrentAction +0x11c, bitfields at +0x184..): the retail class is
// narrower, so a local descriptive model is used.
// Shape: the counted pointer is COPIED into a local in an inner scope (`inc [blk]` before the call, `dec [blk] ; jne` +
// destroy + delete after it, the block held in edi); the object pointer of the copy is the one already loaded for the
// null/finished tests (CSE'd into ecx). The flag is a one-bit bitfield (`or byte ptr ..,4` / `and byte ptr ..,0FBh`).
struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

struct CCreatureActionBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void FinishAction();                // slot 4 (+0x10)
    unsigned char _pad04[0x5d];
    unsigned char m_finished;                   // +0x61
};

struct CActionPointer {                         // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* m_pObject;             // +0
    SCountBlock*         m_pCount;              // +4
    CActionPointer(const CActionPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CActionPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    CCreatureActionBase* operator->() const { return m_pObject; }
};

struct CThingCreatureBase {
    unsigned char  _pad00[0x100];
    CActionPointer m_currentAction;             // +0x100
    unsigned char  _pad108[0x55];
    unsigned char  m_flags15d_0 : 2;            // +0x15d
    unsigned char  m_finishingAction : 1;       // +0x15d bit 2
    unsigned char  m_flags15d_3 : 4;
    unsigned char  m_hiddenOnMiniMap : 1;       // +0x15d bit 7
    void FinishCurrentAction();
};

void CThingCreatureBase::FinishCurrentAction()
{
    if (m_currentAction.m_pObject != 0 && !m_currentAction.m_pObject->m_finished) {
        m_finishingAction = true;
        {
            CActionPointer action(m_currentAction);
            action->FinishAction();
        }
        m_finishingAction = false;
    }
}