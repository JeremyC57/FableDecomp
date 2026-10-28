// CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge(CThingCreatureBase*, CThing*, float) -- base CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING")), name set through the setter, then ChargeDistance and StartPos = owner position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4EA0, length 163 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (__fastcall, hidden
// CSoundPair result), CCombatActionBase::CCombatActionBase @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0,
// CCombatActionBase::SetCombatAnimation @ 0x008574D0, CThing::GetPos @ 0x004C73D0.
// Shape: the ScorpionChargeStrike family constructor (landed 0x008C4F50) plus two body assignments after the
// name setter: the float argument copied as an integer move to +0x114, then the owner's position copied through
// `lea ecx,[esi+118h]` (C3DVector with a user copy constructor and the implicit assignment). `ret 0Ch`: three
// arguments. Vtable 0x01281024.
// Layout: engine/CCreatureAction_ScorpionCharge.h places ChargeDistance at +0x12c / StartPos at +0x130 (PDB
// CCombatActionBase is 0x12c bytes, retail 0x114), so `this` is modelled locally 24 bytes lower.

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CThing { public:   // the creature derives from CThing at offset 0 (mov ecx,edi)
    const C3DVector& GetPos() const;   // 0x004C73D0
};

class CThingCreatureBase : public CThing { public: };

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_ScorpionCharge : public CCombatActionBase { public:   // vtable 0x01281024
    float     ChargeDistance;               // +0x114 (PDB +0x12c)
    C3DVector StartPos;                     // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_ScorpionCharge(CThingCreatureBase* owner, CThing* target, float chargeDistance);
};

CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge(CThingCreatureBase* owner, CThing* target, float chargeDistance)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    {
        CCharString name("CHARGE_LOOP", -1);
        SetCombatAnimation(name);
    }
    ChargeDistance = chargeDistance;
    StartPos = ((CThing*)owner)->GetPos();
}