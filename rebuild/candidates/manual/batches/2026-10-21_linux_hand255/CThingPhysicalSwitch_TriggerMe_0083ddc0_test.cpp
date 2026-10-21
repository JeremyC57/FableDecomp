#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingPhysicalSwitch::TriggerMe(CThing* triggerer) -- trigger the switch once: mark it triggered, queue a CEventTriggered for the triggerer on the entity-events component (0x42), then play the def's TriggerSound through the sound player (0x10) when it has one
// Leaf: ?TriggerMe@CThingPhysicalSwitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DDC0, length 344 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A,
//   NEntityEvents::CEventTriggered::CEventTriggered(CThing*) @ 0x0083CF20 (vtable 0x01273014),
//   CTCEntityEvents::AddEvent(CEventBase*) @ 0x008113C0, CThing::GetDefinitionManager @ 0x004C7990,
//   CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(long, CDefPointer&) @ 0x0083E0D0,
//   CThingPhysicalSwitch::PeekDefinition() const @ 0x0083D900 (landed under the propagated label
//   "CThingDeadCreature::PeekDefinition"), CTCSoundPlayer::PlaySound(const CSoundPair&, const CSoundInit&, ...) @ 0x0066B280,
//   the def's virtual slot 1 on the last release.
// Layout (retail, local model): CThing interface bitset +0x20, interface map +0x44, def global index (unsigned short)
//   +0x8c; CThingPhysicalSwitch::Triggered +0xd0 (Ego_r.pdb +0xe8, retail CThingPhysical is 0x18 lower);
//   CThingPhysicalSwitchDef::TriggerSound +0x90 (PDB +0x94, retail def base 4 smaller). CSoundPair {FirstSound, LastSound}.
// Shape: the first PeekDefinition is inlined (the def is released and then read, exactly as retail does), the second
//   is the out-of-line call; the def out pointer and the second lookup key live in the dead triggerer slot. GetDef has a
//   same-TU noinline stand-in (gives the short `dec [edi+4] ; jne` release). The sound pair is a named local built from
//   the second PeekDefinition before the call (a temporary pair moves the CSoundInit stores above the call), and
//   CSoundInit assigns PlaybackType in its ctor body (retail stores it after OverrideID).
struct CThing;
struct CTCBase;
struct CEventBase { virtual void v00(); };

namespace NEntityEvents {
struct CEventTriggered : CEventBase {
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}


struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair(long sound) : FirstSound(sound), LastSound(sound) {}
};

struct CSoundInit {
    float         Volume;
    float         Pitch;
    float         Pan;
    long          NoPlays;
    long          PlaybackType;
    unsigned long GainFaderBits;
    unsigned long PitchFaderBits;
    long          OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
    }
};

struct CTCSoundPlayer {
    unsigned long PlaySound(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b);   // 0x0066B280
};

struct CDefPointeeBase {
    virtual void v000();
    virtual void DestroyDef();                          // slot 1
    long m_refCount;                                    // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

struct CThingPhysicalSwitchDef : CDefPointeeBase {
    unsigned char _pad08[0x90 - 8];
    long          TriggerSound;                         // +0x90
};

template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) {}
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingPhysicalSwitchDef>& out);   // 0x0083E0D0
};
void* FindSwitchDef(long index);                        // stand-in callee of the stand-in body below
// Same-TU stand-in for GetDef<CThingPhysicalSwitchDef> (0x0083E0D0 sits in this TU, 0x310 bytes after this function):
// it only writes the out slot, so VC7.1 knows the def pointer's address does not escape (the landed 0x0083D900 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingPhysicalSwitchDef>& out)
{
    void* def = FindSwitchDef(index);
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned short _pad8e;
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingPhysicalSwitch : CThing {
    unsigned char _pad90[0xd0 - 0x90];
    bool          Triggered;                            // +0xd0
    const CThingPhysicalSwitchDef& PeekDefinition() const;   // 0x0083D900
    __forceinline const CThingPhysicalSwitchDef& PeekDefinitionInline() const
    {
        CDefPointer<CThingPhysicalSwitchDef> def;
        GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
        CThingPhysicalSwitchDef* switchDef = def.m_pObject;
        if (switchDef)
            switchDef->ReleaseDef();
        return *switchDef;
    }
    void TriggerMe(CThing* triggerer);
};

void CThingPhysicalSwitch::TriggerMe(CThing* triggerer)
{
    if (Triggered)
        return;
    Triggered = true;
    if (HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
        events->AddEvent(new NEntityEvents::CEventTriggered(triggerer));
    }
    if (PeekDefinitionInline().TriggerSound && HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)GetTC(0x10);
        CSoundPair sound(PeekDefinition().TriggerSound);
        player->PlaySound(sound, CSoundInit(), true, false, false);
    }
}


void CEventBase::v00() {}
static CThing* g_eventTriggerer = 0;
NEntityEvents::CEventTriggered::CEventTriggered(CThing* triggerer) { g_eventTriggerer = triggerer; }
static int g_events = 0; static CEventBase* g_lastEvent = 0;
void CTCEntityEvents::AddEvent(CEventBase* event) { ++g_events; g_lastEvent = event; }
static int g_plays = 0; static long g_sound = 0; static bool g_attach = false;
unsigned long CTCSoundPlayer::PlaySound(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b)
{ if (init.Volume == 1.0f && init.PlaybackType == 1 && init.OverrideID == -1 && sound.FirstSound == sound.LastSound && !a && !b) { ++g_plays; g_sound = sound.FirstSound; g_attach = attach; } return 0; }
static int g_destroyed = 0;
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() { ++g_destroyed; }
static CThingPhysicalSwitchDef g_def;
void* FindSwitchDef(long index) { if (index == 7) { ++g_def.m_refCount; return &g_def; } return 0; }
static CDefinitionManager g_manager;
CDefinitionManager* CThing::GetDefinitionManager() const { return &g_manager; }
const CThingPhysicalSwitchDef& CThingPhysicalSwitch::PeekDefinition() const { return g_def; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }

int main()
{
    static CTCEntityEvents events; static CTCSoundPlayer player;
    static CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x10; entries[0].component = (CTCBase*)&player;
    entries[1].type = 0x42; entries[1].component = (CTCBase*)&events;
    entries[2].type = 0x7fffffff; entries[2].component = 0;   // the end sentinel slot (last->component)
    static CThingPhysicalSwitch sw;
    sw.m_interfaces.first = entries; sw.m_interfaces.last = entries + 2;
    sw.m_presentBits[0] = 0x10000; sw.m_presentBits[2] = 4; sw.m_defGlobalIndex = 7;
    g_def.m_refCount = 1; g_def.TriggerSound = 33;
    CThing* hero = (CThing*)&player;
    sw.TriggerMe(hero);
    if (!sw.Triggered || g_events != 1 || g_eventTriggerer != hero || g_plays != 1 || g_sound != 33 || !g_attach
        || g_def.m_refCount != 1 || g_destroyed != 0) { std::printf("0083ddc0_TEST FAIL\n"); return 1; }
    sw.TriggerMe(hero);                                         // already triggered: nothing happens
    if (g_events != 1 || g_plays != 1) { std::printf("0083ddc0_TEST FAIL\n"); return 1; }
    sw.Triggered = false; g_def.TriggerSound = 0; sw.m_presentBits[2] = 0;
    sw.TriggerMe(hero);                                         // no events component, no sound
    if (!sw.Triggered || g_events != 1 || g_plays != 1) { std::printf("0083ddc0_TEST FAIL\n"); return 1; }
    std::printf("0083ddc0_TEST PASS\n");
    return 0;
}
