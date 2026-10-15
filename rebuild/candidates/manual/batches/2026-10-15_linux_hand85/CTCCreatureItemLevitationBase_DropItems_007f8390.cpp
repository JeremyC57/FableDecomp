// CTCCreatureItemLevitationBase::DropItems() -- hand every levitated item back to its physics, then empty the list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8390, length 89 bytes (+7 int3). CTCCreatureItemLevitationBase vtable
// 0x0124156C slot 29.
// Callees: the item's physics component (CThing +0x60 = PhysicsTC; PDB +0x70) virtual slot 38 (`call [edx+98h]`,
// CTCPhysicsBase 0x006B10D0 is an empty `ret 4`) with `true`; the CRT free @ 0x00BFEA14 (the STLport node allocator).
// Shape: LevitatingItems (+0x0c) is the 4-byte retail STLport list<CThing*> (PDB 12 bytes, so NumItems sits at +0x10
// instead of the PDB +0x18). The walk re-reads the header after every virtual call (`cmp esi,[edi+0Ch]`); the STLport
// _M_clear walk is `tmp = cur; cur = cur->next; free(tmp)` and ends by pointing the header at itself (next then prev);
// NumItems = 0 is an immediate store.
#include <stdlib.h>

struct CThing;

struct SThingListNode {                     // STLport _List_node<CThing*>
    SThingListNode* Next;                   // +0x00
    SThingListNode* Prev;                   // +0x04
    CThing* Data;                           // +0x08
};

struct CThingList {                         // STLport list<CThing*> (4 bytes: the header node pointer)
    SThingListNode* Header;                 // +0x00
    SThingListNode* begin() const { return Header->Next; }
    SThingListNode* end() const { return Header; }
    void erase(SThingListNode* node)
    {
        SThingListNode* next = node->Next;
        SThingListNode* prev = node->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(node);
    }
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37();
    virtual void SetPhysicsActive(bool active);   // slot 38
};

struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;              // +0x60
};

struct CTCCreatureItemLevitationBase {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CThingList LevitatingItems;             // +0x0c
    long NumItems;                          // +0x10
    long LastAddFrame;                      // +0x14
    void DropItems();
};

void CTCCreatureItemLevitationBase::DropItems()
{
    for (SThingListNode* it = LevitatingItems.begin(); it != LevitatingItems.end(); it = it->Next)
        it->Data->PhysicsTC->SetPhysicsActive(true);
    SThingListNode* cur = LevitatingItems.Header->Next;
    while (cur != LevitatingItems.Header) {
        SThingListNode* tmp = cur;
        cur = cur->Next;
        free(tmp);
    }
    LevitatingItems.Header->Next = LevitatingItems.Header;
    LevitatingItems.Header->Prev = LevitatingItems.Header;
    NumItems = 0;
}
