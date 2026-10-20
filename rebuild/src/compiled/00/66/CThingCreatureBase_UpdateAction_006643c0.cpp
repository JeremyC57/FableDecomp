// CThingCreatureBase::UpdateAction() -- tick the current creature action (unless it is still waiting to start), then, when actions are queued and the current one is done or yields, finish it and promote the queue's front
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006643C0, length 208 bytes, `ret`.
// Callees: CCreatureActionBase::Update @ 0x006929A0 (engine_api.tsv), CThingCreatureBase::SetCurrentAction(CCreatureActionBase*)
//   @ 0x006644F0, list<CCountedPointer<CCreatureActionBase>>::erase(iterator) @ 0x00669AF0 (manifest names the
//   byte-identical CHoverAnim list instantiation), operator delete @ 0x00BFE9BC; CCreatureActionBase virtual slots 4
//   (+0x10, FinishAction), 7 (+0x1c, returns a long: non-zero holds the update back while the anim is not added;
//   name unproven) and 27 (+0x6c, bool: the action yields to a queued one; name unproven).
// Layout (retail offsets; the engine header carries the Ego_r.pdb ones): PCurrentAction +0x100 (Data) / +0x104 (count
//   block), PQueuedActions +0x108 (retail list = its bare head pointer; node next +0, prev +4, value +8);
//   CCreatureActionBase AddedActionAnim +0x60, Finished +0x61.
// Shape: the current action is re-read through the counted pointer's inline operator-> for every use; the update runs
//   on a block-scoped CCountedPointer copy (add-ref, call, inline release through the block's {count, destroy pmf,
//   object}); "performing any action" is the landed inline IsPerformingAnyAction (0x006622A0); erase takes the
//   4-byte iterator by value (built in the argument area) and returns one through the frame word.
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void v05(); virtual void v06();
    virtual long VSlot07();                     // slot 7 (+0x1c)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool VSlot27();                     // slot 27 (+0x6c)
    unsigned char _pad04[0x60 - 0x04];
    bool AddedActionAnim;                       // +0x60
    bool Finished;                              // +0x61
    void Update();                              // 0x006929A0
};

void __cdecl operator delete(void* block);      // 0x00BFE9BC

struct SCountBlock {                            // CCPPointerInfo: the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};

struct CCountedPointer_CCreatureActionBase {    // {object, count block}
    CCreatureActionBase* Data;                  // +0x00
    SCountBlock*         Info;                  // +0x04
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer_CCreatureActionBase()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCreatureActionBase* operator->() const { return Data; }
};

struct CCreatureActionList {                    // list<CCountedPointer<CCreatureActionBase>> (retail: the head pointer)
    struct node {
        node* next;                             // +0x00
        node* prev;                             // +0x04
        CCountedPointer_CCreatureActionBase value;   // +0x08
    };
    struct iterator {
        node* _Ptr;
        iterator(node* p) : _Ptr(p) { }
        iterator(const iterator& other) : _Ptr(other._Ptr) { }
    };
    node* _Myhead;
    bool empty() const { return _Myhead->next == _Myhead; }
    CCountedPointer_CCreatureActionBase& front() { return _Myhead->next->value; }
    iterator begin() { return iterator(_Myhead->next); }
    iterator erase(iterator where);             // 0x00669AF0
};

struct CThingCreatureBase {
    unsigned char _pad00[0x100];
    CCountedPointer_CCreatureActionBase PCurrentAction;     // +0x100
    CCreatureActionList PQueuedActions;                     // +0x108
    bool IsPerformingAnyAction()
    {
        CCreatureActionBase* action = PCurrentAction.Data;
        if (action != 0) {
            if (!action->Finished)
                return true;
        }
        return false;
    }
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0
    void UpdateAction();
};

void CThingCreatureBase::UpdateAction()
{
    if (!PCurrentAction.Data)
        return;
    if (!PCurrentAction->AddedActionAnim && PCurrentAction->VSlot07() && !PCurrentAction->Finished)
        return;
    {
        CCountedPointer_CCreatureActionBase current(PCurrentAction);
        current->Update();
    }
    if (!PQueuedActions.empty()) {
        if (!IsPerformingAnyAction() || PCurrentAction->VSlot27()) {
            if (IsPerformingAnyAction())
                PCurrentAction->FinishAction();
            if (!PQueuedActions.empty()) {
                SetCurrentAction(PQueuedActions.front().Data);
                PQueuedActions.erase(PQueuedActions.begin());
            }
        }
    }
}