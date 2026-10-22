// CCreatureAction_BlockRespond::Initialise() -- pick the block-response animation from where the blow came from (hit position or the living target) relative to the creature's facing (front/back within 50 degrees, else left/right by the cross product), push the creature away from it, fall back to BLOCK_FRONT_RESPONSE facing the target, then run the block base Initialise and mark the final position preferred
// Leaf: ?Initialise@CCreatureAction_BlockRespond
// Shape: the first normalise is `direction *= FastInvSqrt(direction.GetLengthSquared())` with a plain-inline length method
//   (right-nested X*X + (Y*Y + Z*Z)): operator*=(const float&) then spills the reciprocal and consumes the cached x/y
//   (`fstp [r]; fxch st(1); fmul [r]`) as retail, where the __forceinline Normalise() keeps it on the stack (`fst [r]; fmul st(2)`);
//   that Normalise() (x/y/z locals) does reproduce the second, member normalise. The facing dot is written direction-first
//   (`direction.X * facing.X + ...`): facing-first or a Dot() helper loads direction.X first in the X term. LowerBound's
//   body in the TU (noinline) lets the GetInterface keys and out variables share frame words (frame 0x30).
// Layout: retail UseHitPos +0x124 / HitPos +0x125 / FaceTargetAtStart +0x131 (Ego_r.pdb +0x13c..+0x149), so the generated
//   engine header does not fit.
#include <math.h>
#include <stdlib.h>
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558

struct CTCBase;
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
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
    __forceinline void Normalise() { float x = X; float y = Y; float z = Z; *this *= FastInvSqrt(x * x + (y * y + z * z)); }
};

struct CCharString {
    void* m_pData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const char* text);          // 0x0099EFE0
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
    SEntry* lo = first;
    long n = last - first;
    while (n > 0) {
        long half = n >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return lo;
}
#pragma auto_inline(on)
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
struct CTCPhysicsBase {                                 // interface 2
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
    unsigned char _pad04[8];
    C3DVector     Position;                             // +0x0c
};
struct CAnimationEntry;
struct CAnimationEntryVector {                          // STLport vector<const CAnimationEntry*>
    CAnimationEntry** _M_start;
    CAnimationEntry** _M_finish;
    CAnimationEntry** _M_end_of_storage;
    CAnimationEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CAnimationEntryVector() { if (_M_start) free(_M_start); }
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
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
struct CThingCreatureBase : CThing {
    bool GetPossibleCombatAnimations(const CCharString& name, CAnimationEntryVector& out);   // 0x006668A0
};
struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;                        // +0x04
    CThingCreatureBase* GetPItem() const;               // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCombatActionBase {
public:
    virtual void Initialise();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                    // +0xa8
    unsigned char           _padb0[0xdd - 0xb0];
    bool                    PreferredFinalPosSet;       // +0xdd
    unsigned char           _padde[0x114 - 0xde];
    void SetAnimationName(const CCharString& name);     // 0x008574D0
    void SetCombatAnimations(const CAnimationEntryVector& animations);   // 0x00857DC0
};
class CCreatureAction_BlockBase : public CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00855430
    long      BlowStrength;                             // +0x114
    C3DVector MovementDirection;                        // +0x118
};
#pragma pack(push, 1)
class CCreatureAction_BlockRespond : public CCreatureAction_BlockBase {
public:
    bool      UseHitPos;                                // +0x124
    C3DVector HitPos;                                   // +0x125
    bool      FaceTargetAtStart;                        // +0x131
    virtual void Initialise();
};
#pragma pack(pop)

void CCreatureAction_BlockRespond::Initialise()
{
    PCreature.GetPItem()->GetPosition();
    CCharString animation;
    if (IS_THING_ALIVE(PTarget)) {
        C3DVector direction;
        if (UseHitPos) {
            direction = HitPos;
        } else {
            CTCPhysicsBase* targetPhysics;
            PTarget.GetPItem()->GetInterface(&targetPhysics, 2);
            direction = targetPhysics->Position;
        }
        CTCPhysicsBase* physics;
        PCreature.GetPItem()->GetInterface(&physics, 2);
        direction -= physics->Position;
        direction *= FastInvSqrt(direction.GetLengthSquared());
        C3DVector facing = physics->GetFacing();
        double limit = cos(0.8726646918931635);
        float dot = direction.X * facing.X + direction.Y * facing.Y + direction.Z * facing.Z;
        if (dot > limit)
            animation = "BLOCK_FRONT_RESPONSE";
        else if (-dot > limit)
            animation = "BLOCK_BACK_RESPONSE";
        else if (facing.X * direction.Y - direction.X * facing.Y > 0.0f)
            animation = "BLOCK_LEFT_RESPONSE";
        else
            animation = "BLOCK_RIGHT_RESPONSE";
        MovementDirection = -direction;
        MovementDirection.Normalise();
    }
    CAnimationEntryVector animations;
    if (PCreature.GetPItem()->GetPossibleCombatAnimations(animation, animations)) {
        SetCombatAnimations(animations);
    } else {
        {
            CCharString name("BLOCK_FRONT_RESPONSE");
            SetAnimationName(name);
        }
        FaceTargetAtStart = true;
    }
    CCreatureAction_BlockBase::Initialise();
    PreferredFinalPosSet = true;
}