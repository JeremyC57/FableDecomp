// CHandlerList_00a3ec30::Dispatch(void* event) -- offer the event to each handler of the pointer list at +0x4c in turn (virtual slot 22, +0x58, with the event); true as soon as one accepts it, false when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EC30, length 54 bytes.
// Callees: the handler's virtual slot 22. The chunk label (CMessageEventManager::FindMostRecentMessage<IsOfType>) is propagated: this body walks a list of POINTERS at +0x4c and calls a virtual on each, so the class is named by address. Shape: `this` in edi, the node in esi, the event in ebx, the sentinel re-read for every pass, `xor al,al` / `mov al,1` tails, `ret 4`.
struct IEventHandler_00a3ec30 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual bool HandleEvent(void* event);         // slot 22 (+0x58)
};
struct SHandlerNode {
    SHandlerNode*          m_pNext;
    SHandlerNode*          m_pPrev;
    IEventHandler_00a3ec30* m_value;               // +0x08
};
struct CHandlerPtrList {                           // list<IEventHandler*>: one sentinel pointer
    SHandlerNode* m_pHead;
};

struct CHandlerList_00a3ec30 {
    unsigned char   _pad00[0x4c];
    CHandlerPtrList m_handlers;                    // +0x4c
    bool Dispatch(void* event);
};

bool CHandlerList_00a3ec30::Dispatch(void* event)
{
    for (SHandlerNode* node = m_handlers.m_pHead->m_pNext; node != m_handlers.m_pHead; node = node->m_pNext) {
        if (node->m_value->HandleEvent(event))
            return true;
    }
    return false;
}