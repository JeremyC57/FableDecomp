// CTCDExplosion::GiveDamageToThing(CThingGameObject& thing) -- hit a thing caught in the explosion: unless it evades all hits, strike it from the closest point of the blast segment with the max damage (scaled down with distance when the def says so), remember it as hit, and set burnable things on fire
// Leaf: GiveDamageToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080CA20, 464 bytes (`ret 4`), no EH frame.
//         Ego_r.pdb: `?GiveDamageToThing@CTCDExplosion@@AAEXAAVCThingGameObject@@@Z`.
// Callees: NCreatureMode::IsToEvadeAllHits(const CThing&) __fastcall @ 0x0085FA00, CThing::GetPos @ 0x004C73D0,
//          Geometry_ClosestPointOnLineSegment __fastcall @ 0x008F5970 (hidden result in ecx, start in edx, end + point
//          on the stack), CHitParameters ctor (9 args) @ 0x00789238 / dtor @ 0x00789358, AddDamageWithAttribute @
//          0x00868CA0, Geometry_DistancePointToLineSegment __fastcall @ 0x008F5BA0 (inline
//          GetDistanceFromMeAsRatioOfDamageRadius, landed 0x0080C210), the thing's virtual OnHit (+0x104),
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10 (CIntelligentPointer<CThing>
//          vtable 0x01238C6C), vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520, CInterfaceMap::LowerBound @
//          0x0040F020, CTCBurntEffect::Reset @ 0x007CDD10, CThing::AddTC @ 0x004C9D60, CCharString @ 0x0099EBF0 /
//          0x0099EAE0 ("CTCBurntEffect" @ 0x0123D384).
// Shape: the hit direction is an explicit C3DVector(x, y, z) temporary in the CHitParameters argument list (C3DVector has
//        the PDB's user copy constructor): it computes X first and its frame words are reused by the damage float and the
//        intelligent-pointer temporary; an inline `operator-` returning `C3DVector(...)` computes Z first, and a named
//        direction local keeps its own frame words (`sub esp,80h`). The burn test is the raw bitset word (`test ah,2`), the
//        lookup the inline GetTC, the class name a named CCharString in its own block.
// Layout: CTCDExplosion (Ego_r.pdb) with retail 12-byte vectors: DamageRadius +0x0c, MaxDamage +0x14, StartPos +0x20,
//         EndPos +0x2c, PExplosionDef +0x38, Emitters +0x40, HitThings +0x54 (PDB +0x58). CExplosionDef:
//         DamageFalloffWithRadius +0x38, CauseFireEffectOnHitCreatures +0x65 (PDB +0x69, ExplosionRings 12 bytes in
//         retail). Retail CThing: interface bitset +0x20 (TC 0x89 = CTCBurntEffect: word +0x30 bit 9), map +0x44,
//         ThingTypeFlags +0x6C (bit 3 tested here).
class CTCBase;
class CTCParamsBase;
class CBaseObject;
class CThing;
namespace NHitLocations { class CHitLocation; }

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}   // ??0C3DVector@@QAE@ABV0@@Z (Ego_r.pdb)
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x60];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0 (EDamageAttribute)
};

class CTCBurntEffect {
public:
    void Reset();                                   // 0x007CDD10
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPos() const;           // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CThingGameObject : public CThing {
};

namespace NCreatureMode {
    bool __fastcall IsToEvadeAllHits(const CThing& thing);                  // 0x0085FA00
}

C3DVector __fastcall Geometry_ClosestPointOnLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point);   // 0x008F5970
float __fastcall Geometry_DistancePointToLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                     const C3DVector& point);   // 0x008F5BA0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                 // 0x004AD520
};

class CExplosionDef { public:
    unsigned char _pad00[0x38];
    bool DamageFalloffWithRadius;                   // +0x38
    unsigned char _pad39[0x65 - 0x39];
    bool CauseFireEffectOnHitCreatures;             // +0x65
};

class CTCDExplosion {
public:
    void* __vftable;                                // +0x00 (CTCBase)
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    float DamageRadius;                             // +0x0c
    float PhysicsRadius;                            // +0x10
    float MaxDamage;                                // +0x14
    long FireDamage;                                // +0x18
    long ExplosionLifeTimer;                        // +0x1c
    C3DVector StartPos;                             // +0x20
    C3DVector EndPos;                               // +0x2c
    const CExplosionDef* PExplosionDef;             // +0x38 (CDefPointer object word)
    bool CreatedEmitter;                            // +0x3c
    bool NeedToTriggerExplosion;                    // +0x3d
    unsigned char _pad3e[0x40 - 0x3e];
    unsigned char Emitters[12];                     // +0x40
    long ExplosionEvolutionTimer;                   // +0x4c
    long CurrentExplosionRing;                      // +0x50
    CRetailVector<CIntelligentPointer<CThing> > HitThings;   // +0x54
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) const
    {
        return Geometry_DistancePointToLineSegment(StartPos, EndPos, thing->GetPos()) / DamageRadius;
    }
    void GiveDamageToThing(CThingGameObject& thing);
};

void CTCDExplosion::GiveDamageToThing(CThingGameObject& thing)
{
    if (NCreatureMode::IsToEvadeAllHits(thing))
        return;
    C3DVector closest = Geometry_ClosestPointOnLineSegment(StartPos, EndPos, thing.GetPos());
    const C3DVector& position = thing.GetPos();
    CHitParameters hit(0, ParentThing, 0, closest,
                       C3DVector(position.X - closest.X, position.Y - closest.Y, position.Z - closest.Z),
                       false, false, false, false);
    float damage = MaxDamage;
    if (PExplosionDef->DamageFalloffWithRadius) {
        float ratio = GetDistanceFromMeAsRatioOfDamageRadius(&thing);
        if (ratio < 0.0f)
            ratio = 0.0f;
        else if (ratio > 1.0f)
            ratio = 1.0f;
        damage *= 1.0f - ratio * 0.95f;
    }
    hit.AddDamageWithAttribute(5, damage);
    thing.OnHit(hit);
    HitThings.push_back(CIntelligentPointer<CThing>(&thing));
    if (PExplosionDef->CauseFireEffectOnHitCreatures && (thing.ThingTypeFlags & 8)) {
        if (thing.HasTC(0x89))
            thing.GetTC<CTCBurntEffect>(0x89)->Reset();
        else {
            CCharString name("CTCBurntEffect", -1);
            thing.AddTC(name, true, 0);
        }
    }
}