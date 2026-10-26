// CTCCarrying::GetThingInPrimarySlot() -- walks the things in primary slots: a thing flagged 0x4000000 (+0x20) wins unless its +0x91 bit 0 is set (then the first plain thing so far is returned); otherwise the first live thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1440, length 71 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50. 152 callers.
// Layout: Ego_r.pdb CTCCarrying ThingsInPrimarySlots (list<CIntelligentPointer<CThing>>) at +0x28; the retail
// CVectorMap is 4 bytes narrower (12-byte vector), so the STLport list's node pointer sits at +0x24.
// Nodes: next +0, prev +4, the intelligent pointer at +8.
// Shape: result in edi (zeroed first), the list head re-read for every end test (`cmp esi,[ebx+24h]`),
// two exits: the flagged thing returns from inside the loop (`return t` / `return result` on the +0x91
// bit), the normal exit returns the remembered first thing.
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_flags;                          // +0x20
    unsigned char _pad24[0x91 - 0x24];
    unsigned char m_flags91;                        // +0x91
};

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SThingListNode {                             // STLport list node
    SThingListNode*         _M_next;
    SThingListNode*         _M_prev;
    CBaseIntelligentPointer _M_data;                // +0x08
};

struct CThingList {                                 // STLport list<CIntelligentPointer<CThing>>: the sentinel node pointer
    SThingListNode* _M_node;
};

class CTCCarrying { public:
    virtual void v00();
    unsigned char _pad_0x04[0x24 - 0x04];
    CThingList    ThingsInPrimarySlots;             // +0x24 (PDB +0x28)
    CThing* GetThingInPrimarySlot();
};

CThing* CTCCarrying::GetThingInPrimarySlot()
{
    CThing* result = 0;
    for (SThingListNode* node = ThingsInPrimarySlots._M_node->_M_next; node != ThingsInPrimarySlots._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data.GetPItem();
        if (thing) {
            if (thing->m_flags & 0x4000000) {
                if (thing->m_flags91 & 1)
                    return result;
                return thing;
            }
            if (!result)
                result = thing;
        }
    }
    return result;
}