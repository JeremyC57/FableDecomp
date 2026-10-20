#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void Kill(bool immediately);                    // 0x004C9B80
    void RemoveTC(long interfaceType);              // 0x004C9840
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
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCSpecialAbilities {                         // interface 0xA7
public:
    unsigned char _pad00[0x0c];
    bool AbilityMutexLock;                          // +0x0C
    bool UsingBed;                                  // +0x0D
    long CurrentAbility;                            // +0x10 EHeroAbility
};

class CTCSoundPlayer {                              // interface 0x10
public:
    void StopSample(unsigned long sampleID);        // 0x0066ADD0
};

class CTCFireballSpell {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x44 - 0x08];
    unsigned long InitLoopingSound;                 // +0x44
    CBaseIntelligentPointer FireballBuildUpEffect;  // +0x48 CIntelligentPointer<CThing>
    void CancelMove();
};

void CTCFireballSpell::CancelMove()
{
    CThing* parent = ParentThing;
    CTCSpecialAbilities* abilities;
    if (parent->GetInterface(&abilities, 0xa7)) {
        if (abilities->CurrentAbility != 0x10)
            return;
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    CTCSoundPlayer* sound;
    if (parent->GetInterface(&sound, 0x10))
        sound->StopSample(InitLoopingSound);
    if (IS_THING_ALIVE(FireballBuildUpEffect))
        FireballBuildUpEffect.GetPItem()->Kill(true);
    parent->RemoveTC(0x9d);
}

// ---- test stubs for the retail callees ----
static int g_killed; static bool g_killArg; static long g_removed; static unsigned long g_stopped;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CThing::Kill(bool immediately) { ++g_killed; g_killArg = immediately; }
void CThing::RemoveTC(long t) { g_removed = t; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() {}
void CTCSoundPlayer::StopSample(unsigned long id) { g_stopped = id; }
void CTCFireballSpell::v00() {}

int main()
{
    int ok = 1;
    CTCSpecialAbilities abilities; abilities.AbilityMutexLock = true; abilities.CurrentAbility = 0x10;
    CTCSoundPlayer sound;
    CInterfaceMap::SEntry e[2];
    e[0].type = 0x10; e[0].component = (CTCBase*)&sound;
    e[1].type = 0xa7; e[1].component = (CTCBase*)&abilities;
    CThing owner, effect; std::memset(&owner, 0, sizeof(owner)); std::memset(&effect, 0, sizeof(effect));
    owner.m_interfaces.first = e; owner.m_interfaces.last = e + 2;
    owner.m_presentBits[0] = 1u << 16; owner.m_presentBits[5] = 1u << 7;
    CTCFireballSpell spell; spell.ParentThing = &owner; spell.InitLoopingSound = 99; spell.FireballBuildUpEffect.m_pItem = &effect;
    spell.CancelMove();
    if (abilities.AbilityMutexLock || abilities.CurrentAbility != 0 || g_stopped != 99) ok = 0;
    if (g_killed != 1 || !g_killArg || g_removed != 0x9d) ok = 0;
    // another ability holds the lock: nothing happens
    abilities.AbilityMutexLock = true; abilities.CurrentAbility = 5; g_stopped = 0; g_removed = 0;
    spell.CancelMove();
    if (!abilities.AbilityMutexLock || abilities.CurrentAbility != 5 || g_stopped || g_killed != 1 || g_removed) ok = 0;
    // no abilities component, dead effect: sound stopped, no kill, component removed
    owner.m_presentBits[5] = 0; effect.m_flags91 = 1;
    spell.CancelMove();
    if (g_stopped != 99 || g_killed != 1 || g_removed != 0x9d) ok = 0;
    if (!ok) { std::printf("007c8fd0_TEST FAIL\n"); return 1; }
    std::printf("007c8fd0_TEST PASS\n");
    return 0;
}