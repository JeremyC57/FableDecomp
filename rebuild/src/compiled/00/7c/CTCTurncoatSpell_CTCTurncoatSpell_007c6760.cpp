// CTCTurncoatSpell::CTCTurncoatSpell(CThing* owner) -- build the turncoat-spell component: its will-button helper timer for ability 5, def pointer, intelligent pointers and timers; mark the owner's ability-cast component as busy with ability 5, load SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF, read the owner's turncoat ability level and turn the def's per-level duration tables into whole frames plus fractions
// Leaf: ??0CTCTurncoatSpell
// Retail: Fable.exe @ 0x007C6760, 480 bytes (`ret 4`, no EH frame).
// Shape: the TC 0xA7 / 0x6F lookups are an inline HasTC (masked word) guarding an inline GetTC without its own bit test (const LowerBound for 0x6F); the def name is passed as `const char*` (built before the `push 1`); the frame/fraction pairs go through a named `whole` local.
// Callees: CTCBase ctor 0x00686800, CWillButtonHelperTimer ctor 0x008EDFF0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90,
//   LowerBound 0x0040F020 / 0x004365B0 (const), CCharString ctor/dtor 0x0099EBF0/0x0099EAE0, definition manager 0x0044C6B0 (__fastcall),
//   GetDefIndex 0x009AD390, GetDef<CTurncoatSpellDef> 0x007C7720, CTCInventoryAbilities::GetAbilityLevel 0x005EDE93, __ftol2
class CTCBase;
class CThing;
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
    const SEntry* LowerBound(const long& type) const;   // 0x004365B0
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline void GetInterface(long interfaceType, T** out)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
    template<class T> __forceinline void GetInterfaceC(long interfaceType, T** out) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
};
struct CTCBaseModel {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CTCBaseModel(CThing* owner);                        // 0x00686800
};
struct CWillButtonHelperTimer {
    unsigned char _pad[0x34];
    CWillButtonHelperTimer(long ability, CThing* owner);   // 0x008EDFF0
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
struct CThingIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x0123E7D8
    CThingIntelligentPointer() { }
};
struct CCreatureIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    CCreatureIntelligentPointer() { }
    CCreatureIntelligentPointer(CBaseObject* item) { SetPItem(item); }
};
struct CFloatVector { float* First; float* Last; float* EndOfStorage; };
struct CTurncoatSpellDef {
    unsigned char _pad00[0x3c];
    CFloatVector DurationSeconds;                       // +0x3c
    CFloatVector ChargeSeconds;                         // +0x48
    unsigned char _pad54[0x6c - 0x54];
    CFloatVector CooldownSeconds;                       // +0x6c
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CDefinitionManager { public:
    long GetDefIndex(const CCharString& name, bool required);         // 0x009AD390
    bool GetDef(long index, CDefPointer<CTurncoatSpellDef>* def);     // 0x007C7720
};
CDefinitionManager* __fastcall GetDefinitionManager();  // 0x0044C6B0
struct CTCAbilityCast { unsigned char _pad[0x0c]; bool Busy; unsigned char _pad0d[3]; long Ability; };
class CTCInventoryAbilities { public: long GetAbilityLevel(long ability); };   // 0x005EDE93
extern long g_FramesPerSecond;                          // 0x01375550
class CTCTurncoatSpell : public CTCBaseModel { public:
    CWillButtonHelperTimer Timer;                       // +0x0c
    CDefPointer<CTurncoatSpellDef> Def;                 // +0x40
    CThingIntelligentPointer Target;                    // +0x44
    long Level;                                         // +0x4c
    long State;                                         // +0x50
    CCreatureIntelligentPointer Caster;                 // +0x54
    float DurationFraction;                             // +0x5c
    long Counter;                                       // +0x60
    long DurationFrames;                                // +0x64
    float ChargeFraction;                               // +0x68
    long Counter2;                                      // +0x6c
    long ChargeFrames;                                  // +0x70
    CCreatureIntelligentPointer Creature1;              // +0x74
    CCreatureIntelligentPointer Creature2;              // +0x7c
    CCreatureIntelligentPointer Creature3;              // +0x84
    bool Active;                                        // +0x8c
    long CooldownLeft;                                  // +0x90
    long Cooldown;                                      // +0x94
    CTCTurncoatSpell(CThing* owner);
};

CTCTurncoatSpell::CTCTurncoatSpell(CThing* owner)
    : CTCBaseModel(owner), Timer(5, ParentThing), Level(0), State(0), Caster(0), Counter(0), Counter2(0),
      Creature3(0), Active(false), CooldownLeft(0), Cooldown(0)
{
    if (ParentThing->HasTC(0xa7)) {
        CTCAbilityCast* cast = (CTCAbilityCast*)ParentThing->GetTC(0xa7);
        cast->Busy = true;
        cast->Ability = 5;
    }
    long index = GetDefinitionManager()->GetDefIndex("SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", true);
    GetDefinitionManager()->GetDef(index, &Def);
    if (ParentThing->HasTC(0x6f))
        Level = ((CTCInventoryAbilities*)ParentThing->GetTCC(0x6f))->GetAbilityLevel(5);
    CTurncoatSpellDef* def = Def.Object;
    float duration = def->DurationSeconds.First[Level] / (float)g_FramesPerSecond;
    long whole = (long)duration;
    DurationFrames = whole;
    DurationFraction = duration - (float)whole;
    float charge = def->ChargeSeconds.First[Level] / (float)g_FramesPerSecond;
    whole = (long)charge;
    ChargeFrames = whole;
    ChargeFraction = charge - (float)whole;
    long cooldown = (long)((float)g_FramesPerSecond * def->CooldownSeconds.First[Level]);
    Cooldown = cooldown;
    CooldownLeft = cooldown;
}