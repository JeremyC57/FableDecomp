// CThingManager::RemoveFromUpdateList(CThing* thing) -- unlinks the thing from its update list when it is in it, notifying its physics component first
// Leaf: RemoveFromUpdateList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051ECC0, length 151 bytes (+ int3 padding).
// Callees: CTCPhysicsBase virtual slot 45 (+0xb4, the removed-from-update-list notification).
// Layout (retail, local model): CThing render links prev +0x7c / next +0x80, update links prev +0x84 / next +0x88, update-list
// index byte +0x10, PhysicsTC +0x60; CThingManager RenderList (ThingLList {head, tail}) at retail +0x84 (PDB +0x9c) and the
// UpdateLists vector<ThingLList> begin at retail +0x8c (PDB +0xa4; retail is 0x18 narrower in this region).
// Shape: as RemoveFromRenderList over the update links, with the list a named pointer and the physics notification first.
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void OnRemovedFromUpdateList();     // slot 45 (+0xb4)
    virtual void OnAddedToUpdateList();         // slot 46 (+0xb8)
};
class CThing { public:
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
class CThingManager { public:
    unsigned char _pad00[0x84];
    ThingLList    RenderList;                   // +0x84 (PDB +0x9c)
    ThingLList*   UpdateLists;                  // +0x8c (PDB +0xa4: vector<ThingLList> begin)
    void RemoveFromRenderList(CThing* thing);
    void AddToUpdateList(CThing* thing);
    void RemoveFromUpdateList(CThing* thing);
};

void CThingManager::RemoveFromUpdateList(CThing* thing)
{
    ThingLList* list = &UpdateLists[thing->m_updateListIndex];
    if (thing->m_pNextUpdate || thing->m_pPrevUpdate || list->m_pHead == thing) {
        if (thing->PhysicsTC)
            thing->PhysicsTC->OnRemovedFromUpdateList();
        if (list->m_pHead == thing)
            list->m_pHead = thing->m_pNextUpdate;
        if (list->m_pTail == thing)
            list->m_pTail = thing->m_pPrevUpdate;
        if (thing->m_pNextUpdate)
            thing->m_pNextUpdate->m_pPrevUpdate = thing->m_pPrevUpdate;
        if (thing->m_pPrevUpdate)
            thing->m_pPrevUpdate->m_pNextUpdate = thing->m_pNextUpdate;
        thing->m_pNextUpdate = 0;
        thing->m_pPrevUpdate = 0;
    }
}