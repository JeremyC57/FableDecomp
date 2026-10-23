// CTCDrainLife::DoDrain(CThingCreatureBase* target) -- one drain-life pulse: the caster makes a SIMPLE_NOISE; the damage is the def's per-level value (or the data sub-def's) capped at the target's health; the target is hit with it (attribute 6) from the caster's direction and gets a special-ability hit event; unless the hit was blocked or the target died, spawn the def's DAMAGE_EFFECT particle on the target and, when the target can be drained, fire the per-level number of drain shots back at the caster, each carrying its share of the damage and the soul effect
// Grade: DIFFER(1172v1138) -- control flow, callees and inline lookups as retail; residue is FPO register/frame allocation: retail keeps ebx as the zero register through the shot loop (this and the shot def index in frame slots) where ours puts this / the shot def index in ebx and pushes immediates; the particle is not kept in esi; frame 0xB8 v 0xAC (the GetTC key word and the DAMAGE_EFFECT name share a slot in retail)
// Label: 0x007C3070 is called on CTCDrainLife (this) here -- the drain's SOUL_EFFECT parameter getter, not CCreatureActionBase::GetActionSpeedParameter
// Leaf: ?DoDrain@CTCDrainLife
//
// Retail 0x007C3220, 1152 bytes (`ret 4`; FPO frame 0xAC). Default /O2 TU.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CThingCreatureBase::MakeNoise 0x00662110, the def's
//   GetSubDef<CSpecialAbilitiesDrainLifeDataDef> 0x007C3AD0 and GetSubDef<CSpecialEffectsDef> 0x00669580, CThing::GetPosition
//   0x004C73D0, CTCGraphicAppearance::GetHelperPoint 0x004C0560, CHitParameters(...) 0x00789238 / ~ 0x00789358,
//   AddDamageWithAttribute 0x00868CA0, the target's OnHit (vslot 65, +0x104), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020, operator new 0x00BFEA1A, CEventHitBySpecialAbility 0x00811830, CTCEntityEvents::AddEvent 0x008113C0,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateParticleEffect 0x0072AB70 (__fastcall), IS_THING_ALIVE 0x0040DCE0,
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CThing::GetDefinitionManager 0x004C7990, GetDefGlobalIndexFromName 0x009AD410,
//   CreateShot 0x007C0AD0 (__fastcall), the shot's vslot 28 (+0x70), CTCShotDefinitionDrainLife::SetSoulEffect 0x007BE05C,
//   the drain's soul-effect parameter 0x007C3070 (catalog: CCreatureActionBase::GetActionSpeedParameter), CBaseIntelligentPointer
//   0x00A01B10 / SetPItem 0x00A01B90 / ~ 0x00A01C10, vector<CIntelligentPointer<CThing>>::push_back 0x004AD520.
// Layout: CTCDrainLife per the landed ctor 0x007C30E0 (PDef +0x0C, SpellLevel +0x10, Shots +0x1C); retail drain-life def
//   per-level vectors: damage +0x3C, shot count +0x48, shot def names +0x60; data sub-def damage +0x28; CThing bitset +0x20,
//   interface map +0x44, +0x60 graphic (vslot 72 +0x120 the orientation), appearance +0x64, PBaseDef +0x70, flags +0x91;
//   creature health +0xB4, drain flags +0xBC; CHitParameters 0x60 bytes; shot def SoulDamage +0xA4.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
extern CCharString g_EmptyCCharString;                  // 0x013BAC48
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
struct CRightHandedSet;
class CThing;
class CTCBase;

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(T* p) : Object(p) { if (p) ++p->RefCount; }
    CDefPointer(const CDefPointer& o);
    ~CDefPointer() { T* p = Object; if (p && --p->RefCount == 0) p->Destroy(); }
    T* operator->() const { return Object; }
};
template<class T> struct CTempDefPointer {              // by-value argument form (released and cleared)
    T* Object;
    CTempDefPointer() : Object(0) {}
    CTempDefPointer(const CTempDefPointer& o);
    ~CTempDefPointer() { T* p = Object; if (p) { if (--p->RefCount == 0) p->Destroy(); Object = 0; } }
};
struct CSpecialAbilitiesDrainLifeDataDef : CDefBase {
    long  _pad08[(0x28 - 0x08) / 4];
    float Damage;                                       // +0x28
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long* effect);   // 0x00669D30
};
struct CMeleeCombatAbilityDef : CDefBase {};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CSpecialAbilitiesDrainLifeDataDef>* out);   // 0x007C3AD0
};
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CSpecialAbilitiesDrainLifeDef : CDefBase {
    long                       _pad08[(0x3c - 0x08) / 4];
    CRetailVector<float>       Damage;                  // +0x3C
    CRetailVector<long>        NumShots;                // +0x48
    CRetailVector<long>        _vec54;                  // +0x54
    CRetailVector<CCharString> ShotNames;               // +0x60
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};

