// CGameScriptThingManager::FrameUpdate() -- drops every script thing the manager alone still references (count block reference count 1), then frame-updates the remaining ones
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007047D0, length 130 bytes (padded to 144).
// Callees: the count block's per-type destroy function (__fastcall, object in ecx), operator delete @ 0x00BFE9BC (the
//          count block), free @ 0x00BFEA14 (the list node), CGameScriptThing::FrameUpdate @ 0x008D0190.
// Shape: an STLport list whose sentinel pointer is the manager's first word; each node carries a 16-byte value with the
// CCountedPointer<CGameScriptThing> at value+8 (node +0x10 object, +0x14 count block). The sweep is
// `it = erase(it)` / `++it` with the null-safe reference-count test inline; erase unlinks (prev first), runs the counted
// pointer's destructor (member re-read after the decrement, then both words zeroed) and frees the node.
// Model: local (no engine header for the manager; the value's leading 8 bytes are kept opaque).
void operator delete(void* p);                     // 0x00BFE9BC
extern "C" void free(void* p);                     // 0x00BFEA14
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T*           Data;                             // +0
    SCountBlock* Info;                             // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    long GetRefCount() const { return Info ? Info->m_references : 0; }
    T* operator->() const { return Data; }
};
class CGameScriptThing { public:
    void FrameUpdate();                            // 0x008D0190
};
struct SScriptThingEntry {                         // 16 bytes
    unsigned long                     m_key[2];    // +0x00 (opaque)
    CCountedPointer<CGameScriptThing> m_thing;     // +0x08
};
struct _List_node {
    _List_node*       _M_next;                     // +0x00
    _List_node*       _M_prev;                     // +0x04
    SScriptThingEntry _M_data;                     // +0x08
};
struct list {                                      // STLport list: the sentinel node pointer
    _List_node* _M_node;
    struct iterator {
        _List_node* _M_node;
        iterator(_List_node* n) : _M_node(n) {}
        SScriptThingEntry* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator pos)
    {
        _List_node* next = pos._M_node->_M_next;
        _List_node* prev = pos._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        pos._M_node->_M_data.~SScriptThingEntry();
        free(pos._M_node);
        return iterator(next);
    }
};
class CGameScriptThingManager { public:
    list m_things;                                 // +0x00
    void FrameUpdate();
};

void CGameScriptThingManager::FrameUpdate()
{
    for (list::iterator it = m_things.begin(); it != m_things.end(); ) {
        if (it->m_thing.GetRefCount() == 1)
            it = m_things.erase(it);
        else
            ++it;
    }
    for (list::iterator it2 = m_things.begin(); it2 != m_things.end(); ++it2)
        it2->m_thing->FrameUpdate();
}