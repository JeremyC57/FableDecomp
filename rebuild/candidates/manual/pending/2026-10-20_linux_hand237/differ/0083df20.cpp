// Grade: DIFFER(247v247) -- scheduling only: the CSoundPair temporary's two stores sink below the pushes and the constant 1 lands in ecx (retail stores the pair right after the def peek and keeps 1 in eax)
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
// sound pair share one frame word; the sound pair and the 8-word CSoundInit are temporaries.
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
    bool GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out);   // 0x0083E0D0
};
void* FindDef_CDefinitionManager_GetDef_0083e0d0();
#pragma auto_inline(off)
// Same-TU stand-in for GetDef<CThingPhysicalSwitchDef> 0x0083E0D0: writes the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (retail's LTCG knowledge; the landed 0x0083C1E0 idiom).
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingPhysicalSwitchDef>* out)
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

struct CSoundPair {
    long First;
    long Second;
    CSoundPair(long first, long second) : First(first), Second(second) { }
};
struct CSoundInit {                                            // 0x20 bytes
    float Volume;                                              // +0x00
    float Pitch;                                               // +0x04
    float Delay;                                               // +0x08
    long  LoopCount;                                           // +0x0c
    long  Priority;                                            // +0x10
    long  Pan;                                                 // +0x14
    long  Flags;                                               // +0x18
    long  Handle;                                              // +0x1c
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Delay(0.0f), LoopCount(1), Priority(1), Pan(0), Flags(0), Handle(-1) { }
};
struct CTCSoundPlayer {                                        // the interface-0x10 component
    void PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attached, long a, long b);   // 0x0066B280
};

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
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
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
            player->PlayAttachedSample(CSoundPair(sound, sound), CSoundInit(), true, 0, 0);
        }
    }
}
