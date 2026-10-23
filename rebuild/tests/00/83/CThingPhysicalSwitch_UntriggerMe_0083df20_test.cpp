// Behaviour test for 0083df20: src/0083df20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThingPhysicalSwitch::UntriggerMe()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingPhysicalSwitch::UntriggerMe() -- clear the triggered flag and, when the def names an untrigger sound and the switch has a sound player, play it attached
// Leaf: UntriggerMe
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DF20, length 247 bytes (+ int3 padding).
// Callees: CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<CThingPhysicalSwitchDef> @ 0x0083E0D0
// (out CDefPointer; the def's slot 1 destroys on the last release), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (interface 0x10 = CTCSoundPlayer, bit 16 of CThing+0x20), the switch-def peek @ 0x0083D900 (catalog
// "CThingDeadCreature::PeekDefinition" is a propagated byte twin), CTCSoundPlayer::PlayAttachedSample(const CSoundPair&,
// const CSoundInit&, bool, long, long) @ 0x0066B280 (as the landed CTCShop::SetAlarm 0x00773C90).
// Layout: CThing DefGlobalIndex +0x8c (unsigned short), interface bitset +0x20, map +0x44 (retail); CThingPhysicalSwitch
// Triggered +0xd0 (retail, PDB +0xe8); the switch def's sound index +0x90.
// Shape: the def is peeked through an inline getter that returns the raw pointer of a CDefPointer local (released
// before the test, so the field is read through the stale pointer, as retail does); same-TU `auto_inline(off)` stand-ins
// for GetDef and LowerBound (landed GetSwitchDef 0x0083C1E0 / 0x00848730 idioms) let the def, the lookup key and the
// sound pair share one frame word. The sound pair is a named local (stored right after the peek, as retail), the
// CSoundInit a temporary whose ctor assigns PlaybackType in its body (stored after OverrideID), and PlayAttachedSample,
// which receives both addresses by const reference, has a same-TU noinline stand-in too (without it 22 lines: the pair
// stores sink below the pushes) -- the "every address-taking callee gets a stand-in" lever.
class CTCBase;
struct CDefPointeeBase {                                       // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                                    // slot 1
    long m_refCount;                                           // +0x04
};
struct CThingPhysicalSwitchDef : CDefPointeeBase {
    unsigned char _pad08[0x90 - 0x08];
    long          UntriggerSound;                              // +0x90
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
struct CDefinitionManager {
    bool GetDef_0083e0d0(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out);   // 0x0083E0D0
};
void* FindDef_CDefinitionManager_GetDef_0083e0d0();
#pragma auto_inline(off)
// Same-TU stand-in for GetDef<CThingPhysicalSwitchDef> 0x0083E0D0: writes the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (retail's LTCG knowledge; the landed 0x0083C1E0 idiom).
__declspec(noinline) bool CDefinitionManager::GetDef_0083e0d0(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0083e0d0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CSoundPair {                                            // Ego_r.pdb: 8 bytes
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
struct CSoundInit {                                            // Ego_r.pdb: 0x20 bytes
    float         Volume;                                      // +0x00
    float         Pitch;                                       // +0x04
    float         Pan;                                         // +0x08
    long          NoPlays;                                     // +0x0c
    long          PlaybackType;                                // +0x10 (ESoundPlaybackType)
    unsigned long GainFaderBits;                               // +0x14
    unsigned long PitchFaderBits;                              // +0x18
    long          OverrideID;                                  // +0x1c
    // PlaybackType is assigned in the body: retail stores it after OverrideID (as the landed sibling TriggerMe 0x0083DDC0).
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
    }
};
struct CTCSoundPlayer {                                        // the interface-0x10 component
    void PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attached, long a, long b);   // 0x0066B280
};

// Same-TU stand-in for PlayAttachedSample 0x0066B280 (body is not the retail one).
void PlaySample_0066b280(CTCSoundPlayer* p, long first, long second, const CSoundInit* init, bool attached, long a, long b);
#pragma auto_inline(off)
__declspec(noinline) void CTCSoundPlayer::PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attached, long a, long b)
{
    PlaySample_0066b280(this, sound.FirstSound, sound.LastSound, &init, attached, a, b);
}
#pragma auto_inline(on)

