// Grade: DIFFER(1072v1072) -- the random index stays in edx (retail: if/else into eax), the bool result is set after the pops (retail: mov al,1 / xor al,al before them), the 0x21 component homes in the frame (retail: the dead actionPointName slot)
// CAIStateGroup_GoToPub::GetSeat(CThingObject* table, const CCharString* actionPointName) -- with a given table and action point take a seat at it; otherwise collect the pub's live tavern tables and try them in random order until one gives a seat, then take the seat's tankard
// Retail: Fable.exe (TLC, VC7.1) @ 0x009482A0, 1072 bytes, ret 8.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (list nodes), CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThingObject> vtable 0x0125A858, CIntelligentPointer<CThing>
//   vtable 0x01238C6C, its CThingObject* ctor 0x004A96F0), CVectorMap::LowerBound 0x0040F020, CTCTavernTable::GetSeat 0x007B8840 /
//   GetTankard 0x007B8590, GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890.
// Layout: as the landed sibling CAIStateGroup_GameMaster::GetSeat 0x0094CB40 (Ego_r.pdb CAIStateGroup_GoToPub).
#include <stdlib.h>
#include <new>
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float M[6]; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    __forceinline const SEntry* LowerBoundInline(const long* type) const
    {
        const SEntry* end = last;
        const SEntry* it = first;
        int len = int(end - it);
        while (len > 0) {
            int half = len / 2;
            const SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterfaceConst(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            const CInterfaceMap::SEntry* last = m_interfaces.last;
            const CInterfaceMap::SEntry* it = m_interfaces.first;
            int len = int(last - it);
            while (len > 0) {
                int half = len / 2;
                const CInterfaceMap::SEntry* middle = it + half;
                if (middle->type < type) {
                    it = middle + 1;
                    len = len - half - 1;
                } else
                    len = half;
            }
            *ppOut = (T*)(it == last || type < it->type ? last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing { };
struct CThingAICreature : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v01();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer(CThingObject* item);       // 0x004A96F0 (out of line)
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
template <class T> struct CListNode {
    CListNode* _M_next;
    CListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CStlList {               // STLport list<T> (node from malloc)
    CListNode<T>* _M_node;
    CStlList()
    {
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CStlList()
    {
        CListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
    bool empty() const { return _M_node->_M_next == _M_node; }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (CListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& front() { return _M_node->_M_next->_M_data; }
    void erase(CListNode<T>* node)
    {
        CListNode<T>* next = node->_M_next;
        CListNode<T>* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
    void push_back(const T& x)
    {
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        new (&node->_M_data) T(x);
        CListNode<T>* n = _M_node;
        CListNode<T>* prev = n->_M_prev;
        node->_M_next = n;
        node->_M_prev = prev;
        prev->_M_next = node;
        n->_M_prev = node;
    }
    void pop_front()
    {
        CListNode<T>* node = _M_node->_M_next;
        CListNode<T>* next = node->_M_next;
        CListNode<T>* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
};
struct CPointerListNode {
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTCTableList {                           // interface 0x21
    unsigned char _pad00[0x0c];
    CPointerListNode* _M_node;                  // +0x0c STLport list
};
struct CTCTavernTable {                         // interface 0xB8
    bool GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet& set, long& actionPoint, long& seat,
                 const CCharString* actionPointName);   // 0x007B8840
    CThingObject* GetTankard(long seat);           // 0x007B8590
};
struct CTCTavernGameCoinGolf {                  // interface 0xCF
    void SetGameMaster(CThing* master);         // 0x0094D430
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    CBaseIntelligentPointer PBarman;            // +0x2c
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    bool GetSeat(CThingObject* table, const CCharString* actionPointName);
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}

bool CAIStateGroup_GoToPub::GetSeat(CThingObject* table, const CCharString* actionPointName)
{
    if (table && actionPointName) {
        PTable = CIntelligentPointer<CThingObject>((CThing*)table);
        CTCTavernTable* tavernTable;
        if (PTable.GetPItem()->GetInterface(&tavernTable, 0xb8)
            && tavernTable->GetSeat(*GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, actionPointName)) {
            CIntelligentPointer<CThing> tankard(tavernTable->GetTankard(SeatIndex));
            PTankard = tankard;
            return true;
        }
        ActionPointIndex = -1;
        return false;
    }
    CStlList<CThing*> tables;
    CTCTableList* tableList;
    if (!IS_THING_ALIVE(PPub) || !PPub.GetPItem()->GetInterface(&tableList, 0x21))
        return false;
    for (CPointerListNode* node = tableList->_M_node->_M_next; node != tableList->_M_node; node = node->_M_next) {
        if (IS_THING_ALIVE(node->_M_data) && node->_M_data.GetPItem()->HasTC(0xb8))
            tables.push_back(node->_M_data.GetPItem());
    }
    if (tables.empty())
        return false;
    PTable = CIntelligentPointer<CThingObject>((CThing*)0);
    while (!tables.empty()) {
        unsigned long count = tables.size();
        unsigned long random = GFRandom(GetPWorldSeed());
        unsigned long index;
        if (count)
            index = random % count;
        else
            index = 0;
        CListNode<CThing*>* it;
        for (it = tables._M_node->_M_next; it != tables._M_node; it = it->_M_next) {
            if (index == 0) {
                PTable = CIntelligentPointer<CThingObject>(it->_M_data);
                break;
            }
            --index;
        }
        CTCTavernTable* tavernTable;
        if (PTable.GetPItem()->GetInterfaceConst(&tavernTable, 0xb8)
            && tavernTable->GetSeat(*GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, 0)) {
            PTankard = CIntelligentPointer<CThing>((CThing*)tavernTable->GetTankard(SeatIndex));
            return true;
        }
        tables.erase(it);
    }
    ActionPointIndex = -1;
    return false;
}
