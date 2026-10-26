// Behaviour test for 0x006A1270 CTCCarrying::IsPrimarySlotFree: true only when at least one carry slot's def is
// primary and every primary slot holds nothing; a filled primary slot returns false at once; non-primary slots are
// ignored. Each pass takes and releases one def reference.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __declspec(x)
#endif
class CThing;

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    const SCarrySlotEntry* begin() const { return m_pFirst; }
    const SCarrySlotEntry* end() const { return m_pLast; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const SCarrySlotEntry& operator[](unsigned int n) const { return *(begin() + n); }
};

class CCarrySlotDef { public:
    virtual void v00();
    virtual void DeleteThis();                      // slot 1
    long          RefCount;                         // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;                        // +0x30
};

template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (--m_p->RefCount == 0) m_p->DeleteThis(); }
    T* operator->() const { return m_p; }
};

class CDefinitionManager { public:
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_006a1270();                           // stand-in callee of the stand-in body below
__declspec(noinline) void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_006a1270();
    if (!def)
        return;
    *(void**)out = def;
}

class CGameDefinitionManager { public:
    static CDefinitionManager* Get();               // 0x0044C6B0
};

class CTCCarrying { public:
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    CCarrySlotMap CarrySlots;                       // +0x0c
    bool IsPrimarySlotFree() const;
};

bool CTCCarrying::IsPrimarySlotFree() const
{
    bool found = false;
    for (unsigned int i = 0; i < CarrySlots.size(); ++i) {
        CDefPointer<CCarrySlotDef> def;
        CGameDefinitionManager::Get()->GetDef(CarrySlots[i].first, &def);
        if (def->IsPrimary) {
            if (CarrySlots[i].second.GetPItem())
                return false;
            found = true;
        }
    }
    return found;
}


class CThing { public: int dummy; };
static CCarrySlotDef g_defs[8];
static int g_lookups = 0, g_deletes = 0;
void CCarrySlotDef::v00() { }
void CCarrySlotDef::DeleteThis() { ++g_deletes; }
void CTCCarrying::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pBlock; }
static CDefinitionManager g_mgr;
CDefinitionManager* CGameDefinitionManager::Get() { return &g_mgr; }
static long g_requested = -1;
void* FindDef_006a1270() { ++g_lookups; return &g_defs[g_requested]; }

int main()
{
    for (unsigned int k = 0; k < 8; ++k) { g_defs[k].RefCount = 1000; g_defs[k].IsPrimary = false; }
    g_defs[2].IsPrimary = true;
    g_defs[5].IsPrimary = true;
    static CThing carried;
    SCarrySlotEntry slots[3];
    memset(slots, 0, sizeof(slots));
    slots[0].first = 1;                                 // not primary, holding something: ignored
    slots[0].second.m_pBlock = &carried;
    slots[1].first = 2;                                 // primary, empty
    slots[2].first = 5;                                 // primary, empty

    CTCCarrying c;
    c.CarrySlots.m_pFirst = slots;
    c.CarrySlots.m_pLast = slots + 3;
    c.CarrySlots.m_pEnd = slots + 3;

    int ok = 1;
    // The stand-in GetDef returns g_defs[g_requested] for every pass; passes = lookups made by one call.
    struct Run {
        static bool Go(CTCCarrying& t, const SCarrySlotEntry* s, unsigned int n, int& passes)
        {
            (void)s; (void)n;
            passes = g_lookups;
            bool r = t.IsPrimarySlotFree();
            passes = g_lookups - passes;
            return r;
        }
    };
    int passes = 0;
    g_requested = 2;                                    // every pass sees def 2 (primary)
    slots[0].second.m_pBlock = 0;
    if (!Run::Go(c, slots, 3, passes) || passes != 3) ok = 0;   // all empty, primary: true
    slots[1].second.m_pBlock = &carried;
    if (Run::Go(c, slots, 3, passes) || passes != 2) ok = 0;    // second slot filled: false at once
    g_requested = 1;                                    // no primary def at all: false after a full pass
    if (Run::Go(c, slots, 3, passes) || passes != 3) ok = 0;
    c.CarrySlots.m_pLast = slots;                       // no slots: false
    if (Run::Go(c, slots, 0, passes) || passes != 0) ok = 0;
    if (g_defs[2].RefCount != 1000 - 5 || g_defs[1].RefCount != 1000 - 3 || g_deletes != 0) ok = 0;

    printf(ok ? "006a1270_TEST PASS\n" : "006a1270_TEST FAIL\n");
    return ok ? 0 : 1;
}