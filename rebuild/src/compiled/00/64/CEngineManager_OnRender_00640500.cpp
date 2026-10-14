// CEngineManager::OnRender() -- for every active registered component (slot 51), call its OnRender (slot 48) once per pending 16-byte view entry of the vector at +8; then remove every view entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640500, length 88 bytes.
// Callees: virtual slots 51 (+0xcc, IsActive) and 48 (+0xc0, OnRender(entry)), Vector_RemoveAt @ 0x00515BC0 (on the vector at +8: erase(first, last) of the whole range).
// Shape: the component list is the sentinel list at +4 (edi = node); the inner loop runs the entry pointer in esi from +8 to +0xc (re-read each iteration,
// `add esi,10h`) and reloads the component from the node for each call; `cmp esi,[ebx+0Ch] ; je` skips an empty vector; the erase arguments are
// (first, last) pushed last-first after `lea ecx,[ebx+8]`.
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

struct SViewEntry { unsigned char _body[0x10]; };

struct CRetailViewVector {                        // VC6 Dinkumware vector<SViewEntry>
    SViewEntry* m_pFirst;
    SViewEntry* m_pLast;
    SViewEntry* m_pEnd;
    void erase(SViewEntry* first, SViewEntry* last);   // 0x00515BC0 (Vector_RemoveAt)
};

struct CEngineManager {
    unsigned char     _pad[4];
    SComponentNode*   m_pComponents;               // +0x04 (sentinel node)
    CRetailViewVector m_views;                     // +0x08
    void OnRender();
};

void CEngineManager::OnRender()
{
    for (SComponentNode* node = m_pComponents->m_pNext; node != m_pComponents; node = node->m_pNext) {
        if (node->m_pComponent->IsActive()) {
            for (SViewEntry* view = m_views.m_pFirst; view != m_views.m_pLast; ++view)
                node->m_pComponent->OnRender(view);
        }
    }
    m_views.erase(m_views.m_pFirst, m_views.m_pLast);
}