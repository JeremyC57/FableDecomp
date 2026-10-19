#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

struct CEventBase { };
namespace NEntityEvents {
struct CEventShownItem : CEventBase {           // 0x18 bytes
    unsigned char _pad[0x18];
    CEventShownItem(CThing* shower, CThing* item);   // 0x0084AEE0
};
}
using NEntityEvents::CEventShownItem;
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);           // 0x008113C0
};

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
};

class CCreatureAction_ShowItemToThing : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PItem;              // +0xb0
    virtual void PerformAction();
};

void CCreatureAction_ShowItemToThing::PerformAction()
{
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1)
        && PItem.GetPItem() && !(PItem.GetPItem()->m_deleteFlags & 1)) {
        CTCEntityEvents* events;
        if (PTarget.GetPItem()->GetInterface(&events, 0x42)) {
            events->AddEvent(new CEventShownItem(PCreature, PItem.GetPItem()));
        }
    }
}

static CThing* g_shower = 0; static CThing* g_item = 0; static CEventBase* g_added = 0; static int g_adds = 0;
NEntityEvents::CEventShownItem::CEventShownItem(CThing* shower, CThing* item) { g_shower = shower; g_item = item; }
void CTCEntityEvents::AddEvent(CEventBase* e) { g_added = e; ++g_adds; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e;
    return last;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCreatureActionBase::PerformAction() { }
int main()
{
    static CThing creature, onlooker, item;
    static CTCEntityEvents events;
    static CInterfaceMap::SEntry entry;
    entry.type = 0x42; entry.component = (CTCBase*)&events;
    onlooker.m_interfaces.first = &entry; onlooker.m_interfaces.last = &entry + 1;
    onlooker.m_presentBits[2] = 4;
    CCreatureAction_ShowItemToThing a;
    a.PCreature.m_pItem = &creature; a.PTarget.m_pItem = &onlooker; a.PItem.m_pItem = &item;
    bool ok = true;
    a.PerformAction();
    if (g_adds != 1 || g_shower != &creature || g_item != &item || g_added == 0) ok = false;
    item.m_deleteFlags = 1;        // item being deleted: nothing
    a.PerformAction();
    if (g_adds != 1) ok = false;
    item.m_deleteFlags = 0; onlooker.m_deleteFlags = 1;
    a.PerformAction();
    if (g_adds != 1) ok = false;
    onlooker.m_deleteFlags = 0; onlooker.m_presentBits[2] = 0;   // no events component
    a.PerformAction();
    if (g_adds != 1) ok = false;
    onlooker.m_presentBits[2] = 4; a.PItem.m_pItem = 0;
    a.PerformAction();
    if (g_adds != 1) ok = false;
    if (!ok) { std::printf("00845270_TEST FAIL\n"); return 1; }
    std::printf("00845270_TEST PASS\n");
    return 0;
}