class CHitLocation;
struct CHitParameters {
    long          _pad00[0x20 / 4];
    bool          KnockDown;                            // +0x20
    bool          Blockable;                            // +0x21
    bool          Blocked;                              // +0x22
    bool          Parried;                              // +0x23
    bool          Flourish;                             // +0x24
    unsigned char _pad25[0x60 - 0x25];
    CHitParameters(CHitLocation* location, CThing* hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);   // 0x00789238
    ~CHitParameters();                                  // 0x00789358
    void AddDamageWithAttribute(long attribute, float value);   // 0x00868CA0
};
struct CEventBase {};
namespace NEntityEvents {
struct CEventHitBySpecialAbility : CEventBase {
    long _body[6];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset);   // 0x006E0BE0
};
struct CTCShotDefinitionDrainLife {
    long  _pad00[0xa4 / 4];
    float SoulDamage;                                   // +0xA4
    void SetSoulEffect(long effect);                    // 0x007BE05C
};
struct CGraphic {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03(); virtual void g04(); virtual void g05();
    virtual void g06(); virtual void g07(); virtual void g08(); virtual void g09(); virtual void g10(); virtual void g11();
    virtual void g12(); virtual void g13(); virtual void g14(); virtual void g15(); virtual void g16(); virtual void g17();
    virtual void g18(); virtual void g19(); virtual void g20(); virtual void g21(); virtual void g22(); virtual void g23();
    virtual void g24(); virtual void g25(); virtual void g26(); virtual void g27(); virtual void g28(); virtual void g29();
    virtual void g30(); virtual void g31(); virtual void g32(); virtual void g33(); virtual void g34(); virtual void g35();
    virtual void g36(); virtual void g37(); virtual void g38(); virtual void g39(); virtual void g40(); virtual void g41();
    virtual void g42(); virtual void g43(); virtual void g44(); virtual void g45(); virtual void g46(); virtual void g47();
    virtual void g48(); virtual void g49(); virtual void g50(); virtual void g51(); virtual void g52(); virtual void g53();
    virtual void g54(); virtual void g55(); virtual void g56(); virtual void g57(); virtual void g58(); virtual void g59();
    virtual void g60(); virtual void g61(); virtual void g62(); virtual void g63(); virtual void g64(); virtual void g65();
    virtual void g66(); virtual void g67(); virtual void g68(); virtual void g69(); virtual void g70(); virtual void g71();
    virtual const CRightHandedSet* GetRHSet();          // slot 72 (+0x120)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11();
    virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16(); virtual void t17();
    virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23();
    virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27();
    virtual void SetSlot28(long value);                 // slot 28 (+0x70)
    virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34();
    virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40();
    virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46();
    virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52();
    virtual void t53(); virtual void t54(); virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58();
    virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64();
    virtual void OnHit(CHitParameters& hit);            // slot 65 (+0x104)
    long                  _pad04[(0x20 - 0x04) / 4];
    unsigned long         m_presentBits[9];             // +0x20
    CInterfaceMap         m_interfaces;                 // +0x44
    long                  _pad4c[(0x60 - 0x4c) / 4];
    CGraphic*             Graphic;                      // +0x60
    CTCGraphicAppearance* Appearance;                   // +0x64
    long                  _pad68[(0x70 - 0x68) / 4];
    CDefClassBase*        PBaseDef;                     // +0x70
    long                  _pad74[(0x90 - 0x74) / 4];
    unsigned char         _pad90;
    unsigned char         ThingFlags;                   // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    long  _pad92[(0xb4 - 0x94) / 4];
    float Health;                                       // +0xB4
    long  _padb8;
    unsigned char DrainFlags;                           // +0xBC
    void MakeNoise(const CCharString& noiseDefName);    // 0x00662110
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position);   // 0x0072AB70
CThing* __fastcall CreateShot(long shotDef, CThing* target, CThing* weapon, const CThing* owner, float speed, float accuracy,
                              float spread, const CTempDefPointer<CMeleeCombatAbilityDef>& ability, const C3DVector* position,
                              const CRightHandedSet* orientation);   // 0x007C0AD0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
};
struct CShotVector {
    CIntelligentPointer<CThing>* _M_start;
    CIntelligentPointer<CThing>* _M_finish;
    CIntelligentPointer<CThing>* _M_end_of_storage;
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004AD520
};

