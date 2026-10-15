// CTCThingOwner::ClearDeadEntries() -- walks the owned-object list and erases every entry whose intelligent pointer is null or dead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B240, length 90 bytes (speed TU: the loop head is nop-aligned).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice: the null test, then the dead bit +0x91 & 1, the
// IS_THING_DEAD shape of the landed 0x00836BE0), the element's virtual destructor through vtable slot 0 with flag 0
// (an explicit in-place destroy), the node free @ 0x00BFEA14 (manifest "free", __cdecl, `add esp,4`).
// Shape: the retail std::list is the VC6-era Dinkumware list with ONE sentinel pointer (no size word: the landed
// CTCThingOwner constructor 0x004DDAED shows the member is 4 bytes), so erase() is unlink + destroy + free with no
// count update. The node is {next, prev, CIntelligentPointer value} (value at +8, its vptr at +8). The head is re-read
// at the bottom of every iteration (`cmp esi,[ebx+0Ch]`) because the destroy is a call.
struct CBaseObject;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags;                  // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObject* m_pItem;                   // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual ~CIntelligentPointer();
};

struct SThingListNode {
    SThingListNode*     next;               // +0x00
    SThingListNode*     prev;               // +0x04
    CIntelligentPointer value;              // +0x08
};

extern "C" void __cdecl FreeNode(void* p);   // free 0x00BFEA14

struct CThingComponent {
    virtual void v00();
    unsigned char _pad_0x04[0x8];           // +0x04 .. +0x0B
};

struct CTCThingOwner : CThingComponent {
    SThingListNode* ObjectList;             // +0x0C retail std::list (sentinel pointer only)
    bool CallOnDie;                         // +0x10
    void ClearDeadEntries();
};

static inline bool IsEntryDead(const CIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return false;
    }
    return true;
}

void CTCThingOwner::ClearDeadEntries()
{
    SThingListNode* n = ObjectList->next;
    while (n != ObjectList) {
        if (IsEntryDead(n->value)) {
            SThingListNode* prev = n->prev;
            SThingListNode* next = n->next;
            prev->next = next;
            next->prev = prev;
            n->value.~CIntelligentPointer();
            FreeNode(n);
            n = next;
        } else {
            n = n->next;
        }
    }
}
