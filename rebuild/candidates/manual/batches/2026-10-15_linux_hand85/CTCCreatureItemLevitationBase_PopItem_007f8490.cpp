// CTCCreatureItemLevitationBase::PopItem() -- kill the oldest levitated item, unlink its node and drop the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8490, length 48 bytes. Inlined into CTCCreatureItemLevitationHero::
// LeafUpdate (0x007F8540).
// Callees: CThing::Kill(bool) @ 0x004C9B80 (checks Dead / Dying / KillLocked, sets Dying), the CRT free @ 0x00BFEA14.
// Shape: the front node is read once into a local (esi) and reused after the Kill call for the STLport erase (next/prev
// read, prev->next then next->prev, free); --NumItems is a load / dec / store around the `add esp,4`.
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

struct CThing {
    void Kill(bool checkLocked);            // 0x004C9B80
};

struct CTCCreatureItemLevitationBase {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CThingList LevitatingItems;             // +0x0c (retail 4-byte STLport list)
    long NumItems;                          // +0x10
    long LastAddFrame;                      // +0x14
    void PopItem();
};

void CTCCreatureItemLevitationBase::PopItem()
{
    SThingListNode* front = LevitatingItems.begin();
    front->Data->Kill(true);
    LevitatingItems.erase(front);
    --NumItems;
}