class CThingPhysicalSwitch {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                           // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefGlobalIndex;                             // +0x8c
    unsigned char  _pad8e[0xd0 - 0x8e];
    bool           Triggered;                                  // +0xd0
    CDefinitionManager* GetDefinitionManager();                // 0x004C7990 (CThing)
    const CThingPhysicalSwitchDef* PeekSwitchDef();            // 0x0083D900
    const CThingPhysicalSwitchDef* GetSwitchDefPtr()
    {
        CDefPointer<CThingPhysicalSwitchDef> def;
        GetDefinitionManager()->GetDef_0083e0d0(DefGlobalIndex, &def);
        return def.Object;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    void UntriggerMe();
};

void CThingPhysicalSwitch::UntriggerMe()
{
    if (Triggered) {
        Triggered = false;
        if (GetSwitchDefPtr()->UntriggerSound && HasTC(0x10)) {
            CTCSoundPlayer* player = (CTCSoundPlayer*)GetTC(0x10);
            long sound = PeekSwitchDef()->UntriggerSound;
            CSoundPair pair(sound, sound);
            player->PlayAttachedSample(pair, CSoundInit(), true, 0, 0);
        }
    }
}

// ---- stubs and driver ----
static int g_destroys = 0, g_lookups = 0, g_peeks = 0, g_plays = 0;
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++g_destroys; }
static CDefinitionManager g_mgr;
CDefinitionManager* CThingPhysicalSwitch::GetDefinitionManager() { return &g_mgr; }
static CThingPhysicalSwitchDef* g_def = 0;
void* FindDef_CDefinitionManager_GetDef_0083e0d0() { ++g_lookups; if (g_def) ++g_def->m_refCount; return g_def; }
const CThingPhysicalSwitchDef* CThingPhysicalSwitch::PeekSwitchDef() { ++g_peeks; return g_def; }
static CTCSoundPlayer* g_player = 0; static long g_first = 0, g_last = 0, g_a = -1, g_b = -1; static bool g_att = false;
static CSoundInit g_init;
void PlaySample_0066b280(CTCSoundPlayer* p, long first, long second, const CSoundInit* init, bool attached, long a, long b)
{ ++g_plays; g_player = p; g_first = first; g_last = second; g_init = *init; g_att = attached; g_a = a; g_b = b; }
int main()
{
    static CThingPhysicalSwitch sw; static CThingPhysicalSwitchDef def; static CTCSoundPlayer player;
    static CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x05; entries[0].component = 0;
    entries[1].type = 0x10; entries[1].component = (CTCBase*)&player;
    entries[2].type = 0x42; entries[2].component = 0;
    sw.m_interfaces.first = &entries[0]; sw.m_interfaces.last = &entries[3];
    g_def = &def; def.m_refCount = 1; def.UntriggerSound = 0;
    // 1) not triggered: nothing
    sw.Triggered = false; sw.UntriggerMe();
    CHECK(g_lookups == 0 && g_plays == 0 && sw.Triggered == false);
    // 2) triggered, no untrigger sound: flag cleared, def looked up and released, nothing played
    sw.Triggered = true; sw.UntriggerMe();
    CHECK(sw.Triggered == false && g_lookups == 1 && def.m_refCount == 1 && g_destroys == 0 && g_plays == 0);
    // 3) a sound but no sound player (bit 16 clear): nothing played
    def.UntriggerSound = 77; sw.Triggered = true; sw.UntriggerMe();
    CHECK(sw.Triggered == false && g_plays == 0 && g_peeks == 0);
    // 4) sound player present: pair (77,77) + default init played attached
    sw.m_presentBits[0] = 1UL << 16; sw.Triggered = true; sw.UntriggerMe();
    CHECK(g_plays == 1 && g_player == &player && g_first == 77 && g_last == 77 && g_att == true && g_a == 0 && g_b == 0);
    CHECK(g_init.Volume == 1.0f && g_init.Pitch == 1.0f && g_init.Pan == 0.0f && g_init.NoPlays == 1);
    CHECK(g_init.PlaybackType == 1 && g_init.GainFaderBits == 0 && g_init.PitchFaderBits == 0 && g_init.OverrideID == -1);
    CHECK(g_peeks == 1 && def.m_refCount == 1 && g_destroys == 0);
    // 5) the GetDef reference was the last one: the def is destroyed (the field is still read through the stale pointer)
    def.m_refCount = 0; sw.Triggered = true; sw.UntriggerMe();
    CHECK(g_destroys == 1 && g_plays == 2);
    // 6) lower bound miss (type 0x10 bit set but entry absent): component from `last`
    {
        static CInterfaceMap::SEntry e2[2]; static CTCSoundPlayer tail;
        e2[0].type = 0x05; e2[0].component = 0; e2[1].type = 0x7fffffff; e2[1].component = (CTCBase*)&tail;
        long key = 0x10;
        sw.m_interfaces.first = &e2[0]; sw.m_interfaces.last = &e2[1];
        CHECK(sw.m_interfaces.LowerBound(&key) == &e2[1]);
        def.m_refCount = 1; sw.Triggered = true; sw.UntriggerMe();
        CHECK(g_player == &tail && g_plays == 3);
    }
    if (fails == 0) printf("0083df20_TEST PASS\n");
    return fails;
}
