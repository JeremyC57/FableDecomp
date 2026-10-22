// CCreatureAction_TrollThrowProjectile::Initialise() -- name the animation PROJECTILE_ATTACK_THROW_ROCK and run the
//   combat base Initialise; create the projectile thing named by the creature's ability def at the creature's
//   position, remember it, and (when the creature can carry) put it in the CARRY_SLOT_TROLL_THROW_ROCK(_LEFT) slot;
//   then roll 0..99 against the creature's willpower-indicator def chance and copy its value into the TC 0x30
// Leaf: ?Initialise@CCreatureAction_TrollThrowProjectile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C64F0, length 688 bytes, plain `ret` (virtual).
// Callees: SetAnimationName 0x008574D0, CCombatActionBase::Initialise 0x00857520, GetPItem 0x00A01B50, CDefClassBase
//   GetSubDef<CAbilityDef> 0x0042B1A6 / GetSubDef<CAICreatureWillPowerIndicatorDef> 0x008C78F0, GetPWorld 0x00692940,
//   CDefinitionManager::GetDef<physical def> 0x0078B230, CThing::GetPosition 0x004C73D0, CThingObject::Create
//   0x00703210 (__fastcall), CIntelligentPointer ctor 0x00A01B10 / Set 0x00A01B90 / dtor 0x00A01C10, LowerBound
//   0x0040F020, 0x00669CB0 (weapon-hand query, out param), GetPDefinitionManager 0x00692910, GetDefGlobalIndex
//   0x009AD410, CTCCarrying::AddThing(CThing*, long, bool) 0x006A1F80, CTCGraphicAppearance 0x004BFAC0,
//   GetRandomSeed 0x0049D810, GFROR13 0x00497890 (__fastcall), CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Layout: action +0x08 PCreature, +0x74 combat TC pointer, +0x114 PProjectile, +0x12c long flag (retail bytes);
//   creature +0x70 def, +0x90 player (signed char), +0x64 graphic appearance TC.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CAbilityDef : CDefBase {
    unsigned char _pad08[0xa0 - 8];
    long          ProjectileDef;                             // +0xa0
};
struct CPhysicalDef : CDefBase {
    unsigned char _pad08[0x7c - 8];
    long          DefIndex;                                  // +0x7c
};
struct CWillPowerIndicatorDef : CDefBase {
    unsigned char _pad08[0x48 - 8];
    float         Chance;                                    // +0x48
    long          Value;                                     // +0x4c
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out);           // 0x0042B1A6
    bool GetSubDef(CDefPointer<CWillPowerIndicatorDef>* out);   // 0x008C78F0
};
struct CDefinitionManager {
    long GetDefGlobalIndex(const CCharString& name);         // 0x009AD410
};
struct CPhysicalDefManager {
    bool GetDef(long index, CDefPointer<CPhysicalDef>* out);   // 0x0078B230
};
CAbilityDef* FindAbilityDef(const CDefClassBase* def);                 // stand-ins: the def lookups (not graded here)
CWillPowerIndicatorDef* FindWillPowerDef(const CDefClassBase* def);
CPhysicalDef* FindPhysicalDef(long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out)
{
    CAbilityDef* def = FindAbilityDef(this);
    out->Object = def;
    return def != 0;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWillPowerIndicatorDef>* out)
{
    CWillPowerIndicatorDef* def = FindWillPowerDef(this);
    out->Object = def;
    return def != 0;
}
__declspec(noinline) bool CPhysicalDefManager::GetDef(long index, CDefPointer<CPhysicalDef>* out)
{
    CPhysicalDef* def = FindPhysicalDef(index);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char        _pad00[0x10];
    CPhysicalDefManager* PDefs;                              // +0x10
    unsigned long* GetRandomSeed();                          // 0x0049D810
};
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing;
struct CTCCarrying {
    void AddThing(CThing* thing, long slot, bool now);       // 0x006A1F80
};
struct CTCGraphicAppearance {
    void SetVisibleFlag(unsigned char flag);                 // 0x004BFAC0
};
struct CTCWillPower {
    unsigned char _pad00[0x7c];
    long          Value;                                     // +0x7c
};
class CThing {
public:
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];                  // +0x20
    CInterfaceMap         m_interfaces;                      // +0x44
    unsigned char         _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;                // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                              // +0x70
    unsigned char         _pad74[0x90 - 0x74];
    signed char           Player;                            // +0x90
    const C3DVector* GetPosition() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    ~CIntelligentPointerThing();                             // 0x00A01C10
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct SWeaponHand {
    unsigned char _pad00[8];
    long          Hand;                                      // +0x08
};
struct CCombatTC {
    void GetWeaponHand(SWeaponHand** out);                   // 0x00669CB0
};
SWeaponHand* FindWeaponHand(const CCombatTC* tc);            // stand-in: the hand lookup (not graded here)
#pragma auto_inline(off)
__declspec(noinline) void CCombatTC::GetWeaponHand(SWeaponHand** out)
{
    *out = FindWeaponHand(this);
}
#pragma auto_inline(on)
struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                               // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    unsigned char           _pad10[0x74 - 0x10];
    CCombatTC*              PCombat;                         // +0x74
    void SetAnimationName(const CCharString& name);          // 0x008574D0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefinitionManager* GetPDefinitionManager() const;       // 0x00692910
    CWorld* GetPWorld() const;                               // 0x00692940
};
struct CCreatureAction_TrollThrowProjectile : CCombatActionBase {
    unsigned char            _pad78[0x114 - 0x78];
    CIntelligentPointerThing PProjectile;                    // +0x114
    unsigned char            _pad11c[0x12c - 0x11c];
    long                     ProjectileVisible;              // +0x12c
    virtual void Initialise();
};

