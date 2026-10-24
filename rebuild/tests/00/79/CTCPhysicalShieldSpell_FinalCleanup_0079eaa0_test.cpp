// Behaviour test for 0x0079EAA0 CTCPhysicalShieldSpell::FinalCleanup: clears this spell's mesh effect from the
// caster's graphic appearance (0x5B) when present, clears Pausable, and stops the looping sound through the caster's
// sound player (0x10) when present.
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

class CTCGraphicAppearance {                             // interface 0x5B
public:
    void ClearMeshEffect(void* key);                     // 0x004BFFC0
};

class CTCSoundPlayer {                                   // interface 0x10
public:
    void StopSample(unsigned long sampleID);             // 0x0066ADD0
};

class CTCPhysicalShieldSpell {
public:
    virtual ~CTCPhysicalShieldSpell();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];
    unsigned long LoopingSound;                          // +0x40
    bool          Pausable;                              // +0x44
    bool          Paused;                                // +0x45
    bool          CutSceneCancelled;                     // +0x46

    __forceinline void StopSound(unsigned long sound)    // out of line at 0x0079DDB0
    {
        CTCSoundPlayer* player;
        if (ParentThing->GetInterface(&player, 0x10))
            player->StopSample(sound);
    }
public:
    void FinalCleanup();
};

void CTCPhysicalShieldSpell::FinalCleanup()
{
    CTCGraphicAppearance* appearance;
    if (ParentThing->GetInterface(&appearance, 0x5b))
        appearance->ClearMeshEffect(this);
    Pausable = false;
    StopSound(LoopingSound);
}


static void* g_cleared = 0;
static unsigned long g_stopped = 0;
static int g_stops = 0;
void CTCGraphicAppearance::ClearMeshEffect(void* key) { g_cleared = key; }
void CTCSoundPlayer::StopSample(unsigned long id) { g_stopped = id; ++g_stops; }
CTCPhysicalShieldSpell::~CTCPhysicalShieldSpell() { }

static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }

int main()
{
    static CThing caster;
    memset(&caster, 0, sizeof(caster));
    static CTCGraphicAppearance appearance;
    static CTCSoundPlayer player;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = (CTCBase*)&player;
    entries[1].type = 0x5b; entries[1].component = (CTCBase*)&appearance;
    caster.m_interfaces.first = entries;
    caster.m_interfaces.last = entries + 2;

    CTCPhysicalShieldSpell spell;
    spell.ParentThing = &caster;
    spell.LoopingSound = 0x1234;
    spell.Pausable = true;

    int ok = 1;
    spell.FinalCleanup();                               // no components: only Pausable changes
    if (spell.Pausable || g_cleared != 0 || g_stops != 0) ok = 0;
    SetBit(caster, 0x10);
    SetBit(caster, 0x5b);
    spell.Pausable = true;
    spell.FinalCleanup();
    if (spell.Pausable || g_cleared != (void*)&spell || g_stops != 1 || g_stopped != 0x1234) ok = 0;

    printf(ok ? "0079eaa0_TEST PASS\n" : "0079eaa0_TEST FAIL\n");
    return ok ? 0 : 1;
}