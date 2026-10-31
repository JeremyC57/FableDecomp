// _global::CopyIf_HasBuildingAsHomeOrWork_006c1ed0(CreatureList::const_iterator, CreatureList::const_iterator, back_insert_iterator<CreatureList>, HasBuildingAsHomeOrWork) -- copy_if: append every live creature of a list whose home or work building is the given one to another list
// Leaf: CopyIf_HasBuildingAsHomeOrWork_006c1ed0
// Grade: DIFFER(239v240) 38 difflines: ebx/ebp now right via operator-> building compare; residue add eax,168h/mov ecx,eax vs lea, pushes after the empty test, node/data esi/edi swapped (retry wave 2 sec2b)
// Was: DIFFER(241v240) -- `mov ecx,eax ; add ecx,168h/170h` where retail has `lea ecx,[eax+168h/170h]` before the two building GetPItem calls; callee-saved pushes delayed past the empty-range test (retail pushes ebx/ebp/esi/edi first) and ebx/ebp, esi/edi swapped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C1ED0, length 240 bytes, `ret 10h`.
// Shape: __fastcall (/Gr) with a hidden return pointer in ecx (back_insert_iterator has a user constructor), the list
// iterators, the back inserter and the 4-byte predicate by value on the stack. Loop: the predicate is inlined
// (GetPItem() != 0, !Dead bit 0 of +0x91, home building +0x168 == Building || work building +0x170 == Building,
// GetPItem re-called through operator-> each time), then the STLport list push_back is inlined: end() node read first,
// malloc(0x10) @ 0x00BFEA0E, placement copy-construct CIntelligentPointer<CThingAICreature> (CBaseIntelligentPointer()
// @ 0x00A01B10, vtable 0x0123E7E0, SetPItem(GetPItem()) @ 0x00A01B90 / 0x00A01B50), link before the head reading
// head->prev once. Identity: the manifest label `_Copy_if<..., HasBuildingAsHomeOrWork>` (the landed 0x007BB660 is the
// thing-search filter of the same name and gives the CThingAICreature retail offsets).
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }

struct CBaseObject { };

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    CBaseIntelligentPointer();               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>: vtable 0x0123E7E0
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    T* operator->() const { return static_cast<T*>(GetPItem()); }
};

struct CThingBuilding : CBaseObject { };

struct CThingAICreature : CBaseObject {      // retail layout (see the landed 0x007BB660)
    unsigned char _pad_0x00[0x91];
    unsigned char StateBits;                 // +0x91 bit 0 = Dead
    unsigned char _pad_0x92[0x168 - 0x92];
    CIntelligentPointer<CThingBuilding> HomeBuilding;    // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;    // +0x170
    bool IsDead() const { return (StateBits & 1) != 0; }
};

typedef CIntelligentPointer<CThingAICreature> CCreaturePointer;

// STLport list<CIntelligentPointer<CThingAICreature> >: the list object is its head node pointer; node = next, prev, value.
struct SListNodeBase {
    SListNodeBase* m_pNext;                  // +0x00
    SListNodeBase* m_pPrev;                  // +0x04
};

struct SListNode : SListNodeBase {
    CCreaturePointer m_data;                 // +0x08
};

struct CreatureList {
    SListNodeBase* m_pNode;                  // +0x00 head (end) node

    struct const_iterator {
        SListNodeBase* m_pNode;
        const CCreaturePointer& operator*() const { return ((SListNode*)m_pNode)->m_data; }
        const_iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const const_iterator& other) const { return m_pNode != other.m_pNode; }
    };

    SListNode* _M_create_node(const CCreaturePointer& x)
    {
        SListNode* p = (SListNode*)malloc(sizeof(SListNode));
        new (&p->m_data) CCreaturePointer(x);
        return p;
    }
    void insert(SListNodeBase* position, const CCreaturePointer& x)
    {
        SListNode* tmp = _M_create_node(x);
        SListNodeBase* n = position;
        SListNodeBase* prev = n->m_pPrev;
        tmp->m_pNext = n;
        tmp->m_pPrev = prev;
        prev->m_pNext = tmp;
        n->m_pPrev = tmp;
    }
    void push_back(const CCreaturePointer& x) { insert(m_pNode, x); }
};

struct back_insert_iterator {                // back_insert_iterator<CreatureList>
    CreatureList* container;
    explicit back_insert_iterator(CreatureList& c) : container(&c) { }
    back_insert_iterator& operator=(const CCreaturePointer& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
    back_insert_iterator operator++(int) { return *this; }
};

struct HasBuildingAsHomeOrWork {
    CBaseObject* Building;
    bool operator()(const CCreaturePointer& creature) const
    {
        return creature.GetPItem() != 0 && !creature->IsDead()
            && (creature->HomeBuilding.operator->() == Building || creature->WorkBuilding.operator->() == Building);
    }
};

back_insert_iterator __fastcall CopyIf_HasBuildingAsHomeOrWork_006c1ed0(CreatureList::const_iterator first, CreatureList::const_iterator last, back_insert_iterator dest, HasBuildingAsHomeOrWork pred)
{
    for (; first != last; ++first)
        if (pred(*first)) {
            *dest++ = *first;
        }
    return dest;
}
