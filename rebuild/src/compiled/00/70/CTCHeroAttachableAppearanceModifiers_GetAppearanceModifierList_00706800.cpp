// CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList(list<long>& out, EHeroAttachableAppearanceModifierType type) const -- append the def index of every worn modifier of one appearance type to a caller list
// Leaf: GetAppearanceModifierList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706800, length 86 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?GetAppearanceModifierList@CTCHeroAttachableAppearanceModifiers@@QBEXAAV?$list@JV?$allocator@J@std@@@std@@W4EHeroAttachableAppearanceModifierType@@@Z`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E -- the STLport list node allocator.
// Identity: the manifest label "CArray_InsertNode" is a placeholder. The landed callers
//           CTCInventoryClothing::GetCurrentTotalArmorValue (0x005B4A95) and CCreatureAction_Undress::PerformAction
//           (0x00846410) call it as `GetAppearanceModifierList(list<long>&, type)` on the hero's interface 0x5E.
// Shape: `AppearanceModifiers[type]` (retail +0x30, a vector of 12-byte STLport vectors) is formed once (ebx) and its
//        end re-read every pass; each 8-byte CModifierEntry pushes its DefIndex through the inline STLport
//        `push_back` -> `insert(end(), x)`: end() is read before the node malloc, the placement construct keeps
//        its null test, and the node is linked before the list head (next, prev, prev->next, head->prev).
//        The STLport shape is load-bearing: the iterator needs its user copy constructor and insert() reads the
//        head's prev into a local before linking -- without them VC7.1 shrink-wraps the ebp/esi pushes below the
//        empty-range test (GOTCHAS chunk 263).
// Layout: local model -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers (PDB +0x38
//         is retail +0x30 because the two 16-byte containers in front of it are 12 bytes in retail).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E (import thunk)

struct SLongListNode {                                          // STLport _List_node<long> (12 bytes)
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long _M_data;
};

struct SListIterator {                                          // STLport _List_iterator<long>
    SLongListNode* _M_node;
    SListIterator(SLongListNode* x) : _M_node(x) {}
    SListIterator(const SListIterator& x) : _M_node(x._M_node) {}
};
struct SNodeProxy { SLongListNode* _M_data; };                  // _STLP_alloc_proxy<_Node*, ...>
struct CLongList {                                              // STLport list<long> (4 bytes: the head node)
    SNodeProxy _M_node;
    SLongListNode* _M_create_node(const long& x)
    {
        SLongListNode* p = (SLongListNode*)malloc(sizeof(SLongListNode));
        new (&p->_M_data) long(x);
        return p;
    }
    SListIterator end() { return SListIterator(_M_node._M_data); }
    SListIterator insert(SListIterator position, const long& x)
    {
        SLongListNode* tmp = _M_create_node(x);
        SLongListNode* n = position._M_node;
        SLongListNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const long& x) { insert(end(), x); }
};

struct CModifierEntry {                                         // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                              // +0x00 global def index
    bool Rendered;                                              // +0x04
};

template <class T> struct CRetailVector {                       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    void* ParentThing;                                          // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    unsigned long FrameLastChanged;                             // +0x10
    unsigned char Changed : 1;                                  // +0x14 bit 0
    unsigned char _pad15[0x30 - 0x15];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    void GetAppearanceModifierList(CLongList& out, long type) const;
};

void CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList(CLongList& out, long type) const
{
    const CRetailVector<CModifierEntry>& modifiers = AppearanceModifiers[type];
    for (const CModifierEntry* it = modifiers.begin(); it != modifiers.end(); ++it)
        out.push_back(it->DefIndex);
}