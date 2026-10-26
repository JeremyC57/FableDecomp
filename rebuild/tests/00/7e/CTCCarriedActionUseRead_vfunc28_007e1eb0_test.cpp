// Behaviour test for 007e1eb0: body copied from src/007e1eb0.cpp (CTCCarriedActionUseRead::vfunc_28), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_deleted = 0;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
class CDefPointeeBase { public:
    virtual void v00() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
};
class CBonusItemDef : public CDefPointeeBase { public: };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
};
static CBonusItemDef* g_bonusDef = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBonusItemDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CBonusItemDef>* out)
{
    ++g_getSubDef;
    if (!g_bonusDef) return false;
    ++g_bonusDef->m_refCount; out->Object = g_bonusDef; return true;
}
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
static int g_applied = 0; static CBonusItemDef* g_appliedDef = 0; static int g_showEnded = 0;
class CTCHeroStats { public:
    void ApplyBonusItemEffects(CBonusItemDef* def) { ++g_applied; g_appliedDef = def; }
};
struct CTCShowableItem {
    void OnShowEnded() { ++g_showEnded; }
};
struct CTCCarriedActionUseRead {
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad08[0x18 - 0x08];
    bool AlreadyRead;
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUseRead::vfunc_28(CThing* user)
{
    if (!AlreadyRead) {
        CTCHeroStats* stats;
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CBonusItemDef> out;
        bool found = thingDef->GetSubDef(&out);
        CDefHolder<CBonusItemDef> def(out.Object);
        if (found && user->GetInterface(&stats, 4))
            stats->ApplyBonusItemEffects(def.Object);
        AlreadyRead = true;
    }
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}
int main()
{
    static CThing book, user; static CDefClassBase classDef; static CBonusItemDef bonus; static CTCHeroStats stats; static CTCShowableItem showable;
    memset(&book, 0, sizeof(book)); memset(&user, 0, sizeof(user)); book.PDef = &classDef; bonus.m_refCount = 1;
    CInterfaceMap::SEntry userEntries[2] = { { 4, (CTCBase*)&stats }, { 9, 0 } };
    user.m_interfaces.first = userEntries; user.m_interfaces.last = userEntries + 2; user.m_presentBits[0] |= 1UL << 4;
    CInterfaceMap::SEntry bookEntries[1] = { { 0x1a, (CTCBase*)&showable } };
    book.m_interfaces.first = bookEntries; book.m_interfaces.last = bookEntries + 1;
    CTCCarriedActionUseRead read; memset(&read, 0, sizeof(read)); read.ParentThing = &book;
    // first read, bonus def found, user has hero stats: effects applied; book has no showable component
    g_bonusDef = &bonus; read.vfunc_28(&user);
    CHECK(g_getSubDef == 1 && g_applied == 1 && g_appliedDef == &bonus); CHECK(read.AlreadyRead); CHECK(bonus.m_refCount == 1 && g_deleted == 0); CHECK(g_showEnded == 0);
    // already read: nothing applied; the showable component is notified once present
    book.m_presentBits[0] |= 1UL << 0x1a; read.vfunc_28(&user);
    CHECK(g_getSubDef == 1 && g_applied == 1 && g_showEnded == 1);
    // first read with a user lacking hero stats: nothing applied, still marked read
    read.AlreadyRead = false; user.m_presentBits[0] = 0; read.vfunc_28(&user);
    CHECK(g_getSubDef == 2 && g_applied == 1 && read.AlreadyRead && g_showEnded == 2);
    // no bonus def: nothing applied, nothing released
    read.AlreadyRead = false; user.m_presentBits[0] |= 1UL << 4; g_bonusDef = 0; read.vfunc_28(&user);
    CHECK(g_getSubDef == 3 && g_applied == 1 && g_deleted == 0);
    // a def whose only reference is the lookup is deleted by the holder
    read.AlreadyRead = false; g_bonusDef = &bonus; bonus.m_refCount = 0; read.vfunc_28(&user);
    CHECK(g_applied == 2 && g_deleted == 1);
    if (fails == 0) printf("007e1eb0_TEST PASS\n"); else printf("007e1eb0_TEST FAIL\n");
    return fails;
}