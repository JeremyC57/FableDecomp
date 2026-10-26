// CAIStateGroup_IceTrollPoundGroundAttack::Update() -- with a live target: mark the target's stealth component in danger, rise up if knocked down, then with the target still alive play LOOK_BACKWARD when it is more than 60 degrees off the creature's XY facing (LOOK_FORWARD otherwise) and, still runnable, perform the pound attack; terminate at the end or on any failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009624D0, length 738 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020, CTCStealth::SetAsInDanger
// @ 0x00747B80, CTCCreatureModeManager::HasMode @ 0x0073A8A0, CCreatureAction_RiseUp ctor @ 0x008BD880, ~CCombatActionBase
// @ 0x0062E900, AIAction_CombatPerformActionAndWait @ 0x0097DA40, CThing::GetPosition @ 0x004C73D0, C3DVector::Normalise
// @ 0x00A14440, the physics body's GetFacing (virtual slot 70 on CThing+0x60), CCharString ctor @ 0x0099EBF0 / dtor
// @ 0x0099EAE0, CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0, AIAction_PerformActionAndWait @ 0x0097DB70,
// IsStillRunnable @ 0x00961C50, action ctor @ 0x0094E5F0 (manifest CCombatAction_StabThing), Terminate @ 0x008FCF40.
// Shape: the target position is a frame copy (user copy ctor; its dead Z copy drops out); the direction takes the full
// 3D difference and then zeroes Z, which fixes the x87 operand order of the Z product in the dot; the facing is the
// physics body's GetFacing with Z cleared; both are normalised only above the 1e-4 epsilon squared.
// Layout: Ego_r.pdb CAIStateGroup_IceTrollPoundGroundAttack PTarget +0x14.
#include <math.h>
#pragma intrinsic(cos)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& AccurateNormalise();                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
extern float g_epsilon_0129ba3c;                // 0x0129BA3C (1e-4f)
struct CPhysicsBody {                           // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
#undef VF10
#undef VF
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
class CThing;
class CThingCreatureBase;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
    bool IsPerformingAnyAction();               // 0x006622A0
};
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
class CTCCreatureModeManager { public:                 // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                   // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;                        // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_IceTrollIcicleAttack : public CCombatActionBase { public:   // vtable 0x012960C4
    virtual void v00();
    CCreatureAction_IceTrollIcicleAttack(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 0x96, CSoundPair()) { }
};
class CCombatAction_StabThing : public CCombatActionBase { public:   // manifest label (propagated?) for 0x0094E5F0
    unsigned char _stab[4];
    CCombatAction_StabThing(CThingCreatureBase* owner, CThing* target);   // 0x0094E5F0
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:
    long m_riseState;
    CCreatureAction_RiseUp(CThingCreatureBase* creature);   // 0x008BD880
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    long m_loop;                                // retail sizeof 0x118 (frame)
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
inline void SetPerformAction(CBrainOutput& output, CCombatActionBase* action)
{
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
}
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_IceTrollPoundGroundAttack : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    bool IsStillRunnable();                     // 0x00961C50
    void Update();
};

void CAIStateGroup_IceTrollPoundGroundAttack::Update()
{
    if (IS_THING_ALIVE(PTarget)) {
        CTCStealth* stealth;
        if (PTarget.GetPItem()->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(GetCreature(), 3);
        CTCCreatureModeManager* modes;
        if (GetCreature()->GetInterface(&modes, 0x31) && modes->HasMode(0x1e)) {
            long result = AIAction_CombatPerformActionAndWait(&CCreatureAction_RiseUp(GetCreature()), true);
            if (result == 1) {
                Terminate();
                return;
            }
        }
        if (IS_THING_ALIVE(PTarget)) {
            C3DVector targetPos = PTarget.GetPItem()->GetPosition();
            const C3DVector& myPos = GetCreature()->GetPosition();
            C3DVector toTarget(targetPos.X - myPos.X, targetPos.Y - myPos.Y, targetPos.Z - myPos.Z);
            toTarget.Z = 0.0f;
            if (toTarget.MagnitudeSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
                toTarget.AccurateNormalise();
            C3DVector facing = GetCreature()->PhysicsBody->GetFacing();
            facing.Z = 0.0f;
            if (facing.MagnitudeSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
                facing.AccurateNormalise();
            if (Dot(facing, toTarget) < cos(1.0471976115464443)) {
                long result = AIAction_PerformActionAndWait(
                    &CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_BACKWARD", true), false);
                if (result == 1) {
                    Terminate();
                    return;
                }
            } else {
                long result = AIAction_PerformActionAndWait(
                    &CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_FORWARD", true), false);
                if (result == 1) {
                    Terminate();
                    return;
                }
            }
            if (IsStillRunnable()) {
                CThing* target = PTarget.GetPItem();
                long result = AIAction_CombatPerformActionAndWait(&CCombatAction_StabThing(GetCreature(), target), true);
                if (result == 1) {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}