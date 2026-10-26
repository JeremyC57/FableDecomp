// NCreatureMode::CModeKnockedDown::OnActivate() -- show emote icon 8 (priority 0, 0.1 s, full scale and alpha), reset the interface-0x30 component's ability timers, suspend a player creature's animation, then the base activation
// Leaf: ?OnActivate@CModeKnockedDown
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9480, 166 bytes (+ int3 padding), `ret`; vtable 0x01267E78 slot 5.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: type 0x45 = bit 5 of +0x28 = CTCEmoteIcon, type 0x30 = bit 16
//   of +0x24), CTCEmoteIcon::SetEmoteIcon(long, long, float, float, unsigned char) @ 0x0071AAB0 (landed), the 0x30
//   component's call @ 0x007340B0 (manifest label "TCBase_InvalidateAllAbilityTimers", kept as a hint; this is its only
//   caller), CThingAICreature::SetSuspendAnimation(bool, long) @ 0x00833A10, CModeBase::OnActivate @ 0x0073AAC0 (qualified).
// Name: FableWin.pdb `?OnActivate@CModeKnockedDown@NCreatureMode@@UAEXXZ` (slot 5).
// Shape: the emote lookup is the out-parameter GetInterface (`mov eax,[eax+4] ; push ... ; mov ecx,eax`), the 0x30 lookup
//   the HasTC + one-ternary GetTC on the non-const map (component straight into ecx); ParentThing is re-read for each block.
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

class CThingAICreature { public:
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

class CThing : public CThingAICreature { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                  // +0x10 (1 = player creature)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
};

class CTCEmoteIcon { public:                      // interface type 0x45
    void SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha);   // 0x0071AAB0
};
struct CTCInterface30 {                    // interface type 0x30
    void InvalidateAllAbilityTimers();     // 0x007340B0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    void OnActivate();                     // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeKnockedDown : CModeBase {      // vtable 0x01267E78
    unsigned char Filter[0x0c];            // +0x1c CAnimFilterOfGroup
    void OnActivate();
};
}

void NCreatureMode::CModeKnockedDown::OnActivate()
{
    CTCEmoteIcon* emote;
    if (ParentThing->GetInterface(&emote, 0x45))
        emote->SetEmoteIcon(8, 0, 0.1f, 1.0f, 0xff);
    if (ParentThing->HasTC(0x30))
        ParentThing->GetTC<CTCInterface30>(0x30)->InvalidateAllAbilityTimers();
    if (ParentThing->m_type == 1)
        ParentThing->SuspendAI(true, 0);
    CModeBase::OnActivate();
}