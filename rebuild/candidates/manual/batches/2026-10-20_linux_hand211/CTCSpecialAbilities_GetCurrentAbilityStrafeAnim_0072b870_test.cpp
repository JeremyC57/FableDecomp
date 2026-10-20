// Behaviour test for CTCSpecialAbilities::GetCurrentAbilityStrafeAnim (0x0072B870): ability 0x10 asks the fireball component
// (interface 0x9d), 0xb the will-lightning component (0x94); a missing component or any other ability gives "".
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTCBase;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length) : m_pData(text) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { }
    ~CCharString() { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCFireballSpell {
    CCharString GetStrafeAnimName() const { return CCharString("FIREBALL_STRAFE", -1); }
};
struct CTCWillLightning {
    CCharString GetStrafeAnimName() const { return CCharString("LIGHTNING_STRAFE", -1); }
};
struct CTCSpecialAbilities {
    virtual void v00() { }
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    CCharString GetCurrentAbilityStrafeAnim() const;
};

CCharString CTCSpecialAbilities::GetCurrentAbilityStrafeAnim() const
{
    switch (CurrentAbility) {
    case 0x10: {
        CTCFireballSpell* fireball;
        if (ParentThing->GetInterface(0x9d, &fireball))
            return fireball->GetStrafeAnimName();
        break;
    }
    case 0xb: {
        CTCWillLightning* lightning;
        if (ParentThing->GetInterface(0x94, &lightning))
            return lightning->GetStrafeAnimName();
        break;
    }
    }
    return CCharString("", -1);
}

int main()
{
    int bad = 0;
    CTCFireballSpell fireball; CTCWillLightning lightning;
    CInterfaceMap::SEntry e[2] = { { 0x94, (CTCBase*)&lightning }, { 0x9d, (CTCBase*)&fireball } };
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.m_presentBits[4] = (1UL << (0x9d & 31)) | (1UL << (0x94 & 31));
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 2;
    CTCSpecialAbilities a; a.ParentThing = &thing;
    a.CurrentAbility = 0x10;
    if (std::strcmp(a.GetCurrentAbilityStrafeAnim().m_pData, "FIREBALL_STRAFE")) bad |= 1;
    a.CurrentAbility = 0xb;
    if (std::strcmp(a.GetCurrentAbilityStrafeAnim().m_pData, "LIGHTNING_STRAFE")) bad |= 2;
    a.CurrentAbility = 3;
    if (std::strcmp(a.GetCurrentAbilityStrafeAnim().m_pData, "")) bad |= 4;
    thing.m_presentBits[4] = 0; a.CurrentAbility = 0x10;
    if (std::strcmp(a.GetCurrentAbilityStrafeAnim().m_pData, "")) bad |= 8;
    if (bad) { std::printf("0072b870_TEST FAIL\n"); return 1; }
    std::printf("0072b870_TEST PASS\n");
    return 0;
}
