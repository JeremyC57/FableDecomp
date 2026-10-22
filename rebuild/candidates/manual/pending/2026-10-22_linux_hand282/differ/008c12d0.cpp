// CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise() -- Jack rushes only on the hit that reaches the battle def's NumGetHitsBeforeRush (else a plain strike response); then, facing away from the hit (or the living hitter), play the front or back rush animation and turn to face along it
// Grade: DIFFER(809v808) -- register allocation: retail keeps &PCreature in ebp, edi = map/boss/def/&PThingThatHit/position/physics, ebx = 0/hitter, and reloads the def from its slot only for the final release (`mov edi,[esp+14h]`, frame 0x3c); ours puts &PCreature in edi, boss/def/hitter in ebp and map/0/&PThingThatHit in ebx, so the def is reloaded into ebp right after the hitter block and its slot is reused for the angle (frame 0x38); every instruction otherwise in retail order | tried: extern GetSubDef (850: ebp=&PCreature as retail but the def becomes address-exposed -> no xor-ebx zero, bl=1 constant, tail-duplicated early return, reload after the dec), escaping stand-ins (850/856), Release()/local-copy dtors, def declared first, PCreature reference/accessor, PThingThatHit reference, battle-def local, stand-in defined after the caller, the real GetSubDef template body (0x008C33E0 shape: CCharString name, manager lookup, operator=; inline and noinline) | next: lane gap: kind=register allocation (the in-TU non-escaping def takes ebp ahead of &PCreature)
// Leaf: ?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush
#include <math.h>
#pragma intrinsic(cos, atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558

struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
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
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CJackOfBladesBattleDef : CDefBase {
    unsigned char _pad08[0x40 - 0x08];
    long          NumGetHitsBeforeRush;                 // +0x40 (PDB +0x44)
};
template<class T> struct CDefPointer {                  // released without a null test (the def is used unconditionally)
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CJackOfBladesBattleDef>* out);   // 0x008C33E0
};
void* FindSubDef_CJackOfBladesBattleDef(CDefClassBase* defClass);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CJackOfBladesBattleDef>* out)
{
    void* def = FindSubDef_CJackOfBladesBattleDef(this);
    if (!def)
        return false;
    out->Object = (CJackOfBladesBattleDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CTCBoss {                                        // interface 0xAD
    unsigned char _pad00[0x10];
    long          GenericCounter;                       // +0x10
};
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
struct CTCPhysicsBase {
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngleXY(float angle);         // slot 66 (+0x108)
    VF(67) VF(68) VF(69)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* m_pPhysics;                         // +0x60
    unsigned char   _pad64[0x70 - 0x64];
    CDefClassBase*  m_pDef;                             // +0x70
    unsigned char   _pad74[0x91 - 0x74];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    CDefClassBase* GetDef() const { return m_pDef; }
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
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);     // 0x008574D0
};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00865F40
    CBaseIntelligentPointer PThingThatHit;              // +0x114
    C3DVector               PointOfHit;                 // +0x11c
    unsigned char           _pad128[0x148 - 0x128];
};
class CCombatAction_StrikeResponseJackOfBladesIntoRush : public CCombatAction_GenericStrikeResponseBase {
public:
    bool CauseRush;                                     // +0x148 (PDB +0x160)
    virtual void Initialise();
};

void CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise()
{
    CTCBoss* boss;
    PCreature.GetPItem()->GetInterface(&boss, 0xad);
    CDefPointer<CJackOfBladesBattleDef> def;
    PCreature.GetPItem()->GetDef()->GetSubDef(&def);
    long hits = boss->GenericCounter + 1;
    if (hits > def->NumGetHitsBeforeRush)
        CauseRush = false;
    else if (hits == def->NumGetHitsBeforeRush)
        CauseRush = true;
    else
        CauseRush = false;
    if (!CauseRush) {
        CCombatAction_GenericStrikeResponseBase::Initialise();
        return;
    }
    C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
    C3DVector direction = PointOfHit - PCreature.GetPItem()->GetPosition();
    if (IS_THING_ALIVE(PThingThatHit)) {
        CThing* hitter = PThingThatHit.GetPItem();
        direction = hitter->GetPosition() - PCreature.GetPItem()->GetPosition();
    }
    C3DVector away(0.0f, 0.0f, 0.0f);
    direction.Normalise();
    if (Dot(facing, direction) < (float)cos(1.745329383786327)) {
        away = -direction;
        CCharString name("RESPONSE_GENERIC_HIT_BACK_RUSH");
        SetAnimationName(name);
    } else {
        CCharString name("RESPONSE_GENERIC_HIT_FRONT_RUSH");
        SetAnimationName(name);
    }
    CTCPhysicsBase* physics = PCreature.GetPItem()->GetPhysics();
    physics->SetFacingAngleXY(WrapAngle((float)atan2(away.X, away.Y) * 0.15915494f));
    CCombatActionBase::Initialise();
}
