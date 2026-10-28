// CCreatureAction_ScorpionTailPlunge::CCreatureAction_ScorpionTailPlunge(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING")), own vtable (0x01286494), then SetCombatAnimation("TAIL_PLUNGE")
// Label: the manifest row says CCreatureAction_BriarRoseShotFarInto (propagated); the stored vtable 0x01286494 is CCreatureAction_ScorpionTailPlunge (rtti-vtables.tsv).
// Leaf: ??0CCreatureAction_ScorpionTailPlunge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4640, length 124 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (twice), GetSoundPair @ 0x008561A0 (`__fastcall`: ecx hidden
// result, edx owner; the name on the stack), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0 (twice), CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0.
// Shape: both names are IMPLICIT conversion temporaries (`push -1 ; push offset`, re-addressed with `lea` for the use): the
// sound name is built before the owner load and dies after the base constructor; the vtable store is scheduled between the
// second temporary's pushes and its constructor call, the animation name dies after SetCombatAnimation. `ret 8`.
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
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase { public:   // vtable 0x01286494
    virtual void v00();
    CCreatureAction_ScorpionTailPlunge(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_ScorpionTailPlunge::CCreatureAction_ScorpionTailPlunge(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    SetCombatAnimation("TAIL_PLUNGE");
}