// CCreatureAction_CastStrengthMultiStrikeProper::Initialise() -- load SPECIAL_ABILITIES_MULTI_STRIKE_DEF, play its sound for this spell level through the owner's sound component (if any), then CCombatActionBase::Initialise
// Grade: DIFFER(230v226) -- retail loads the def object into ebx right after GetDef, reuses the def pointer's frame slot for the lookup key and the GetString result, and releases through ebx; ours keeps the def pointer in its own slot and re-reads it | tried: function-scope CDefPointer + block-scoped name | next: lane gap: CDefPointer object kept in a register across calls (GOTCHAS wave 25 chunk 163)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6690, length 226 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("SPECIAL_ABILITIES_MULTI_STRIKE_DEF" @ 0x01235A9C), CGameDefinitionManager::Get()
// 0x0044C6B0 (`__fastcall`, twice), the manager's GetDefIndexFromName(const CCharString&, long) 0x009AD390 (manifest label
// "COptimisedPrimitive::ActualToUniqueIndex" is propagated), CDefinitionManager::GetDef<CSpecialAbilitiesMultiStrikeDef>(long,
// CDefPointer*) 0x007CD760, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x10),
// CDefStringTable::GetString(CDefString) 0x009D49B0 on the global table @ 0x013CA828 (CCharString by hidden pointer), the sound
// component's play-by-name 0x0066BD00, CCombatActionBase::Initialise 0x00857520; the def's virtual slot 1 on the last release.
// Shape: the def-name string is a named local in its own block (as the landed CTCHeroStats::OnInitialActivate 0x0057D1A4); the
// def pointer is a function-scope local released at the end (refcount at +4); Sounds[SpellLevel - 1] reads `[def+level*4+3Ch]`.
// Layout: SpellLevel at retail +0x114 (PDB +0x12c); CSpecialAbilitiesMultiStrikeDef Sounds (CDefString[4]) at +0x40 (Ego_r.pdb).
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CDefString { long Index; };

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CSpecialAbilitiesMultiStrikeDef {
    virtual void v00();
    virtual void Release();                     // slot 1 (+4): destroys the def when the last reference goes
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CDefString    Sounds[4];                    // +0x40
    CDefString    Animations[4];                // +0x50
};

struct CDefPointerMultiStrike {                 // CDefPointer<const CSpecialAbilitiesMultiStrikeDef>
    CSpecialAbilitiesMultiStrikeDef* Object;
    CDefPointerMultiStrike() : Object(0) { }
    ~CDefPointerMultiStrike()
    {
        if (Object && --Object->RefCount == 0)
            Object->Release();
    }
    CSpecialAbilitiesMultiStrikeDef* operator->() const { return Object; }
};

struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
    long GetDefIndexFromName(const CCharString& name, long column);       // 0x009AD390
    bool GetDef(long index, CDefPointerMultiStrike* out);                 // 0x007CD760
};

struct CTCSound {                               // the interface-0x10 component
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_CastStrengthMultiStrikeProper : public CCombatActionBase {
public:
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastStrengthMultiStrikeProper::Initialise()
{
    CDefPointerMultiStrike def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_STRIKE_DEF");
        index = CGameDefinitionManager::Get().GetDefIndexFromName(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (PCreature.GetPItem()->GetInterface(&sound, 0x10))
        sound->PlaySound(g_DefStringTable.GetString(def->Sounds[SpellLevel - 1]), 1, 0, 0, -1);
    CCombatActionBase::Initialise();
}
