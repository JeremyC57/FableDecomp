// CTCRockTrollShield::OnHitAtPos(CThing* attacker, const C3DVector& pos, const C3DVector& direction, long damage, bool* blocked) -- absorb a high, frontal projectile hit with the shield and spawn the hit effect
// Leaf: OnHitAtPos
// Grade: DIFFER(533v533) -- only the facing dot product pair order (retail Y*fY then Z*fZ, ours Z then Y; nine source orders tried) and the eax/edx swap it rotates at the GetSubDef call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AF0F0, length 531 bytes, `ret 14h`
// (Ego_r.pdb ?OnHitAtPos@CTCRockTrollShield@@QAEXPAVCThing@@...).
// Callees: CThing::GetPosition @ 0x004C73D0, the physics body's GetFacing (slot 70, +0x118),
// CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CSpecialEffectsDef::GetEffectFromString @ 0x00669D30,
// CCharString(const char*, long) @ 0x0099EBF0 / ~ 0x0099EAE0, CreateEffectThing (fastcall) @ 0x006E0880.
// Constants: 0.5f 0x0122F59C, 3.0f 0x0122DED4, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, the double 1.0471976 (60 degrees)
// 0x01261DC8, the inverse-square-root mantissa table 0x013CE558; string "GET_HIT_BY_ARROW".
// Layout: Ego_r.pdb CTCRockTrollShield (0x14): health +0x0c, rechargeCounter +0x10; the thing's def +0x70 holds the
// height at +0x5c, the physics body sits at +0x60; the attacker's flag word +0x2c.
// Shape: the landed 0x008C0FB0 model -- same-TU GetSubDef / GetEffectFromString stand-ins, the conditional string
// temporary in `a && b(CCharString(...))`, the def released with `--RefCount == 0`.
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void operator*=(const float& s) { X *= s; Y *= s; Z *= s; }
    __forceinline void Normalise();
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
__forceinline void C3DVector::Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
__forceinline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefVirtual {
    virtual ~CDefVirtual();                             // slot 0
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    __forceinline ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
};
struct CDefClassBase : CDefBase {
    unsigned char _pad08[0x5c - 0x08];
    float         Height;                               // +0x5c
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
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

struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
};
struct CThing {
    unsigned char  _pad00[0x2c];
    unsigned long  Flags2C;                             // +0x2c
    unsigned char  _pad30[0x60 - 0x30];
    CPhysicsBody*  PhysicsBody;                         // +0x60
    unsigned char  _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                              // +0x70
    const C3DVector& GetPosition();                     // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CTCRockTrollShield {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    long    health;                                     // +0x0c
    long    rechargeCounter;                            // +0x10
    void OnHitAtPos(CThing* attacker, const C3DVector& pos, const C3DVector& direction, long damage, bool* blocked);
};

void CTCRockTrollShield::OnHitAtPos(CThing* attacker, const C3DVector& pos, const C3DVector& direction, long damage,
                                    bool* blocked)
{
    if (health == 0)
        return;
    if (attacker == 0 || !(attacker->Flags2C & 0x800000)) {
        *blocked = true;
        return;
    }
    float height = pos.Z - ParentThing->GetPosition().Z;
    if (height / ParentThing->m_pDef->Height > 0.5f) {
        C3DVector toward(direction.X, direction.Y, 0.0f);
        toward.Normalise();
        toward *= -1.0f;
        if (Dot(toward, ParentThing->PhysicsBody->GetFacing()) > cos(1.0471976115464443)) {
            health += damage;
            CDefPointer<CSpecialEffectsDef> effects;
            long effect = -1;
            bool found;
            if (ParentThing->m_pDef->GetSubDef(effects) &&
                effects.m_p->GetEffectFromString("GET_HIT_BY_ARROW", effect))
                found = true;
            else
                found = false;
            if (found)
                CreateEffectThing(effect, &pos, false);
        }
    }
    if (health <= 0) {
        health = 5;
        rechargeCounter = 0;
    } else
        *blocked = true;
}
