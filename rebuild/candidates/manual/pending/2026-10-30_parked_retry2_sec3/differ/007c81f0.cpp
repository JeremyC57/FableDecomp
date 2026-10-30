// CTCHitOnCollision::OnCollision(const C3DVector& point, const C3DVector& normal, CThing& thing) -- hit a game object the owner collides with (once per object): optionally skip things in a combat action, push sideways from the owner's facing, apply the damage, and on a landed hit remember it and make the owner not solid to it
// Grade: DIFFER(768v768) -- the solid-to-thing lookup in the bool GetInterface(T**, long) out-parameter form (the component through eax as retail); residue: iterator-tag byte slot (0x27 v 0x33), begin() read order, the CurrentAction pointer tested in eax (retail ecx), direction/position copy slots and the Normalise multiply/scatter order
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void RotateAboutZ(float turns);                     // 0x00A14040
    __forceinline void Normalise()
    {
        float lengthSquared = X * X + Y * Y + Z * Z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CThing;
class CTCBase;
class CTCParamsBase;
namespace NHitLocations { class CHitLocation; }
class CHitParameters {                                  // 0x60 bytes in retail
public:
    unsigned char _pad00[0x1c];
    bool Blockable;                                     // +0x1c
    unsigned char _pad1d[0x26 - 0x1d];
    bool Blocked;                                       // +0x26
    bool HitNegated;                                    // +0x27
    unsigned char _pad28[0x60 - 0x28];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);   // 0x00789238
    ~CHitParameters();                                  // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0
    void SetThingHitting(CThing* thing);                // 0x00868CE0
};
struct CPhysicsBody {
    virtual void p000(); virtual void p001(); virtual void p002(); virtual void p003(); virtual void p004(); virtual void p005();
    virtual void p006(); virtual void p007(); virtual void p008(); virtual void p009(); virtual void p010(); virtual void p011();
    virtual void p012(); virtual void p013(); virtual void p014(); virtual void p015(); virtual void p016(); virtual void p017();
    virtual void p018(); virtual void p019(); virtual void p020(); virtual void p021(); virtual void p022(); virtual void p023();
    virtual void p024(); virtual void p025(); virtual void p026(); virtual void p027(); virtual void p028(); virtual void p029();
    virtual void p030(); virtual void p031(); virtual void p032(); virtual void p033(); virtual void p034(); virtual void p035();
    virtual void p036(); virtual void p037(); virtual void p038(); virtual void p039(); virtual void p040(); virtual void p041();
    virtual void p042(); virtual void p043(); virtual void p044(); virtual void p045(); virtual void p046(); virtual void p047();
    virtual void p048(); virtual void p049(); virtual void p050(); virtual void p051(); virtual void p052(); virtual void p053();
    virtual void p054(); virtual void p055(); virtual void p056(); virtual void p057(); virtual void p058(); virtual void p059();
    virtual void p060(); virtual void p061(); virtual void p062(); virtual void p063(); virtual void p064(); virtual void p065();
    virtual void p066(); virtual void p067(); virtual void p068(); virtual void p069();
    virtual C3DVector GetFacing();                             // slot 70 (+0x118)
};
struct CAction {
    virtual void a000(); virtual void a001(); virtual void a002(); virtual void a003(); virtual void a004(); virtual void a005();
    virtual void a006(); virtual void a007(); virtual void a008(); virtual void a009(); virtual void a010(); virtual void a011();
    virtual void a012(); virtual void a013(); virtual void a014();
    virtual bool IsCombatAction();                             // slot 15 (+0x3C)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
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
class CThing {
public:
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017();
    virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035();
    virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041();
    virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053();
    virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059();
    virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void OnHit(CHitParameters& hit);                   // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20 bitset
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                          // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x100 - 0x70];
    CAction* CurrentAction;                             // +0x100
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CThingGameObject;
struct CTCSolidToThing {
    void AddThingNotSolidTo(CThing* thing);             // 0x00770FA0
};
struct CIteratorTag { CIteratorTag() {} };
CThingGameObject** __fastcall FindCollided(CThingGameObject** first, CThingGameObject** last, CThingGameObject* const& value, const CIteratorTag& tag);   // 0x007C8130
struct CCollidedList {                                  // retail std::vector<CThingGameObject*>: 12 bytes
    CThingGameObject** _M_start;
    CThingGameObject** _M_finish;
    CThingGameObject** _M_end_of_storage;
    CThingGameObject** begin() { return _M_start; }
    CThingGameObject** end() { return _M_finish; }
    void push_back(CThingGameObject* const& value);     // 0x0078915C
};
void GrowCollided(CCollidedList* list, CThingGameObject* const& value);
#pragma auto_inline(off)
__declspec(noinline) void CCollidedList::push_back(CThingGameObject* const& value)
{
    if (_M_finish != _M_end_of_storage) {
        *_M_finish = value;
        ++_M_finish;
    } else
        GrowCollided(this, value);
}
#pragma auto_inline(on)
inline CThingGameObject** Find(CThingGameObject** first, CThingGameObject** last, CThingGameObject* const& value)
{
    return FindCollided(first, last, value, CIteratorTag());
}
class CTCHitOnCollision {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CCollidedList CollidedList;                         // +0x0c
    bool DontHitWhenCombatAction;                       // +0x18
    bool Blockable;                                     // +0x19
    bool PassThroughIfBlocked;                          // +0x1a
    bool PushSideways;                                  // +0x1b
    float Damage;                                       // +0x1c
    void OnCollision(const C3DVector& point, const C3DVector& normal, CThing& thing);
};

void CTCHitOnCollision::OnCollision(const C3DVector& point, const C3DVector& normal, CThing& thing)
{
    if (!(thing.ThingTypeFlags & 8))
        return;
    if (Find(CollidedList.begin(), CollidedList.end(), (CThingGameObject*)&thing) != CollidedList.end())
        return;
    if (DontHitWhenCombatAction && thing.CurrentAction && thing.CurrentAction->IsCombatAction())
        return;
    C3DVector direction = normal;
    C3DVector position = point;
    if (PushSideways) {
        C3DVector facing = ParentThing->PhysicsBody->GetFacing();
        facing.RotateAboutZ(-0.25f);
        C3DVector toThing = thing.GetPosition() - ParentThing->GetPosition();
        toThing.Normalise();
        direction = facing;
        if (Dot(facing, toThing) > 0.0f)
            direction.RotateAboutZ(0.5f);
        position = direction + thing.GetPosition();
    }
    CHitParameters hit(0, ParentThing, 0, position, direction, true, false, false, false);
    hit.AddDamageWithAttribute(0, Damage);
    hit.Blockable = Blockable;
    hit.SetThingHitting(ParentThing);
    thing.OnHit(hit);
    if (PassThroughIfBlocked || (!hit.Blocked && !hit.HitNegated)) {
        CollidedList.push_back((CThingGameObject*)&thing);
        if (!ParentThing->HasTC(0x4d)) {
            CCharString name("CTCSolidToThing", -1);
            ParentThing->AddTC(name, false, 0);
        }
        CTCSolidToThing* solid;
        if (ParentThing->GetInterface(&solid, 0x4d))
            solid->AddThingNotSolidTo(&thing);
    }
}
