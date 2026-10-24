// CWeaponTrailListRecord::CWeaponTrailListRecord(const CWeaponTrailListRecord& other) -- copy constructor of an 8-byte vector element: copies the STLport list<CWeaponTrailEntry> (fresh sentinel, range insert) and the trailing word
// Leaf: ??0CWeaponTrailListRecord@@QAE@ABU0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00864F80, 71 bytes, `ret 4`, returns `this`.
// Identity: NOT LinkedList::Init (propagated label). Callers are the element constructor 0x00865640 (`if (p) new(p) T(x)`)
// and the vector push_back paths 0x00865846 / 0x008658C4 (`add [esi+4],8`: 8-byte elements), next to
// CThingPhysical_RenderWeaponTrails 0x00864FF0. The list's range insert 0x0076FD40 copies 28-byte CWeaponTrailEntry
// records (landed CEnginePrimitiveWeaponTrail ctor 0x007703B0, whose CWeaponTrailList model is reused so 0x0076FD40 keeps
// its symbol). Ego_r.pdb CTCWeaponTrail::Trail is a vector<list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>>; the
// retail element adds one word after the 4-byte list. The record name is descriptive (not identified).
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte sentinel node), the STLport range insert
// `_M_insert_dispatch(pos, first, last, const __false_type&)` @ 0x0076FD40.
// Shape: the inline STLport list copy constructor (proxy zeroed, sentinel malloc'd and self-linked, insert(begin(),
// x.begin(), x.end()) with the by-value 4-byte iterator built in its argument slot and the empty tag homed over the dead
// parameter slot), then the word copy.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes (the range insert copies 7 dwords)

struct SFalseType { SFalseType() { } };                  // STLport __false_type dispatch tag
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };   // 0x24 bytes
struct SListIterator {                                   // 4-byte iterator with a user constructor
    SListNodeBase* m_node;
    SListIterator(SListNodeBase* node) : m_node(node) { }
    SListIterator(const SListIterator& other) : m_node(other.m_node) { }
};
struct SListConstIterator { SListNodeBase* m_node; };

struct CWeaponTrailList {                                // STLport list: the alloc proxy's node pointer only
    SListNode* m_node;
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        insert(begin(), x.cbegin(), x.cend());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const SFalseType& tag);   // 0x0076FD40
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last)
    {
        InsertDispatch(pos, first, last, SFalseType());
    }
};

struct CWeaponTrailListRecord {
    CWeaponTrailList Entries;                            // +0x00
    unsigned long    Extra;                              // +0x04
    CWeaponTrailListRecord(const CWeaponTrailListRecord& other);
};

CWeaponTrailListRecord::CWeaponTrailListRecord(const CWeaponTrailListRecord& other)
    : Entries(other.Entries), Extra(other.Extra)
{
}