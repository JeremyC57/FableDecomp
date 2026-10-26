// CCreatureAction_CastWillLightningSpell::CCreatureAction_CastWillLightningSpell(CThingCreatureBase*) -- combat action 150: WILL_CAST_LIGHTNING animation, load the lightning spell def and play its cast sound on the creature's sound component
// Leaf: ??0CCreatureAction_CastWillLightningSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BC790, 294 bytes (304 with padding), `ret 4`; vtable 0x0127DB2C.
// Callees: CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CCharString(const char*, long)
//   0x0099EBF0 ("WILL_CAST_LIGHTNING" 0x0127DC70, "SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF" 0x01235B14), ~CCharString
//   0x0099EAE0, CCombatActionBase::SetAnimationName 0x008574D0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall),
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CSpecialAbilitiesLightningSpellDef> 0x007A35D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface type 0x10,
//   the CTCSoundPlayer component: bit 16 of the presence word at +0x20), CDefStringTable::GetString 0x009D49B0 on
//   g_DefStringTable 0x013CA828, CTCSoundPlayer::PlaySound 0x0066BD00.
// Layout: retail CCombatActionBase is 0x114 bytes (PCreature +0x08); this class's first member +0x114 is zeroed; the
//   lightning def's cast-sound string id at +0x110 (the def's RefCount +0x04, Destroy in vtable slot 1).
// Shape: init list (zero CSoundPair temporary, member zero) with the vtable/member stores scheduled into the first
//   string's pushes; the two def-name strings are named locals in their own blocks and the cast sound a named local
//   in the if block, all homed in the dead `owner` argument slot; the def pointer is a CDefPointer local declared
//   after the animation block (shares the CSoundPair's frame words) released by the retry-10 DecRefToZero form.
//   GetDef needs its same-TU stand-in body (below, under auto_inline(off)): without it the def lands in the argument
//   slot, is reloaded for the release and the frame/registers differ (LTCG knowledge that the pointer it fills does
//   not escape).
class CThingCreatureBase;
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

class CSpecialAbilitiesLightningSpellDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x110 - 0x08];
    long CastSound;                                   // +0x110 CDefString id
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<CSpecialAbilitiesLightningSpellDef>* out) const;      // 0x007A35D0
    CDefPointer<CSpecialAbilitiesLightningSpellDef> GetPDefFromGlobalIndex(long index) const; // 0x009AD9E0
};
#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line GetDef<CSpecialAbilitiesLightningSpellDef> 0x007A35D0 (the landed body of that
// address, CDefinitionManager_GetDef007a35d0): compiled above the caller so VC7.1 sees that the def pointer it fills
// does not escape (retail's LTCG knowledge), which keeps the def in ebx for the release at the end.
bool CDefinitionManager::GetDef(long index, CDefPointer<CSpecialAbilitiesLightningSpellDef>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<CSpecialAbilitiesLightningSpellDef> def = GetPDefFromGlobalIndex(index);
    if (def.Object) {
        *out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();  // 0x0044C6B0
};
struct CDefStringTable {
    CCharString GetString(long id);                   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCSoundPlayer { public:                                     // interface type 0x10
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_CastWillLightningSpell : public CCombatActionBase { public:   // vtable 0x0127DB2C
    long m_spellState;                                // +0x114
    virtual void v00();
    CCreatureAction_CastWillLightningSpell(CThingCreatureBase* owner);
};

CCreatureAction_CastWillLightningSpell::CCreatureAction_CastWillLightningSpell(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, CSoundPair()), m_spellState(0)
{
    {
        CCharString animation("WILL_CAST_LIGHTNING");
        SetAnimationName(animation);
    }
    CDefPointer<CSpecialAbilitiesLightningSpellDef> def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSoundPlayer* sound;
    if (PCreature.GetPItem()->GetInterface(&sound, 0x10)) {
        CCharString castSound = g_DefStringTable.GetString(def->CastSound);
        sound->PlayAttachedCriteria(castSound, 0, 0, 0, -1);
    }
}