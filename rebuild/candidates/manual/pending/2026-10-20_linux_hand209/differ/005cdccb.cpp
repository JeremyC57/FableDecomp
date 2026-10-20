// Grade: DIFFER(187v187) -- same instructions; retail shares [ebp-4] between the inline GetInterface type word and the world-frame local (QFE never shares sibling-scope slots), shifting every other slot by one
// CTCInventoryStats::FrameUpdate() -- counts the creatures the hero killed last frame: walks the owner's entity-event list and, for every kill event (type 0x22) stamped with the previous world frame, bumps that creature's entry in CreaturesKilled (inserting it at 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CDCCB, length 187 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound const @ 0x004365B0 (the inline GetInterface for type 0x42 =
// CTCEntityEvents), CWorld::GetWorldFrame @ 0x0049D870, the event's virtual slot 1 (GetType), map<unsigned long,long>::find
// @ 0x005D83B3 (catalog "CMemoryAllocatorVariableSize_FindNode", propagated) and insert(const value_type&) @ 0x0045CA09 (the landed
// CRetailTree unique insert; pair<iterator,bool> through a hidden pointer).
// Layout: retail CTCInventoryStats ParentThing +0x04, CreaturesKilled +0x134 (PDB +0x14C; the tree's header pointer first, node key +0x10,
// value +0x14); the entity-events component's list head at +0x0C (STLport list: next at +0, element at +8); the event's frame at +0x04
// and subject at +0x08.
// Shape: the events component is a named local re-read from the frame for the end() test each pass (`cmp ebx,[eax+0Ch]`); the world
// frame is read once into the frame; the subject is kept in edi and copied into the find key and the insert pair; `this` is spilled
// to [ebp-8] for the tree address.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CEntityEvent {
    virtual void v00();
    virtual long GetType();                          // slot 1 (+0x04)
    long Frame;                                      // +0x04
    long Subject;                                    // +0x08
};
struct SEventNode {                                  // STLport _List_node<CEntityEvent*>
    SEventNode*   _M_next;
    SEventNode*   _M_prev;
    CEntityEvent* _M_data;
};
struct CEventList {                                  // std::list<CEntityEvent*>: the header node
    SEventNode* _M_node;
    SEventNode* begin() const { return _M_node->_M_next; }
    SEventNode* end() const { return _M_node; }
};
struct CTCEntityEvents {                             // interface type 0x42
    unsigned char _pad00[0x0c];
    CEventList    Events;                            // +0x0c
};
struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};
struct SKillNode {                                   // the tree node: key +0x10, value +0x14
    unsigned char _base[0x10];
    unsigned long Key;
    long          Value;
};
struct SKillPair {                                   // pair<const unsigned long, long>
    unsigned long first;
    long          second;
};
struct CKillMap {                                    // std::map<unsigned long, long> (the header pointer first)
    typedef SKillNode* iterator;
    struct SInsertResult { iterator first; bool second; };   // pair<iterator, bool>
    SKillNode* _M_header;
    iterator end() { return _M_header; }
    iterator find(const unsigned long& key);         // 0x005D83B3
    SInsertResult insert(const SKillPair& value);    // 0x0045CA09
};
enum { ENTITY_EVENT_CREATURE_KILLED = 0x22 };

namespace NInventory {
struct CTCInventoryStats {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CKillMap      CreaturesKilled;                   // +0x134
    void FrameUpdate();
};
void CTCInventoryStats::FrameUpdate()
{
    CTCEntityEvents* events;
    if (ParentThing->GetInterface(&events, 0x42)) {
        long frame = CWorld::GetWorldFrame();
        for (SEventNode* it = events->Events.begin(); it != events->Events.end(); it = it->_M_next) {
            CEntityEvent* event = it->_M_data;
            if (event->GetType() == ENTITY_EVENT_CREATURE_KILLED && event->Frame == frame - 1) {
                long creature = event->Subject;
                CKillMap* killed = &CreaturesKilled;
                SKillNode* found = killed->find(creature);
                if (found != killed->end()) {
                    ++found->Value;
                } else {
                    SKillPair value;
                    value.first = creature;
                    value.second = 1;
                    killed->insert(value);
                }
            }
        }
    }
}
}
