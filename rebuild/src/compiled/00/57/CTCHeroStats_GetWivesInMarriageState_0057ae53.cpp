// CTCHeroStats::GetWivesInMarriageState(std::list<CThingCreatureBase*>&, EMarriageState) const -- collects the living wives whose marriage is in the given state (0 = any)
// Leaf: GetWivesInMarriageState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AE53, 97 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: std::list<CThingCreatureBase*>::clear() @ 0x0057F5E1 (manifest "CGuiControlTreePane::SortTreeRecursively" is
//          propagated), CTCBase::GetThingManager() @ 0x00686AF0 (the world global; manifest "GFGetPreMainMemoryUsed"),
//          CWorld::GetThingFromUID(unsigned __int64) @ 0x0051ED60 (manifest "CStopWatch::GetTicks"),
//          std::list<CThingCreatureBase*>::push_back(const value_type&) @ 0x005805C9.
// Shape: the marriage list is walked from the head sentinel at +0x1D8 (head in edi, node in esi); a zero state
// matches every marriage (`test eax,eax ; je`); the wife is looked up by UID, kept when it is non-null and its
// byte at +0x10 is 1 (a creature), and pushed as `static_cast<CThingCreatureBase*>(wife)`: the cast's temporary is
// what lands in [ebp-4] AFTER the tests (pushing the CThing* local itself stores it right after the lookup).
// Layout: retail Marriages list head +0x1D8 (PDB list +0x1F0, 8 bytes smaller in retail); list node = {next, prev,
// CHeroMarriageInfo {WifeUID +0x08, MaritalHomeUID +0x10, State +0x18, GayMarriage +0x1C}}.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing { public:
    unsigned char _pad_0x00[0x10];
    unsigned char ThingType;                         // +0x10 (1 = creature)
};

class CThingCreatureBase : public CThing { public:
};

struct CThingList {                                  // std::list<CThingCreatureBase*> (retail: head pointer only)
    void* _M_node;
    void clear_0057f5e1();                                    // 0x0057F5E1
    void push_back(CThingCreatureBase* const& wife); // 0x005805C9
};

class CWorld { public:
    CThing* GetThingFromUID(unsigned __int64 uid);   // 0x0051ED60
};

struct CHeroMarriageInfoNode {
    CHeroMarriageInfoNode* Next;                     // +0x00
    CHeroMarriageInfoNode* Prev;                     // +0x04
    unsigned __int64       WifeUID;                  // +0x08
    unsigned __int64       MaritalHomeUID;           // +0x10
    long                   State;                    // +0x18 EMarriageState
    bool                   GayMarriage;              // +0x1C
};

class CTCHeroStats { public:
    void*                  __vftable;                // +0x00
    void*                  ParentThing;              // +0x04 CTCBase
    unsigned char          _pad_0x08[0x1D0];
    CHeroMarriageInfoNode* Marriages_Head;           // +0x1D8 list sentinel (PDB +0x1F0)

    CWorld* GetThingManager() const;                 // 0x00686AF0 (CTCBase)
    void GetWivesInMarriageState(CThingList& wives, long state) const;
};

void CTCHeroStats::GetWivesInMarriageState(CThingList& wives, long state) const
{
    wives.clear_0057f5e1();
    CHeroMarriageInfoNode* head = Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        if (state == 0 || node->State == state) {
            CThing* wife = GetThingManager()->GetThingFromUID(node->WifeUID);
            if (wife && wife->ThingType == 1)
                wives.push_back(static_cast<CThingCreatureBase*>(wife));
        }
    }
}