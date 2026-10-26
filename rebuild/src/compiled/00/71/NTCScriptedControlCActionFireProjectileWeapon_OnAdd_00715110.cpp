// Levers: in-body `#pragma inline_depth(0)` around the reload `Actions.Add(CCountedPointer(new ...))` keeps that temporary's ~CCountedPointer out of line (0x0048C5C0) while the aim/fire temporaries expand it; `hasMissileWeapon` declared before `needsReload` so its false store is an immediate byte (not the bl zero).
// NTCScriptedControl::CActionFireProjectileWeapon::OnAdd() -- queue reload (when needed), aim and fire creature actions for a creature holding a missile weapon
// Layout: owning control at +4 (its +4 is the creature, as in the landed 0x007122D0); target CScriptThing at +0xC; the creature-action queue at +0x18.
// Callees: CVectorMap LowerBound copy @ 0x004365B0, ReloadMissileWeapon ctor 0x00718710, aim ctor 0x00718650, fire ctor 0x008D5110,
// CCountedPointer(T*) 0x00695880 (out of line), ~CCountedPointer 0x0048C5C0, queue add 0x00719360.
class CTCBase;
class CThing;
struct CCreatureAction { };
class CCreatureAction_ReloadMissileWeapon : public CCreatureAction { public: char _d[0xb0]; CCreatureAction_ReloadMissileWeapon(CThing* creature); };   // 0x00718710
class CCreatureAction_AimMissileWeapon : public CCreatureAction { public: char _d[0xb0]; CCreatureAction_AimMissileWeapon(CThing* creature); };         // 0x00718650
class CCreatureAction_FireMissileWeapon : public CCreatureAction { public: char _d[0xc8]; CCreatureAction_FireMissileWeapon(CThing* creature, CThing* target); };   // 0x008D5110

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCountedPointer {
    CCreatureAction* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(CCreatureAction* object);    // 0x00695880
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
struct CCreatureActionQueue { void Add(const CCountedPointer& action); };   // 0x00719360

class CTCInventoryAbilities { public: CThing* GetWeaponInHand(); };    // 0x006A1440
class CTCWeapon { public: bool IsMissileWeapon(); bool IsLoaded(); };  // 0x005DAA40 / 0x005DA980

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;            // 0x004365B0
};

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x91 - 0x4c];
    unsigned char       Flags91;                     // +0x91
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    long _a, _b;
};

struct CCreaturePointer { void* __vftable; CThing* PThing; };

class CActionFireProjectileWeapon {
public:
    void* __vftable;                                 // +0x00
    CCreaturePointer* PControl;                      // +0x04
    char _pad08[0xc - 0x8];
    CScriptThing PTargetThing;                       // +0x0C
    CCreatureActionQueue Actions;                    // +0x18
    void OnAdd();
};

void CActionFireProjectileWeapon::OnAdd()
{
    CThing* creature = PControl->PThing;
    bool hasMissileWeapon = false;
    bool needsReload = false;
    CTCInventoryAbilities* inventory;
    if (creature->GetInterface(&inventory, 0x46)) {
        CThing* weapon = inventory->GetWeaponInHand();
        CTCWeapon* tcWeapon;
        if (weapon && !(weapon->Flags91 & 1) && weapon->GetInterface(&tcWeapon, 0x48) && tcWeapon->IsMissileWeapon()) {
            hasMissileWeapon = true;
            needsReload = !tcWeapon->IsLoaded();
        }
    }
    if (PTargetThing.IsAlive() && hasMissileWeapon) {
#pragma inline_depth(0)
        if (needsReload)
            Actions.Add(CCountedPointer(new CCreatureAction_ReloadMissileWeapon(creature)));
#pragma inline_depth()
        Actions.Add(CCountedPointer(new CCreatureAction_AimMissileWeapon(creature)));
        Actions.Add(CCountedPointer(new CCreatureAction_FireMissileWeapon(creature, PTargetThing.GetThing())));
    }
}