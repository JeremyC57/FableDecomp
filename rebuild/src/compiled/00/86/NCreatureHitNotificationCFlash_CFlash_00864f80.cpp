// NCreatureHitNotification::CFlash::CFlash(const CFlash& other) -- copy constructor of the 8-byte hit-flash vector element: copies the STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry (fresh sentinel, range insert) and CreationFrame
// Leaf: ??0CFlash@NCreatureHitNotification@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00864F80, 71 bytes, `ret 4`, returns `this`.
// Naming: Ego_r.pdb NCreatureHitNotification::CFlash { list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;
//   long CreationFrame; } (16 bytes with the 12-byte debug list; retail 8 bytes = the 4-byte STLport list + the word) and
//   vector<NCreatureHitNotification::CFlash> (Ego_r.pdb _Tidy / _Destroy instantiations); its copy constructor is the
//   implicit memberwise one, mangled ??0CFlash@NCreatureHitNotification@@QAE@ABV01@@Z. The neighbours are the hit-notification
//   code: CTCCreatureHitNotification::DrawHitNotification 0x00864FF0 and UpdateHitNotification 0x008650E0 (FINDINGS chunk
//   403). Callers are the element constructor 0x00865640 (`if (p) new(p) T(x)`) and the vector push_back paths 0x00865846 /
//   0x008658C4 (`add [esi+4],8`: 8-byte elements). Was landed under the descriptive stand-in "CWeaponTrailListRecord"
//   (manifest "LinkedList::Init" propagated). The list stand-in keeps the landed CEnginePrimitiveWeaponTrail ctor
//   0x007703B0 model, so the range insert 0x0076FD40 keeps its symbol.
// The list's range insert 0x0076FD40 copies 28-byte CWeaponTrailEntry records (Ego_r.pdb: C3DVector Top, Bottom; float
// CreationTime).
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte sentinel node), the STLport range insert
// `_M_insert_dispatch(pos, first, last, const __false_type&)` @ 0x0076FD40.
// Shape: the inline STLport list copy constructor (proxy zeroed, sentinel malloc'd and self-linked, insert(begin(),
// x.begin(), x.end()) with the by-value 4-byte iterator built in its argument slot and the empty tag homed over the dead
// parameter slot), then the word copy.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class C3DVector { public: float X, Y, Z; };
class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                            // 28 bytes (the range insert copies 7 dwords)
    public:
        C3DVector Top;                                   // +0x00
        C3DVector Bottom;                                // +0x0c
        float     CreationTime;                          // +0x18
    };
};
typedef CEnginePrimitiveWeaponTrail::CWeaponTrailEntry CWeaponTrailEntry;

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

namespace NCreatureHitNotification {
class CFlash {
public:
    CWeaponTrailList FlashGeometry;                      // +0x00
    long             CreationFrame;                      // +0x04
    CFlash(const CFlash& other);
};

CFlash::CFlash(const CFlash& other)
    : FlashGeometry(other.FlashGeometry), CreationFrame(other.CreationFrame)
{
}
}
