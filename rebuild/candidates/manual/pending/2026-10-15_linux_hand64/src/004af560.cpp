// CQuestManager::IsQuestCardActive(const CCharString&) -- true when one of the active quest-card things has the given def name
// Leaf: IsQuestCardActive
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AF560, 77 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetDefName @ 0x004C7CC0 (returns a CDefString
// through the hidden pointer), CDefString::operator==(const CCharString&) @ 0x004B7980.
// Label: the chunk label "CParticleSystem::ClearAllDecals" is propagated. `this` is the quest manager singleton
// (0x013B89FC, the caller is CGameScriptInterface::GiveHeroQuestCardDirectly 0x008968C0) and +0x5c is
// ActiveQuestCards (list<CIntelligentPointer<CThing>>, PDB +136: the retail vectors are 12 bytes and the STLport
// lists 4). Leaf named by role.
// Shape: STLport list walk (node data at +8), the name reference cached in ebx after the entry test; the
// CDefString temporary is homed in the name's dead argument slot (`lea ecx,[esp+14h]`); `if (card && name ==)`
// returns through the shared `mov al,1` / `xor al,al` tails.
struct CCharString {
    void* PStringData;
};

struct CDefString {                                  // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_index;
    CDefString();
    bool operator==(const CCharString& name) const;  // 0x004B7980
};

struct CBaseObject;

struct CThing {
    CDefString GetDefName() const;                   // 0x004C7CC0
};

struct CBaseIntelligentPointer {                     // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {              // STLport list: one word, the sentinel node
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
};

struct CQuestManager {
    unsigned char                              m_pad00[0x5c];
    CRetailList< CIntelligentPointer<CThing> > ActiveQuestCards;      // +0x5c
    CRetailList< CIntelligentPointer<CThing> > CompletedQuestCards;   // +0x60
    bool IsQuestCardActive(const CCharString& defName);
};

bool CQuestManager::IsQuestCardActive(const CCharString& defName)
{
    for (CRetailList< CIntelligentPointer<CThing> >::iterator it = ActiveQuestCards.begin(); it != ActiveQuestCards.end(); ++it) {
        CThing* card = (*it).Get();
        if (card && card->GetDefName() == defName)
            return true;
    }
    return false;
}