void CCreatureAction_TrollThrowProjectile::Initialise()
{
    {
        CCharString animation("PROJECTILE_ATTACK_THROW_ROCK");
        SetAnimationName(animation);
    }
    CCombatActionBase::Initialise();
    CDefPointer<CAbilityDef> ability;
    CThing* self = GetCreature();
    self->PDef->GetSubDef(&ability);
    CDefPointer<CPhysicalDef> projectileDef;
    CWorld* world = GetPWorld();
    world->PDefs->GetDef(ability->ProjectileDef, &projectileDef);
    CThingObject* thing;
    {
        CCharString name("");
        CThing* owner = GetCreature();
        CThing* creature = GetCreature();
        thing = CThingObject::Create(projectileDef->DefIndex, creature->GetPosition(), owner->Player, 0, 0, name);
    }
    if (thing) {
        {
            PProjectile = CIntelligentPointerThing(thing);
        }
        CTCCarrying* carrying;
        if (GetCreature()->GetInterface(&carrying, 0x46)) {
            SWeaponHand* hand;
            PCombat->GetWeaponHand(&hand);
            long slot;
            if (hand->Hand == 1)
                slot = GetPDefinitionManager()->GetDefGlobalIndex("CARRY_SLOT_TROLL_THROW_ROCK_LEFT");
            else
                slot = GetPDefinitionManager()->GetDefGlobalIndex("CARRY_SLOT_TROLL_THROW_ROCK");
            carrying->AddThing(thing, slot, true);
        }
        ProjectileVisible = 0;
        thing->PGraphicAppearance->SetVisibleFlag((unsigned char)ProjectileVisible);
    }
    CDefPointer<CWillPowerIndicatorDef> willPower;
    CDefClassBase* creatureDef = GetCreature()->PDef;
    if (creatureDef->GetSubDef(&willPower)) {
        unsigned long roll = GFRandom(GetPWorld()->GetRandomSeed()) % 100;
        CTCWillPower* tc;
        if ((float)roll < willPower->Chance && GetCreature()->GetInterface(&tc, 0x30) && willPower->Value > 0)
            tc->Value = willPower->Value;
    }
}