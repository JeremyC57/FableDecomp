// Behaviour test for CTCCombat::SetMeleeFlourishValid (0x00733E10): valid + usable def -> the index counts up and the
// sound plays on reaching 0 (when a sound component exists); otherwise the index is -1; the block timer is always -1;
// the def reference from GetHeroCombatDef is released.
#include <cstdio>
#include <cstring>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CCharString {
    const char* PStringData;
    CCharString(const char* text, long length) : PStringData(text) { }
    ~CCharString() { }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyed = 0;
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CHeroCombatDef : CDefVirtual, CRefCounted {
    bool MeleeFlourishUsable;
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    explicit CDefPointer(T* object) : Object(object) { if (object) ++object->m_count; }   // test-only: built straight in the return slot (no NRVO in VC7.1)
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
struct CTCSound {
    int Plays; const char* Name; bool Pos; long A, B, C;
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c)
    { ++Plays; Name = name.PStringData; Pos = positional; A = a; B = b; C = c; return 0; }
};
static CHeroCombatDef* g_def = 0;
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
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
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const
    {
        return CDefPointer<CHeroCombatDef>(g_def);
    }
};
struct CTCCombat {
    virtual void v00() {}
    CThing* ParentThing;
    CThing* ParentCreature;
    long CurrentAbilityIndex;
    long BlockCounterTimer;
    void SetMeleeFlourishValid(bool valid);
};

void CTCCombat::SetMeleeFlourishValid(bool valid)
{
    bool usable = false;
    CDefPointer<CHeroCombatDef> def = ParentCreature->GetHeroCombatDef();
    if (def.Object && def->MeleeFlourishUsable)
        usable = true;
    if (valid && usable) {
        if (++CurrentAbilityIndex == 0 && ParentThing->HasTC(0x10)) {
            CTCSound* sound = (CTCSound*)ParentThing->GetTC(0x10);
            {
                CCharString name("FLOURISH_AVAILABLE", -1);
                sound->PlaySound(name, true, 1, 0, -1);
            }
        }
    } else {
        CurrentAbilityIndex = -1;
    }
    BlockCounterTimer = -1;
}

int main()
{
    int bad = 0;
    CTCSound sound; sound.Plays = 0;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0x10, (CTCBase*)&sound } };
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1; thing.m_presentBits[0] = 1UL << 16;
    CHeroCombatDef def; def.m_count = 1; def.MeleeFlourishUsable = true; g_def = &def;
    CTCCombat c; c.ParentThing = &thing; c.ParentCreature = &thing; c.CurrentAbilityIndex = -1; c.BlockCounterTimer = 7;
    c.SetMeleeFlourishValid(true);                   // -1 -> 0: announce
    if (c.CurrentAbilityIndex != 0 || sound.Plays != 1 || std::strcmp(sound.Name, "FLOURISH_AVAILABLE") || !sound.Pos
        || sound.A != 1 || sound.B != 0 || sound.C != -1 || c.BlockCounterTimer != -1 || def.m_count != 1) bad |= 1;
    c.SetMeleeFlourishValid(true);                   // 0 -> 1: silent
    if (c.CurrentAbilityIndex != 1 || sound.Plays != 1) bad |= 2;
    c.SetMeleeFlourishValid(false);                  // reset
    if (c.CurrentAbilityIndex != -1 || sound.Plays != 1) bad |= 4;
    def.MeleeFlourishUsable = false; c.BlockCounterTimer = 3;
    c.SetMeleeFlourishValid(true);                   // def forbids: reset
    if (c.CurrentAbilityIndex != -1 || c.BlockCounterTimer != -1) bad |= 8;
    def.MeleeFlourishUsable = true; thing.m_presentBits[0] = 0;
    c.SetMeleeFlourishValid(true);                   // no sound component: counts only
    if (c.CurrentAbilityIndex != 0 || sound.Plays != 1) bad |= 16;
    g_def = 0; c.CurrentAbilityIndex = 5;
    c.SetMeleeFlourishValid(true);                   // no def
    if (c.CurrentAbilityIndex != -1) bad |= 32;
    def.m_count = 0; g_def = &def;
    c.SetMeleeFlourishValid(false);                  // last reference released
    if (g_destroyed != 1 || def.m_count != 0) bad |= 64;
    if (bad) { std::printf("00733e10_TEST FAIL\n"); return 1; }
    std::printf("00733e10_TEST PASS\n");
    return 0;
}