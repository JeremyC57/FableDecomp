// CAIStateGroup_GoToPub::GetBar() -- collect the live things in the pub that have a tavern bar component, adopt the first as the bar and its barman as ours; false when the pub has none
//   STLport shape (`insert(end(), x)` with a by-value iterator that declares a copy ctor, `create_node`, and the position's
//   prev read into a local before linking): without it VC7.1 shrink-wraps the ebp push around the loop; (2) front() through
//   an inline `begin()` returning the head's next by reference, which keeps retail's `cmp [esi],esi` empty test and the
//   re-read of head->next on the true path; (3) the counted-pointer operator= names the source item in its own statement
//   (`CThing* item = other.GetPItem(); SetPItem(item);`), which schedules the `this` reload after the GetPItem call.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009490D0, length 464 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (vtables CIntelligentPointer<CThingObject> 0x0125A858, CIntelligentPointer<CThingAICreature> 0x0123E7E0), CVectorMap::
// LowerBound @ 0x0040F020 (inlined GetInterface 0x21 = bit 1 of +0x24, 0xb9 = bit 25 of +0x34), the CRT malloc @ 0x00BFEA0E
// and free @ 0x00BFEA14 (the local STLport list's nodes).
// Layout: GoToPub from Ego_r.pdb (base 0x14: PPub +0x14, PBar +0x1c, PBarman +0x2c); the building contents component (0x21)
// keeps its thing list head at +0x0c (landed SitForMeal 0x00945B10); CTCTavernBar (0xb9) Barman is Ego_r.pdb +0x1c, +0x18 in
// retail (the Jugs vector at +0x0c is 12 bytes in STLport).
#include "engine/CAIStateGroupBase.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingObject;
class CThingAICreature;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};

#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

// The building's contents component (interface 0x21): a Dinkumware list of CIntelligentPointer<CThing>, head at +0x0c.
struct SContentsNode {
    SContentsNode* next;
    SContentsNode* prev;
    CBaseIntelligentPointer value;              // +0x08
};
struct CTCBuildingContents {
    unsigned char _pad[0xc];
    SContentsNode* head;                        // +0x0c
};

class CTCTavernBar { public:                           // interface 0xb9
    unsigned char _pad[0x18];
    CIntelligentPointer<CThingAICreature> Barman;   // +0x18
    CIntelligentPointer<CThingAICreature> GetPBarman() const { return Barman; }
};

// A local STLport list<CThing*> on the malloc node allocator: {head}; node {next, prev, value}.
struct SThingNode {
    SThingNode* next;
    SThingNode* prev;
    CThing* value;
};
struct CThingPtrList {
    SThingNode* head;
    CThingPtrList()
    {
        head = (SThingNode*)malloc(sizeof(SThingNode));
        head->next = head;
        head->prev = head;
    }
    ~CThingPtrList()
    {
        clear();
        free(head);
    }
    void clear()
    {
        SThingNode* cur = head->next;
        while (cur != head) {
            SThingNode* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        head->next = head;
        head->prev = head;
    }
    bool empty() const { return head->next == head; }
    CThing*& front() { return (*begin()).value; }
    SThingNode*& begin() { return head->next; }
    struct iterator {
        SThingNode* m_node;
        iterator(SThingNode* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
    };
    iterator end() { return iterator(head); }
    SThingNode* create_node(CThing* const& value)
    {
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        new (&node->value) CThing*(value);
        return node;
    }
    iterator insert(iterator position, CThing* const& value)
    {
        SThingNode* node = create_node(value);
        SThingNode* next = position.m_node;
        SThingNode* prev = next->prev;
        node->next = next;
        node->prev = prev;
        prev->next = node;
        next->prev = node;
        return iterator(node);
    }
    void push_back(CThing* const& value) { insert(end(), value); }
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14 CIntelligentPointer<CThingBuilding>
    CIntelligentPointer<CThingObject> PBar;     // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingAICreature> PBarman;   // +0x2c
    bool GetBar();
};

bool CAIStateGroup_GoToPub::GetBar()
{
    CTCBuildingContents* contents;
    ((CThing*)PPub.GetPItem())->GetInterface(&contents, 0x21);
    CThingPtrList bars;
    for (SContentsNode* node = contents->head->next; node != contents->head; node = node->next) {
        if (IS_THING_ALIVE(((CThing*)node->value.GetPItem())) && ((CThing*)node->value.GetPItem())->HasTC(0xb9))
            bars.push_back(((CThing*)node->value.GetPItem()));
    }
    if (bars.empty())
        return false;
    PBar = (CThingObject*)bars.front();
    CTCTavernBar* bar;
    ((CThing*)PBar.GetPItem())->GetInterface(&bar, 0xb9);
    PBarman = bar->GetPBarman();
    return true;
}