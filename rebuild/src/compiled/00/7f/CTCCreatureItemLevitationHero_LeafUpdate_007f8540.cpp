// CTCCreatureItemLevitationHero::LeafUpdate() -- pop the oldest levitated item when over the limit or 30 frames after the last add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8540, length 79 bytes. CTCCreatureItemLevitationHero vtable 0x0124167C
// slot 31 (the Nymph twin is 0x007F84C0).
// Callees: own virtual slot 30 (`call [eax+78h]`; the Hero override 0x004DE76D returns 1: the item limit), the world-frame
// getter @ 0x0049D870, then the inlined CTCCreatureItemLevitationBase::PopItem (0x007F8490: CThing::Kill(bool)
// @ 0x004C9B80, STLport erase of the front node, free @ 0x00BFEA14, --NumItems).
// Shape: NumItems (+0x10) is read into a local (cached in edi across the virtual call); LastAddFrame (+0x14) is read into a
// local of an inline timeout test before the frame call and offset by 30 after it; the `||` shares one PopItem body.
#include <stdlib.h>

class CThing;

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

class CThing { public:
    void Kill(bool checkLocked);            // 0x004C9B80
};

long GetWorldFrame();                       // 0x0049D870 (returns the global frame counter @ 0x013B89BC)

class CTCCreatureItemLevitationBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual long GetMaxNumItems();          // slot 30
    virtual void LeafUpdate();              // slot 31
    CThing* ParentThing;                    // +0x04 (CTCBase)
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CThingList LevitatingItems;             // +0x0c (retail 4-byte STLport list)
    long NumItems;                          // +0x10
    long LastAddFrame;                      // +0x14
    bool HasTimedOut()
    {
        long lastAdd = LastAddFrame;
        return GetWorldFrame() > lastAdd + 30;
    }
    void PopItem()
    {
        SThingListNode* front = LevitatingItems.begin();
        front->Data->Kill(true);
        LevitatingItems.erase(front);
        --NumItems;
    }
};

struct CTCCreatureItemLevitationHero : CTCCreatureItemLevitationBase {
    virtual void LeafUpdate();
};

void CTCCreatureItemLevitationHero::LeafUpdate()
{
    long numItems = NumItems;
    if (numItems > 0) {
        if (numItems > GetMaxNumItems() || HasTimedOut())
            PopItem();
    }
}