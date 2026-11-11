// CTCInventoryStats::FrameUpdate() -- counts the creatures the hero killed last frame: walks the owner's entity-event list and, for every kill event (type 0x22) stamped with the previous world frame, bumps that creature's entry in CreaturesKilled (inserting it at 1)
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
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
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
class CTCBase;
// agentNF16: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x004365B0 is its const LowerBound (both original PDBs, Codex pass 20 / queue land1011bu).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
const CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key) const
{
    const value_type* it = _M_start;
    int len = int(_M_finish - _M_start);
    while (len > 0) {
        int half = len / 2;
        const value_type* middle = it + half;
        if (middle->first < key) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            const CInterfaceMap::value_type* found = (it == map->_M_finish || wanted < it->first) ? map->_M_finish : it;
            *out = (T*)found->second;
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
class CTCEntityEvents { public:                             // interface type 0x42
    unsigned char _pad00[0x0c];
    CEventList    Events;                            // +0x0c
};
class CWorld { public:
    static long __fastcall GetFrame();                     // 0x0049D870
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
SKillNode* InsertKillNode(CKillMap* map, unsigned long key, long value, bool* inserted);
// Same-TU stand-ins for the STLport map members 0x005D83B3 / 0x0045CA09 (template instantiations whose bodies retail's TU
// had): both only read their const-reference argument, so VC7.1 knows the key / pair temporaries go no further.
#pragma auto_inline(off)
__declspec(noinline) CKillMap::iterator CKillMap::find(const unsigned long& key)
{
    SKillNode* y = _M_header;
    SKillNode* x = *(SKillNode**)(_M_header->_base + 4);
    while (x) {
        if (!(x->Key < key)) { y = x; x = *(SKillNode**)(x->_base + 8); }
        else x = *(SKillNode**)(x->_base + 0xc);
    }
    return (y == _M_header || key < y->Key) ? _M_header : y;
}
__declspec(noinline) CKillMap::SInsertResult CKillMap::insert(const SKillPair& value)
{
    SInsertResult r;
    r.first = InsertKillNode(this, value.first, value.second, &r.second);
    return r;
}
#pragma auto_inline(on)
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
        long frame = CWorld::GetFrame();
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