// CThingManager::AddToUpdateList(CThing* thing) -- appends the thing to the tail of its update list (selected by its list index) unless it is already linked, notifying its physics component
// Leaf: AddToUpdateList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051EC50, length 100 bytes (+ int3 padding).
// Callees: CTCPhysicsBase virtual slot 46 (+0xb8, the added-to-update-list notification).
// Layout (retail, local model): CThing render links prev +0x7c / next +0x80, update links prev +0x84 / next +0x88, update-list
// index byte +0x10, PhysicsTC +0x60; CThingManager RenderList (ThingLList {head, tail}) at retail +0x84 (PDB +0x9c) and the
// UpdateLists vector<ThingLList> begin at retail +0x8c (PDB +0xa4; retail is 0x18 narrower in this region).
// Shape: the list is a named pointer `&UpdateLists[thing->index]` (`lea edi,[ecx+eax*8]`); early return when already
// linked; the tail link is an if/else, then prev = tail, next = 0, tail = thing.
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void OnRemovedFromUpdateList();     // slot 45 (+0xb4)
    virtual void OnAddedToUpdateList();         // slot 46 (+0xb8)
};
struct CThing {
    unsigned char   _pad00[0x10];
    unsigned char   m_updateListIndex;          // +0x10 (which update list the thing lives in)
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x7c - 0x64];
    CThing*         m_pPrevRender;              // +0x7c
    CThing*         m_pNextRender;              // +0x80
    CThing*         m_pPrevUpdate;              // +0x84
    CThing*         m_pNextUpdate;              // +0x88
};
struct ThingLList {                             // 8 bytes: head, tail
    CThing* m_pHead;
    CThing* m_pTail;
};
struct CThingManager {
    unsigned char _pad00[0x84];
    ThingLList    RenderList;                   // +0x84 (PDB +0x9c)
    ThingLList*   UpdateLists;                  // +0x8c (PDB +0xa4: vector<ThingLList> begin)
    void RemoveFromRenderList(CThing* thing);
    void AddToUpdateList(CThing* thing);
    void RemoveFromUpdateList(CThing* thing);
};

void CThingManager::AddToUpdateList(CThing* thing)
{
    ThingLList* list = &UpdateLists[thing->m_updateListIndex];
    if (thing->m_pNextUpdate || thing->m_pPrevUpdate || list->m_pHead == thing)
        return;
    if (thing->PhysicsTC)
        thing->PhysicsTC->OnAddedToUpdateList();
    if (list->m_pTail)
        list->m_pTail->m_pNextUpdate = thing;
    else
        list->m_pHead = thing;
    thing->m_pPrevUpdate = list->m_pTail;
    thing->m_pNextUpdate = 0;
    list->m_pTail = thing;
}
