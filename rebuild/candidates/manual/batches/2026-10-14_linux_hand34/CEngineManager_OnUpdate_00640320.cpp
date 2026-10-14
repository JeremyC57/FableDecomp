// CEngineManager::OnUpdate(void* context) -- for every registered component (the sentinel list at +4) that is active (virtual slot 51), call its OnUpdate (slot 9) with the context
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640320, length 64 bytes.
// Callees: virtual slots 51 (+0xcc, IsActive: a bool in al) and 9 (+0x24, OnUpdate(context)). Shape: the list head at +4 is a sentinel node (`mov eax,[ebx+4] ; mov edi,[eax] ; cmp edi,eax ; je`
// skips an empty list); the context is loaded into ebp only when the loop runs, the component is kept in esi across the two virtual calls, and the loop
// test re-reads the head pointer from the object (`cmp edi,[ebx+4]`).
struct CEngineComponent_00640320 {               // the registered component: vtable slots 9 (+0x24) OnUpdate, 10 (+0x28) OnPausedUpdate, 48 (+0xc0) OnRender, 51 (+0xcc) IsActive
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void OnUpdate(void* context);          // slot 9 (+0x24)
    virtual void OnPausedUpdate();                 // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41();
    virtual void AddMap(void* map, long flags);    // slot 42 (+0xa8)
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void OnRender(void* view);             // slot 48 (+0xc0)
    virtual void v49(); virtual void v50();
    virtual bool IsActive();                       // slot 51 (+0xcc)
};

struct SComponentNode {                            // a doubly linked list node: next, prev, the component
    SComponentNode*           m_pNext;
    SComponentNode*           m_pPrev;
    CEngineComponent_00640320* m_pComponent;       // +0x08
};

struct CEngineManager {
    unsigned char   _pad[4];
    SComponentNode* m_pComponents;                 // +0x04 (sentinel node)
    void OnUpdate(void* context);
};

void CEngineManager::OnUpdate(void* context)
{
    for (SComponentNode* node = m_pComponents->m_pNext; node != m_pComponents; node = node->m_pNext) {
        CEngineComponent_00640320* component = node->m_pComponent;
        if (component->IsActive())
            component->OnUpdate(context);
    }
}
