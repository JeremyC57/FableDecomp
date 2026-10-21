// CAIStateGroup_TavernKeeper::GetBar() -- collect the live tavern bars (interface 0xb9) among the pub's contained things into a local list and latch the first as the bar; false when the pub is dead or has none
// Grade: DIFFER(ours v retail) -- draft: the local STLport list keeps its head in memory and the push_back/iteration register plan differs (368 vs 346 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00968650, length 352 bytes (+ int3 padding).
// Callees: malloc @ 0x00BFEA0E (STLport list nodes, 12 bytes), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
//          (PPub +0x24, PBar +0x1c; temporary CIntelligentPointer<CThingObject> vtable 0x0125A858), the inlined GetTC 0x21
//          (bit 1 of +0x24, the pub's container component whose +0x0c list holds intelligent pointers), the local list's
//          out-of-line destructor @ 0x00792240 (manifest "CGuiControlTreePane::SortTreeRecursively" is propagated).
// Shape: first draft: the STLport list ctor and push_back are inline (head node self-linked; the node value built with
//          a null-checked placement copy), the destructor out of line at both exits.
// Layout: engine/CAIStateGroup_TavernKeeper.h (PBar +0x1c, PPub +0x24).
#include "engine/CAIStateGroup_TavernKeeper.h"

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CPointerList {                           // STLport list<CIntelligentPointer<CThing>>
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CBaseIntelligentPointer m_value; };
    SNode* m_pHead;
};
struct CTCContainer {                           // interface 0x21 (descriptive)
    unsigned char _pad[0x0c];
    CPointerList Contents;                      // +0x0c (descriptive)
};
struct CThingList {                             // STLport list<CThing*>
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    __forceinline CThingList() { SNode* head = (SNode*)malloc(sizeof(SNode)); head->m_pNext = head; head->m_pPrev = head; m_pHead = head; }
    ~CThingList();                              // 0x00792240
    __forceinline void push_back(CThing* const& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        CThing** slot = &node->m_value;
        if (slot)
            *slot = value;
        node->m_pNext = m_pHead;
        node->m_pPrev = m_pHead->m_pPrev;
        m_pHead->m_pPrev->m_pNext = node;
        m_pHead->m_pPrev = node;
    }
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    __forceinline CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CAIStateGroupTavernKeeperMethods : CAIStateGroup_TavernKeeper {
    CBaseIntelligentPointer& Pub() { return *(CBaseIntelligentPointer*)PPub; }
    CBaseIntelligentPointer& Bar() { return *(CBaseIntelligentPointer*)PBar; }
    bool GetBar();
};

bool CAIStateGroupTavernKeeperMethods::GetBar()
{
    CThingList bars;
    if (IS_THING_ALIVE(Pub()) && Pub().GetPItem()->HasTC(0x21)) {
        CTCContainer* container = Pub().GetPItem()->GetTC<CTCContainer>(0x21);
        for (CPointerList::SNode* node = container->Contents.m_pHead->m_pNext; node != container->Contents.m_pHead; node = node->m_pNext) {
            CBaseIntelligentPointer& thing = node->m_value;
            if (IS_THING_ALIVE(thing) && thing.GetPItem()->HasTC(0xb9))
                bars.push_back(thing.GetPItem());
        }
        if (bars.m_pHead->m_pNext != bars.m_pHead) {
            CIntelligentPointerObject first(bars.m_pHead->m_pNext->m_value);
            Bar().SetPItem(first.GetPItem());
            return true;
        }
    }
    return false;
}
