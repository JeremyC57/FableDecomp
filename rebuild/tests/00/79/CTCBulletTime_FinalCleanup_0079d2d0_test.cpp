// Behaviour test for 0x0079D2D0 CTCBulletTime::FinalCleanup: clears Pausable, removes this power's multiplier from
// the caster's haste component (0x8C) when present, and clears the interface-0x46 component's flag when present.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CTCBase;

struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(interfaceType < found->type))
                it = found;
            else
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CMultiplierMap {                                  // retail map<void*, float>
    void* m_head;
    unsigned long erase(void* const& key);               // 0x0058119B
};

class CTCHaste {                                         // interface 0x8C
public:
    unsigned char  _pad00[0x18];
    CMultiplierMap Multipliers;                          // +0x18
    void RecalculateMultiplier();                        // 0x00800020
    void RemoveMultiplier(void* source)
    {
        Multipliers.erase(source);
        RecalculateMultiplier();
    }
};

class CTCInterface46 {                                   // interface 0x46 component
public:
    unsigned char _pad00[0x1c];
    bool          m_flag1c;                              // +0x1c
};

class CTCBulletTime {
public:
    virtual ~CTCBulletTime();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    bool          Pausable;                              // +0x54
    bool          Paused;                                // +0x55
    bool          CutSceneCancel;                        // +0x56

public:
    void FinalCleanup();
};

void CTCBulletTime::FinalCleanup()
{
    Pausable = false;
    CTCHaste* haste;
    if (ParentThing->GetInterface(&haste, 0x8c))
        haste->RemoveMultiplier(this);
    CTCInterface46* component;
    if (ParentThing->GetInterface(&component, 0x46))
        component->m_flag1c = false;
}


static void* g_erased = 0;
static int g_recalcs = 0;
unsigned long CMultiplierMap::erase(void* const& key) { g_erased = key; return 1; }
void CTCHaste::RecalculateMultiplier() { ++g_recalcs; }
CTCBulletTime::~CTCBulletTime() { }

static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }

int main()
{
    static CThing caster;
    memset(&caster, 0, sizeof(caster));
    static CTCHaste haste;
    static CTCInterface46 c46;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x46; entries[1].component = (CTCBase*)&c46;
    entries[2].type = 0x8c; entries[2].component = (CTCBase*)&haste;
    caster.m_interfaces.first = entries;
    caster.m_interfaces.last = entries + 3;

    CTCBulletTime power;
    power.ParentThing = &caster;
    power.Pausable = true;
    c46.m_flag1c = true;

    int ok = 1;
    power.FinalCleanup();                               // no interface bits: only Pausable changes
    if (power.Pausable || g_recalcs != 0 || g_erased != 0 || !c46.m_flag1c) ok = 0;
    SetBit(caster, 0x8c);
    SetBit(caster, 0x46);
    power.Pausable = true;
    power.FinalCleanup();
    if (power.Pausable || g_recalcs != 1 || g_erased != (void*)&power || c46.m_flag1c) ok = 0;

    printf(ok ? "0079d2d0_TEST PASS\n" : "0079d2d0_TEST FAIL\n");
    return ok ? 0 : 1;
}