// CCreatureAction_CastBattleCharge::Initialise() -- names the cast animation, runs the base Initialise, adds a CTCBattleCharge component to the owner and aims it at the action's target; an override sets the dash distance and turns the owner along the stored direction, an AI-state owner gets the cast level, and a live target thing is handed over
// Leaf: ?Initialise@CCreatureAction_CastBattleCharge@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CD5A0, 400 bytes.
// Callees: CCreatureActionBase::SetAnimationName 0x008574D0 ("..." literal 0x01283E30), CCombatActionBase::Initialise
//          0x00857520, CThing::AddTC 0x004C9D60 (literal 0x0123C4C0), the component's SetPTarget(CThing*, bool) 0x007A7150,
//          physics vtable slot 74 SetFacingAnglesFromRHSet (the landed CastAssassinRush::Initialise 0x008CD470 model),
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/B50/B90/C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +8, PItemRemoved +0x2c, PTarget +0xa8);
// engine/CCreatureAction_CastBattleCharge.h has Override +0x12c, Dir +0x12d, Distance +0x13c, Level +0x140,
// IsZTargetting +0x144 (PDB); retail is PDB - 0x18 (+0x114/+0x115/+0x124/+0x128/+0x12c), as in the landed CastAssassinRush.
// The component's fields (distance +0x3c, level +0x40, target pointer +0x44) are descriptive.
#include <string.h>
struct CTCParamsBase;
struct CBaseObject;
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem(other.GetPItem()); return *this; }
};
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CPhysics {
    virtual void v00();
    void SetFacingAnglesFromRHSet(const CRightHandedSet& set);
};
typedef void (CPhysics::*TSetFacing)(const CRightHandedSet&);
union CPhysicsSlot { void* Address; TSetFacing Fn; };
inline void CPhysics::SetFacingAnglesFromRHSet(const CRightHandedSet& set)
{
    (this->*((*(CPhysicsSlot**)this)[74].Fn))(set);
}
struct CTCBattleCharge {
    unsigned char _pad00[0x3c];
    float Distance;                             // +0x3c
    long  Level;                                // +0x40
    CIntelligentPointerThing PTarget;           // +0x44
    void SetPTarget(CThing* target, bool zTargetting);   // 0x007A7150
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x60 - 0x11];
    CPhysics* PPhysics;                         // +0x60
    __forceinline CPhysics* GetPhysics() const { return PPhysics; }
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CTCBattleCharge* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
};
struct CCombatActionBase {
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    virtual void Initialise();                  // 0x00857520
};
#pragma pack(push, 1)
struct CCreatureAction_CastBattleCharge : CCombatActionBase {
    bool          Override;                     // +0x114
    unsigned char Dir[12];                      // +0x115 C3DVector (packed)
    unsigned char _pad121[3];
    float         Distance;                     // +0x124
    long          Level;                        // +0x128
    bool          IsZTargetting;                // +0x12c
    void CopyDirection(C3DVector& v) const { memcpy(&v.X, Dir, 4); memcpy(&v.Y, Dir + 4, 4); memcpy(&v.Z, Dir + 8, 4); }
    virtual void Initialise();
};
#pragma pack(pop)

void CCreatureAction_CastBattleCharge::Initialise()
{
    {
        CCharString name("ST_BATTLE_CHARGE_CAST", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCBattleCharge* tc = PCreature.GetPItem()->AddTC("CTCBattleCharge", true, 0);
    CThing* aim = PTarget.GetPItem();
    tc->SetPTarget(aim, IsZTargetting);
    if (Override) {
        tc->Distance = Distance;
        CPhysics& physics = *PCreature.GetPItem()->GetPhysics();
        CRightHandedSet set;
        set.Up.X = 0; set.Up.Y = 0; set.Up.Z = 1;
        CopyDirection(set.Forward);
        physics.SetFacingAnglesFromRHSet(set);
    }
    if (PCreature.GetPItem()->m_state == 1)
        tc->Level = Level;
    CIntelligentPointerThing target = PItemRemoved;
    if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1))
        tc->PTarget = target;
}