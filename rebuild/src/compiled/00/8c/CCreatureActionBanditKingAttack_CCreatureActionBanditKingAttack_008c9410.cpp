// CCreatureAction_BanditKingAttack::CCreatureAction_BanditKingAttack(CThingCreatureBase*, CThing*, const CCharString&, bool, bool) -- base CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING")), own vtable (0x01281B0C), FaceTarget / ForceCauseRecoil from the two flags, then SetAnimationName(animName)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9410, length 119 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (`__fastcall`: ecx hidden result,
// edx owner; the name on the stack), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0.
// Shape: the sound name is an implicit conversion temporary (built first, re-addressed with `lea`); the 8-byte CSoundPair
// result lands in a frame temporary through the hidden pointer and goes by reference to the base; after the temporary
// dies the two flag bytes (init-list members, loaded with the animation-name argument before the push) sink below the
// vtable store, and the body's SetAnimationName call follows. `ret 14h`: five arguments.
// Layout: engine/CCreatureAction_BanditKingAttack.h has FaceTarget/ForceCauseRecoil at the PDB's +0x12c/+0x12d; retail
// CCombatActionBase is 0x114 bytes, so `this` is modelled locally with the flags at +0x114/+0x115.
class CThingCreatureBase;
class CThing;

class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CSoundPair { public:                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_BanditKingAttack : public CCombatActionBase { public:   // vtable 0x01281B0C
    bool FaceTarget;                        // +0x114 (PDB +0x12c)
    bool ForceCauseRecoil;                  // +0x115 (PDB +0x12d)
    virtual void v00();
    CCreatureAction_BanditKingAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName, bool faceTarget, bool forceCauseRecoil);
};

CCreatureAction_BanditKingAttack::CCreatureAction_BanditKingAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName, bool faceTarget, bool forceCauseRecoil)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING"))
    , FaceTarget(faceTarget)
    , ForceCauseRecoil(forceCauseRecoil)
{
    SetAnimationName(animName);
}