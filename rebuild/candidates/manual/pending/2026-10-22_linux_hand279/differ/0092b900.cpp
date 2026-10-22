// Grade: DIFFER(824v827) -- control flow, calls and the null-physics tail match; Normalise squared-length term order (retail z*z + x*x + y*y), the facing dot term order (retail keeps &Forward in eax: Y, Z, then X) and the atan2 arguments read through edi not esi; frame 8 bytes smaller
// CAIStateGroup_OnHeroPush::Update() -- take the push direction from the creature's 0xC4 push component, normalise it, face along it (pushed from behind) or against it (from the front), play PUSHED_FORWARD / PUSHED_BACKWARD on the first push, post the hero's 0x49 opinion deed against the creature, remove the 0xC4 component and Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092B900, length 827 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (GetInterface 0xC4, bit 4 of +0x38) and @ 0x0040F020 (the hero's 0x51 =
// CTCHeroOpinionDeedLog, bit 17 of +0x28), the physics body's virtual slots 72 (+0x120, right-handed set) and 66 (+0x108,
// SetFacingAngle), _CIfmod @ 0x00BFEB28, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPCharacter @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0,
// CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990, CThing::RemoveTC @ 0x004C9840, Terminate @ 0x008FCF40.
// Data: the reciprocal-square-root mantissa table @ 0x013CE558; 3.0f / 0.5f / 0.0f / 1.0f @ 0x0122DED4 / 0x0122F59C / 0x0122DEDC /
// 0x0122DED8, 1/(2 pi) @ 0x01237F08, 1.0 (double) @ 0x01237F00, "PUSHED_FORWARD" @ 0x01291BDC, "PUSHED_BACKWARD" @ 0x01291BCC,
// g_pMainGameComponent @ 0x013B86A0 (+0x1c player manager).
// Layout: local model (PBrain +4; brain PCreature +0x18; PushDirection +0x14; push component: direction +0x10, push count +0x1c;
// retail CThing physics body +0x60; the frame fits a 0xa8-byte CCreatureAction_PlayAnimation).
// Shape: the facing fraction is the landed 0x0069FCE0 atan2/fmod wrap; the two animation branches tail-merge.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z + x * x + y * y;
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
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);    // slot 66 (+0x108)
    VF(67) VF(68) VF(69) VF(70) VF(71)
    virtual const CRightHandedSet& GetRHSet();      // slot 72 (+0x120)
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PPhysics;                     // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void RemoveTC(long interfaceType);          // 0x004C9840
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingPlayerCreature : CThingCreatureBase { };
struct CTCHeroPush {                            // interface 0xC4
    unsigned char _pad[0x10];
    C3DVector Direction;                        // +0x10
    long PushCount;                             // +0x1c
};
struct CFaction;
struct CTCHeroOpinionDeedLog {                  // interface 0x51
    long PostDeed(long deed, const C3DVector& position, CThing* target, CThing* creature, long postType,
        bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CPlayer {
    CThingPlayerCreature* GetPCharacter();      // 0x00487DC0
};
struct CPlayerManager {
    CPlayer& GetMainPlayer();                   // 0x00449970
};
struct CMainGameComponent {
    unsigned char _pad[0x1c];
    CPlayerManager* PPlayerManager;             // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
static __forceinline float FacingFraction(float x, float y)
{
    float fraction = (float)atan2(x, y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}
struct CAIStateGroup_OnHeroPush : CAIStateGroupBase {
    C3DVector PushDirection;                    // +0x14
    virtual void Update();
};

void CAIStateGroup_OnHeroPush::Update()
{
    const CTCHeroPush* push;
    if (!((const CThing*)GetCreature())->GetInterface(&push, 0xc4))
        return;
    {
        PushDirection = push->Direction;
        PushDirection.Normalise();
        CThingCreatureBase* creature = GetCreature();
        const CRightHandedSet& orientation = creature->PPhysics->GetRHSet();
        C3DVector forward(orientation.Forward.X, orientation.Forward.Y, 0.0f);
        CPhysicsBody* physics = creature->PPhysics;
        if (physics) {
        bool fromBehind;
        if (Dot(forward, PushDirection) >= 0.0f) {
            physics->SetFacingAngle(FacingFraction(PushDirection.X, PushDirection.Y));
            fromBehind = true;
        } else {
            physics->SetFacingAngle(FacingFraction(-PushDirection.X, -PushDirection.Y));
            fromBehind = false;
        }
        if (push->PushCount <= 1) {
            if (fromBehind)
                AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "PUSHED_FORWARD", CSoundPair(),
                    false, true, false, 0, 0), false);
            else
                AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "PUSHED_BACKWARD", CSoundPair(),
                    false, true, false, 0, 0), false);
        }
        CThingPlayerCreature* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer().GetPCharacter();
        CTCHeroOpinionDeedLog* deeds;
        if (hero && !(hero->m_flags91 & 1) && hero->GetInterface(&deeds, 0x51))
            deeds->PostDeed(0x49, ((const CThing*)hero)->GetPosition(), 0, GetCreature(), 1, false, false, 0, false, 0);
        GetCreature()->RemoveTC(0xc4);
        Terminate();
        return;
        }
    }
    Terminate();
}
