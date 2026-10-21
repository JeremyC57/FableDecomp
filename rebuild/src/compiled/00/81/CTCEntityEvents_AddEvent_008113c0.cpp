// CTCEntityEvents::AddEvent(NEntityEvents::CEventBase*) -- stamp the event with the world frame, put a counted pointer to it at the front of the component's event list, then tell the event it was added
// Leaf: AddEvent
//
// Retail 0x008113C0, 143 bytes (`ret 4`). Callees: CTCBase::GetWorld() @ 0x00686A70 (manifest
// "CViewBase::GetDisplayEngine", propagated), CWorld::GetWorldFrame() @ 0x0049D870 (static), operator
// new @ 0x00BFEA1A (the 12-byte count block), malloc @ 0x00BFEA0E (the 16-byte list node), operator
// delete @ 0x00BFE9BC; the block's destroy function is DeleteData @ 0x008119A0 (`__fastcall`,
// `if (p) p->DeletingDestructor(1)`); the event's virtual slot 2 (+0x08) with the component.
// Layout = engine/CTCEntityEvents.h (Ego_r.pdb, 0x18): Events (list<CCountedPointer<CEventBase>>) at
// +0x0c, retail the 4-byte STLport list (its sentinel pointer); CEventBase's frame stamp at +0x04.
// The header's members are opaque bytes, so the list is modelled locally over the same offset.
// Shape: the landed CGuiVarTransferStruct::Add (0x00CB7E50) list insert of a CCountedPointer temporary,
// here push_front (begin() = sentinel->next read before the node malloc, placement copy with its null test and add-ref, linked before the
// head, temporary released through the block's destroy function); here the block is built without an
// object null test (only new's own), and `GetWorld()->GetWorldFrame()` still emits the GetWorld call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void __cdecl operator delete(void* p);               // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace NEntityEvents { struct CEventBase; }
void __fastcall DeleteData(NEntityEvents::CEventBase* event);   // 0x008119A0

struct CWorld {
    static long GetWorldFrame();                            // 0x0049D870
};

struct CTCEntityEvents;

namespace NEntityEvents {
struct CEventBase {
    virtual void v00();
    virtual void v01();
    virtual void OnAddedTo(CTCEntityEvents& events);        // slot 2 (+0x08)
    long Frame;                                             // +0x04
};
}

template <class T> struct CCountedPointer {
    struct SBlock {
        long m_count;
        void (__fastcall *m_destroy)(T*);
        T*   m_object;
        SBlock(T* object) : m_count(1), m_destroy(DeleteData), m_object(object) {}
    };
    T*      m_object;
    SBlock* m_block;
    CCountedPointer(T* object) : m_object(object) { m_block = new SBlock(object); }
    CCountedPointer(const CCountedPointer& other) : m_object(other.m_object), m_block(other.m_block)
    {
        if (m_block)
            ++m_block->m_count;
    }
    ~CCountedPointer()
    {
        if (m_block && --m_block->m_count == 0) {
            m_block->m_destroy(m_block->m_object);
            delete m_block;
        }
    }
};

template <class T> struct CRetailList {                    // STLport list: the sentinel pointer only
    struct SNodeBase { SNodeBase* m_next; SNodeBase* m_prev; };
    struct SNode : SNodeBase { T m_value; };
    struct iterator {
        SNodeBase* m_node;
        iterator(SNodeBase* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
    };
    SNode* m_head;
    iterator begin() { return iterator(m_head->m_next); }
    SNode* create_node(const T& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->m_value) T(value);
        return node;
    }
    iterator insert(iterator position, const T& value)
    {
        SNode* node = create_node(value);
        SNodeBase* n = position.m_node;
        SNodeBase* prev = n->m_prev;
        node->m_next = n;
        node->m_prev = prev;
        prev->m_next = node;
        n->m_prev = node;
        return iterator(node);
    }
    void push_front(const T& value) { insert(begin(), value); }
};

struct CTCEntityEvents {
    virtual void v00();
    struct CThing* ParentThing;                             // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CRetailList<CCountedPointer<NEntityEvents::CEventBase> > Events;   // +0x0c
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    void AddEvent(NEntityEvents::CEventBase* event);
};

void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase* event)
{
    event->Frame = GetWorld()->GetWorldFrame();
    Events.push_front(CCountedPointer<NEntityEvents::CEventBase>(event));
    event->OnAddedTo(*this);
}