// CThingGameObject::GetMeleeHitParameters(CHitLocation*, CThing& hitter, CThing* weapon, const C3DVector& position,
// const C3DVector& direction, CHitParameters& params) const -- fills a melee hit: the hitter/weapon/location, a localised
// directional hit, the creature action's flourish/knock-down/blockable/decapitate flags and damage multiplier, the hitter's
// weapon-modifier, experience and definition multipliers, the hitter's charge component, then adds the damage
// Retail: Fable.exe (TLC, VC7.1) @ 0x00838BC0, `ret 0x18`.
// Layout (retail; Ego_r.pdb in brackets): CThing interface bitset +0x20, map +0x44, ThingTypeFlags +0x6c [+0x7c],
//   PBaseDef +0x70 [+0x80], Dead +0x91 bit 0; creature current action +0x100; CHitParameters Position +0x2, Direction +0xe.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CCountedBase {
    virtual void v00();
    virtual void DeleteThis();                       // slot 1
    long RefCount;                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            DeleteThis();
    }
};
template<class T> struct CCountedPointer {
    T* Object;
    __forceinline CCountedPointer() : Object(0) { }
    __forceinline CCountedPointer(const CCountedPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->DeleteThis();
    }
    T* operator->() const { return Object; }
};
struct CTCHeroExperience;
struct CExperienceCurve {
    float GetAbilityMultiplier(const CTCHeroExperience* experience) const;   // 0x00851BD0
};
struct CExperienceDef : CCountedBase {
    unsigned char _pad08[0xa8 - 0x08];
    float High;                                      // +0xa8
    float Low;                                       // +0xac
    CExperienceCurve Curve;                          // +0xb0
};
struct CTCHeroExperience {                           // interface 0x68
    unsigned char _pad00[0x58];
    CCountedPointer<CExperienceDef> Def;             // +0x58
};
struct CTCWeaponModifier {                           // interface 0x87 (descriptive)
    bool CausesKnockDown();                          // 0x007CCA60
    float GetDamageMultiplier();                     // 0x007CCA80
};
struct CTCCharge {                                   // interface 0x84 (descriptive)
    float GetDamageMultiplier() const;               // 0x0078BE50
    bool IsDecapitating() const;                     // 0x0078BFC0
    void GetHitPosition(C3DVector* out) const;       // 0x0078C150
};
struct CTCWeapon {                                   // interface 0x48 (descriptive)
    bool HasDamage() const;                          // 0x005DAA00
    long GetDamageAttribute() const;                 // 0x005DA810
    float GetDamage() const;                         // 0x005DAAA0
};
struct CDamageMultiplierDef : CCountedBase {
    unsigned char _pad08[0x44 - 0x08];
    float DamageMultiplier;                          // +0x44
};
struct CThingBaseDef {
    bool GetDamageMultiplierDef(CDefPointer<CDamageMultiplierDef>* out) const;   // 0x00696130
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool UsesDamageMultiplier();             // slot 15 (+0x3c)
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual bool IsKnockDown();                      // slot 19 (+0x4c)
    virtual void v20();
    virtual bool IsFlourish();                       // slot 21 (+0x54)
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28();
    virtual bool IsBlockable();                      // slot 29 (+0x74)
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual bool IsDecapitate();                     // slot 54 (+0xd8)
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual float GetDamageMultiplier();             // slot 80 (+0x140)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                    // +0x6c
    const CThingBaseDef* PBaseDef;                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Dead : 1;                          // +0x91 bit 0
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* CurrentAction;              // +0x100 (creatures only)
    float GetBaseMeleeDamage();                      // 0x00662190
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetConstInterface(const T** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
};
namespace NHitLocations { struct CHitLocation; }
#pragma pack(push, 1)
struct CHitParameters {
    bool Localised;                                  // +0x00
    bool Directional;                                // +0x01
    C3DVector Position;                              // +0x02
    C3DVector Direction;                             // +0x0e
    bool KnockDown;                                  // +0x1a
    bool Decapitate;                                 // +0x1b
    bool Blockable;                                  // +0x1c
    bool Flourish;                                   // +0x1d
    bool EpicSpell;                                  // +0x1e
    bool BlockCounter;                               // +0x1f
    bool PlayHitResponse;                            // +0x20
    bool PlayHitResponseOverrideSet;                 // +0x21
    bool MoveBack;                                   // +0x22
    void SetThingHitting(CThing& thing);             // 0x008685F0
    void SetHitLocation(NHitLocations::CHitLocation* location);   // 0x00868590
    void SetWeapon(CThing* weapon);                  // 0x00868CE0
    void AddDamageWithAttribute(long attribute, float damage);    // 0x00868CA0
};
#pragma pack(pop)
struct CThingGameObject : CThing {
    void GetMeleeHitParameters(NHitLocations::CHitLocation* location, CThing& hitter, CThing* weapon,
                               const C3DVector& position, const C3DVector& direction, CHitParameters& params) const;
};
// R&D I-3 sweep: a same-TU auto_inline(off) GetDamageMultiplierDef (fills only *out) lets the 0x87/0x68 interface keys and the
// damage-multiplier def share the params argument slot, as in retail.
CDamageMultiplierDef* FindDamageMultiplierDef(const CThingBaseDef* def);
#pragma auto_inline(off)
bool CThingBaseDef::GetDamageMultiplierDef(CDefPointer<CDamageMultiplierDef>* out) const
{
    out->Object = FindDamageMultiplierDef(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
void CThingGameObject::GetMeleeHitParameters(NHitLocations::CHitLocation* location, CThing& hitter, CThing* weapon,
                                             const C3DVector& position, const C3DVector& direction,
                                             CHitParameters& params) const
{
    params.SetThingHitting(hitter);
    if (weapon && !weapon->Dead)
        params.SetWeapon(weapon);
    if (location)
        params.SetHitLocation(location);
    params.Position = position;
    params.Localised = true;
    params.Direction = direction;
    params.Directional = true;
    float multiplier = 1.0f;
    if (hitter.ThingTypeFlags & 8) {
        CCreatureActionBase* action = hitter.CurrentAction;
        if (action) {
            if (action->IsFlourish())
                params.Flourish = true;
            if (action->IsKnockDown())
                params.KnockDown = true;
            if (!action->IsBlockable())
                params.Blockable = false;
            if (action->IsDecapitate())
                params.Decapitate = true;
            if (action->UsesDamageMultiplier())
                multiplier = action->GetDamageMultiplier();
        }
    }
    float weaponMultiplier = 1.0f;
    CTCWeaponModifier* modifier;
    if (weapon && !weapon->Dead && weapon != &hitter && hitter.GetInterface(&modifier, 0x87)) {
        weaponMultiplier = modifier->GetDamageMultiplier();
        params.MoveBack = false;
        params.Blockable = false;
        if (modifier->CausesKnockDown())
            params.KnockDown = true;
    }
    const CTCHeroExperience* experience;
    if (hitter.GetConstInterface(&experience, 0x68)) {
        CCountedPointer<CExperienceDef> def = experience->Def;
        float t = def->Curve.GetAbilityMultiplier(experience);
        float low = def->Low;
        multiplier *= low + (def->High - low) * t;
    }
    const CThingBaseDef* baseDef = hitter.PBaseDef;
    CDefPointer<CDamageMultiplierDef> damageDef;
    if (baseDef->GetDamageMultiplierDef(&damageDef))
        multiplier *= damageDef->DamageMultiplier;
    const CTCCharge* charge;
    if (hitter.GetConstInterface(&charge, 0x84)) {
        multiplier = charge->GetDamageMultiplier() * multiplier;
        charge->GetHitPosition(&params.Position);
        params.KnockDown = true;
        params.Decapitate = charge->IsDecapitating();
    }
    if (HasTC(0x84)) {
        params.PlayHitResponseOverrideSet = true;
        params.PlayHitResponse = false;
    }
    long attribute;
    float damage;
    const CTCWeapon* tc;
    if (weapon && !weapon->Dead && weapon->GetConstInterface(&tc, 0x48) && tc->HasDamage()) {
        damage = tc->GetDamage();
        attribute = tc->GetDamageAttribute();
    } else {
        attribute = 1;
        damage = hitter.GetBaseMeleeDamage();
    }
    params.AddDamageWithAttribute(attribute, damage * weaponMultiplier * multiplier);
}