class CTCDrainLife {
public:
    virtual void v00();
    CThing*                        ParentThing;         // +0x04
    long                           _pad08;
    CSpecialAbilitiesDrainLifeDef* PDef;                // +0x0C (CDefPointer word)
    long                           SpellLevel;          // +0x10
    long                           PItemRemoved[2];     // +0x14
    CShotVector                    Shots;               // +0x1C
    long GetSoulEffect();                               // 0x007C3070
    void DoDrain(CThingCreatureBase* target);
};

void CTCDrainLife::DoDrain(CThingCreatureBase* target)
{
    CThingCreatureBase* caster = (CThingCreatureBase*)ParentThing;
    caster->MakeNoise("SIMPLE_NOISE");
    float damage = PDef->Damage._M_start[SpellLevel];
    CDefPointer<CSpecialAbilitiesDrainLifeDataDef> data;
    bool hasData;
    {
        CDefPointer<CDefClassBase> def(ParentThing->PBaseDef);
        hasData = def->GetSubDef(&data);
    }
    if (hasData)
        damage = data->Damage;
    float health = target->Health;
    if (damage > health)
        damage = health;
    C3DVector position = target->GetPosition();
    CTCGraphicAppearance* appearance = target->Appearance;
    if (appearance)
        appearance->GetHelperPoint(g_EmptyCCharString, 0, 0, &position, 1.0f);
    const C3DVector& from = caster->GetPosition();
    const C3DVector& to = target->GetPosition();
    C3DVector direction;
    direction.X = to.X - from.X;
    direction.Y = to.Y - from.Y;
    direction.Z = to.Z - from.Z;
    CHitParameters hit(0, caster, 0, position, direction, false, false, false, false);
    hit.AddDamageWithAttribute(6, damage);
    hit.Blockable = true;
    hit.KnockDown = false;
    hit.Flourish = false;
    target->OnHit(hit);
    if (!(target->ThingFlags & 1) && target->HasTC(0x42)) {
        CTCEntityEvents* events = target->GetTC<CTCEntityEvents>(0x42);
        events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 6));
    }
    if (!hit.Blocked && !hit.Parried && !(target->ThingFlags & 1)) {
        CDefPointer<CSpecialEffectsDef> effects;
        long effect = 0;
        CCharString effectName("DAMAGE_EFFECT");
        if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(effectName, &effect)) {
            CThing* particle = CreateParticleEffect(effect, &target->GetPosition());
            if (IS_THING_ALIVE(particle) && particle->HasTC(0x4b))
                particle->GetTC<CTCDParticleEmitter>(0x4b)->AttachToThing(*target, 0xc, g_EmptyCCharString, 0, 0.0f);
        }
        if (target->DrainFlags & 1) {
            long numShots = PDef->NumShots._M_start[SpellLevel];
            float shotDamage = damage / (float)numShots;
            long shotDef = caster->GetDefinitionManager()->GetDefGlobalIndexFromName(PDef->ShotNames._M_start[SpellLevel]);
            for (long i = 0; i < numShots; ++i) {
                C3DVector from = target->GetPosition();
                if (appearance)
                    appearance->GetHelperPoint(g_EmptyCCharString, 0, 0, &from, 1.0f);
                CThing* shot = CreateShot(shotDef, target, 0, caster, 100.0f, 1.0f, 0.0f, CTempDefPointer<CMeleeCombatAbilityDef>(),
                                          &from, target->Graphic->GetRHSet());
                if (shot && !(shot->ThingFlags & 1)) {
                    shot->SetSlot28(0);
                    if (shot->HasTC(0x77)) {
                        CTCShotDefinitionDrainLife* shotDefinition = shot->GetTC<CTCShotDefinitionDrainLife>(0x77);
                        shotDefinition->SoulDamage = shotDamage;
                        shotDefinition->SetSoulEffect(GetSoulEffect());
                    }
                    Shots.push_back(CIntelligentPointer<CThing>(shot));
                }
            }
        }
    }
}
