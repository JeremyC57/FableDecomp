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
class CThing;
class CTCBase;
struct CEventBase { virtual void v00(); };

namespace NEntityEvents {
class CEventTriggered : public CEventBase { public:
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}


class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair(long sound) : FirstSound(sound), LastSound(sound) {}
};

class CSoundInit { public:
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

class CTCSoundPlayer { public:
    unsigned long PlaySound(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b);   // 0x0066B280
};

class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef();                          // slot 1
    long m_refCount;                                    // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

class CThingPhysicalSwitchDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x90 - 8];
    long          TriggerSound;                         // +0x90
};

template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) {}
};

class CDefinitionManager { public:
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

class CThing { public:
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

class CThingPhysicalSwitch : public CThing { public:
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