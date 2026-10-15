// CTCRegionFollower::IsRegionFollower(CThing* thing) -- true when thing is one of the followers held in the Followers list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AEB60, length 56 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (on each node's intelligent pointer, `lea ecx,[esi+8]`).
// Shape: a null argument returns false first; the list walk starts at head->next compared against the head already in eax,
// then re-reads the head member at the bottom of each iteration (`cmp esi,[edi+0Ch] ; jne`); the found exit (`mov al,1`) is
// placed after the shared `xor al,al` fall-through. GetPItem's result is on the left of `==` (`cmp eax,ebx`).
// Layout = Ego_r.pdb CTCRegionFollower (0x18) with the retail one-pointer list: Followers
// (list<CIntelligentPointer<CThing>>, node = next, prev, 8-byte pointer) at +0x0c. No engine header; modelled locally.
struct CBaseObject { };
struct CThing : CBaseObject { };

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* PData;                                            // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
#pragma pack(pop)

template<class T>
struct CRetailList {                                        // retail std::list: one sentinel pointer
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* m_node;
        iterator(node* n) : m_node(n) { }
        T& operator*() const { return m_node->value; }
        iterator& operator++() { m_node = m_node->next; return *this; }
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    node* head;
    iterator begin() const { return iterator(head->next); }
    iterator end() const { return iterator(head); }
};

struct CTCRegionFollower {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CRetailList<CBaseIntelligentPointer> Followers;         // +0x0c CIntelligentPointer<CThing>
    bool IsRegionFollower(CThing* thing);
};

bool CTCRegionFollower::IsRegionFollower(CThing* thing)
{
    if (thing) {
        for (CRetailList<CBaseIntelligentPointer>::iterator it = Followers.begin(); it != Followers.end(); ++it) {
            if ((*it).GetPItem() == thing)
                return true;
        }
    }
    return false;
}