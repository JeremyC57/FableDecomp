#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawPerceivers::GetPerceivedThings(std::vector<const CThing*>*) -- append the item of every intelligent pointer in the perceiving list to the output vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D3A5, length 53 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on each node's value at +8), vector<const CThing*>::push_back(const CThing* const&) @ 0x0049C450
// (the landed retail push_back for 4-byte elements; the item is a frame local whose address is passed).
// Layout: PDB CDrawPerceivers: Things (list) at +0x40, retail +0x3c. Shape: a frame-pointer size TU; the node is compared with the head register
// on entry and re-read from the member after the calls; the output vector pointer is reloaded from its argument slot for the call.
class CThing;

struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    const CThing* GetPItem();                      // 0x00A01B50
};

struct CRetailThingPointerList {                   // VC6 Dinkumware list<CIntelligentPointer<const CThing>>: one sentinel pointer
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    struct iterator {
        node* m_pNode;
        explicit iterator(node* n) : m_pNode(n) { }
        CBaseIntelligentPointer& operator*() const { return m_pNode->value; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
};

struct CRetailThingVector {                        // VC6 Dinkumware vector<const CThing*>
    const CThing** m_pFirst;
    const CThing** m_pLast;
    const CThing** m_pEnd;
    void push_back(const CThing* const& value);    // 0x0049C450
};

namespace NPlayerGui {

class CDrawPerceivers { public:
    unsigned char           _pad[0x3c];            // CDrawBase, State, Allegiance, FrameInterp, ThrobScale, ThingCount
    CRetailThingPointerList Things;                // +0x3c
    void GetPerceivedThings(CRetailThingVector* things);
};

void CDrawPerceivers::GetPerceivedThings(CRetailThingVector* things)
{
    for (CRetailThingPointerList::iterator it = Things.begin(); it != Things.end(); ++it) {
        const CThing* thing = (*it).GetPItem();
        things->push_back(thing);
    }
}

}