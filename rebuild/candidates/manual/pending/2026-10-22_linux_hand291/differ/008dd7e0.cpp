// Grade: DIFFER(744v744) -- one store slot: the box MinX store (mov [esp+34h],ecx) lands after add ebp,eax; retail has it right after mov edx,esi. Corner objects + inline GetParentThing (advisor C1) closed the rest
// NCreatureMode::CModeThunderLightningStorm::UpdateDamage() -- probe
// Leaf: ?UpdateDamage@CModeThunderLightningStorm
class CTCBase;
class CTCParamsBase;
class CThing;
namespace NHitLocations { class CHitLocation; }

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    unsigned char _pad23[0x60 - 0x23];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0
};

class CEventBase {
public:
    virtual ~CEventBase();
};
namespace NEntityEvents {
class CEventHitBySpecialAbility : public CEventBase {   // 0x18 bytes
public:
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
class CTCEntityEvents {                             // interface 0x42
public:
    void AddEvent(CEventBase* event);               // 0x008113C0
};
class CTCElectrocutedEffect {
public:
    void Reset();                                   // 0x007CE7A0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

static __forceinline bool DecRefToZero(long& count);
struct CDefBase {
    virtual ~CDefBase();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { T* p = m_p; if (p) p->Release(); }
    T* operator->() const { return m_p; }
};
struct CThunderBattleDef : CDefBase {
    unsigned char _pad08[0x44 - 0x08];
    float Damage;                                   // +0x44
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x008E3240
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

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
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                          // +0x91 (bit 0: dead)
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPosition() const;           // 0x004C73D0
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};
struct random_access_iterator_tag { };
CIntelligentPointerThing* __fastcall FindIntelligentPointer(CIntelligentPointerThing* first, CIntelligentPointerThing* last,
                                                            const CIntelligentPointerThing& value,
                                                            const random_access_iterator_tag& tag);   // 0x006C4060
inline CIntelligentPointerThing* Find(CIntelligentPointerThing* first, CIntelligentPointerThing* last, const CIntelligentPointerThing& value)
{
    random_access_iterator_tag tag;
    return FindIntelligentPointer(first, last, value, tag);
}
inline bool IsNotIn(CIntelligentPointerThing* first, CIntelligentPointerThing* last, const CIntelligentPointerThing& value)
{
    return Find(first, last, value) == last;
}
struct CIntelligentPointerVector {                  // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    void push_back(const CIntelligentPointerThing& x);   // 0x004AD520
};
extern "C" void __cdecl free(void* p);
struct CThingVector {                               // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
};

struct CThingFilter_IsNotThing {
    const CThing* Thing;
    CThingFilter_IsNotThing(const CThing* thing) : Thing(thing) {}
};
struct CThingFilter_InSphere {
    C3DVector Centre;
    float     Radius;
    CThingFilter_InSphere(const C3DVector& centre, float radius) : Centre(centre), Radius(radius) {}
};
template <class A, class B> struct CFilter_Bind2 {
    const A* First;
    const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
typedef CFilter_Bind2<CThingFilter_IsNotThing, CThingFilter_InSphere> CStormFilter;
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CStormFilter& filter, CThingVector& out) const;   // 0x008E3C50
};
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;            // +0x20
};
inline C2DBoxI BoxAround(const C3DVector& pos, float radius)
{
    long x = (long)pos.X;
    long y = (long)pos.Y;
    long r = (long)radius;
    return C2DBoxI(x - r, y - r, x + r, y + r);
}

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x14];
    CWorld* GetWorld() const;                       // 0x0073A680
    __forceinline CThing* GetParentThing() const { return ParentThing; }
};
namespace NCreatureMode {
struct CModeThunderLightningStorm : CModeBase {
    unsigned char             _pad1c[0xa8 - 0x1c];
    CIntelligentPointerVector HitThings;            // +0xa8
    unsigned char             _padb4[0xc0 - 0xb4];
    float                     DamageRadius;         // +0xc0
    void UpdateDamage();
};

void CModeThunderLightningStorm::UpdateDamage()
{
    CDefPointer<CThunderBattleDef> def;
    ParentThing->PDef->GetSubDef(def);
    CThingVector things;
    {
        CThingFilter_InSphere sphere(ParentThing->GetPosition(), DamageRadius);
        CThingFilter_IsNotThing notMe(ParentThing);
        CStormFilter filter(notMe, sphere);
        const C3DVector& pos = ParentThing->GetPosition();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        long r = (long)DamageRadius;
        C2DCoordI lower(centre.X - r, centre.Y - r);
        C2DCoordI upper(centre.X + r, centre.Y + r);
        C2DBoxI area(lower.X, lower.Y, upper.X, upper.Y);
        CWorld* world = GetWorld();
        world->ThingSearchTools->GetAllThingsInRoughArea(area, filter, things);
    }
    for (CThing** it = things._M_start; it != things._M_finish; ++it) {
        CThing* thing = *it;
        if (thing && !(thing->Flags91 & 1)) {
            if (IsNotIn(HitThings._M_start, HitThings._M_finish, CIntelligentPointerThing(thing))) {
                if (!thing->HasTC(0x8a)) {
                    CTCBase* effect;
                    {
                        CCharString name("CTCElectrocutedEffect");
                        effect = thing->AddTC(name, true, 0);
                    }
                    ((CTCElectrocutedEffect*)effect)->Reset();
                }
                const C3DVector& myPos = ParentThing->GetPosition();
                const C3DVector& thingPos = thing->GetPosition();
                CHitParameters hit(0, GetParentThing(), 0, thing->GetPosition(),
                                   C3DVector(thingPos.X - myPos.X, thingPos.Y - myPos.Y, thingPos.Z - myPos.Z),
                                   true, false, false, false);
                hit.AddDamageWithAttribute(2, def->Damage);
                hit.MoveBack = false;
                thing->OnHit(hit);
                if (!(thing->Flags91 & 1) && thing->HasTC(0x42)) {
                    CTCEntityEvents* events = thing->GetTC<CTCEntityEvents>(0x42);
                    events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 0xb));
                }
                HitThings.push_back(CIntelligentPointerThing(thing));
            }
        }
    }
}
}
