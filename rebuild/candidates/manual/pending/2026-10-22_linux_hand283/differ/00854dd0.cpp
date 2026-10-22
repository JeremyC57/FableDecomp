// CCreatureAction_SpiritAttack::PerformAction() -- take the target from the creature's interface-8 TC and remember it;
// Grade: DIFFER(702v720) -- retail keeps the direction X/Y on the x87 stack through the fast normalise (only Z spilled) and holds this in edi / &PCreature in ebp; ours stores the subtraction result and swaps those two registers (ctor-based operator- evaluates Z first)
//   when it is alive and hittable (+0x6c bit 3) and within the co-op spirit's range plus its own physics radius, create
//   the attack effect and hit it from just behind the spirit along the spirit-to-target direction (one unit of
//   attribute-0 damage), granting one experience point unless the hit was blocked or deflected; always finish with the
//   spirit TC's hit handler.
// Leaf: ?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854DD0, length 720 bytes, plain `ret` (virtual).
// Callees: GetPItem 0x00A01B50, LowerBound 0x0040F020, interface-8 TC slot 33 (+0x84) target, CIntelligentPointer
//   ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C), CTCCoopSpirit range 0x00670000 /
//   AddExperience 0x0066FB10 / OnHit 0x006707E0, CThingPhysical::IsInRange 0x006A4DF0, CThing::GetPosition
//   0x004C73D0, CreateAttackEffect 0x008548B0, CHitParameters ctor 0x00789238 / AddDamageWithAttribute 0x00868CA0 /
//   dtor 0x00789358, thing slot 65 (+0x104) hit.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout (retail bytes): action +0x08 PCreature, +0xa8 PTarget; thing +0x20 present bits, +0x44 interface map,
//   +0x60 physics (+0x34 radius), +0x6c flags, +0x91 bit 0 dead; CHitParameters 0x60 bytes (frame), +0x26/+0x27 the
//   blocked/deflected flags.
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + Y * Y + X * X); }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CTCPhysics {
    unsigned char _pad00[0x34];
    float         Radius;                                    // +0x34
};
class CThing;
struct CHitParameters {
    unsigned char _pad00[0x26];
    bool          Blocked;                                   // +0x26
    bool          Deflected;                                 // +0x27
    unsigned char _pad28[0x60 - 0x28];
    CHitParameters(void* location, CThing* attacker, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool a, bool b, bool c, bool d);   // 0x00789238
    ~CHitParameters();                                       // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CThing {
public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void Hit(CHitParameters& hit);                   // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysics*   PPhysics;                                  // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char m_flags6c;                                 // +0x6c bit 3: can be hit
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
    const C3DVector* GetPosition() const;                    // 0x004C73D0
    bool IsInRange(const C3DVector* position, float range);  // 0x006A4DF0
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
struct CTCTargeting {
    FILL10(0) FILL10(1) FILL10(2)
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                             // slot 33 (+0x84)
};
struct CTCCoopSpirit {
    float GetRange();                                        // 0x00670000
    void AddExperience(long amount);                         // 0x0066FB10
    void OnHit();                                            // 0x006707E0
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
struct CCreatureAction_SpiritAttack {
    virtual void v00();
    unsigned char            _pad04[4];
    CBaseIntelligentPointer  PCreature;                      // +0x08
    unsigned char            _pad10[0xa8 - 0x10];
    CIntelligentPointerThing PTarget;                        // +0xa8
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    void CreateAttackEffect();                               // 0x008548B0
    virtual void PerformAction();
};

void CCreatureAction_SpiritAttack::PerformAction()
{
    CTCTargeting* targeting;
    GetCreature()->GetInterface(&targeting, 8);
    CThing* target = targeting->GetTarget();
    PTarget = CIntelligentPointerThing(target);
    if (target && !(target->m_flags91 & 1) && (target->m_flags6c & 8)) {
        CTCCoopSpirit* spirit;
        GetCreature()->GetInterface(&spirit, 0xc7);
        float range = spirit->GetRange();
        if (target->IsInRange(GetCreature()->GetPosition(), range + target->PPhysics->Radius)) {
            CreateAttackEffect();
            C3DVector direction = *target->GetPosition() - *GetCreature()->GetPosition();
            direction.Normalise();
            CHitParameters hit(0, GetCreature(), 0, *GetCreature()->GetPosition() - direction, direction,
                               false, true, false, false);
            hit.AddDamageWithAttribute(0, 1.0f);
            target->Hit(hit);
            if (!hit.Blocked && !hit.Deflected)
                spirit->AddExperience(1);
        }
    }
    CTCCoopSpirit* spirit;
    GetCreature()->GetInterface(&spirit, 0xc7);
    spirit->OnHit();
}
