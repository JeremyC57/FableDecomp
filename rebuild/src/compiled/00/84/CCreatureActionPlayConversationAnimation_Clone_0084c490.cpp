// CCreatureAction_PlayConversationAnimation::Clone() const -- new CCreatureAction_PlayConversationAnimation(*this): the implicit copy constructor inline (base copy, vtable, UseAnimationMovement, Duration)
// Leaf: ?Clone@CCreatureAction_PlayConversationAnimation@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084C490, 71 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x012737CC slot 11 (the Clone slot; 0x012737F8 is this function's only reference), RTTI
//   .?AVCCreatureAction_PlayConversationAnimation@@ (rtti-vtables.tsv), and the body stores that same vtable into the copy.
//   Ego_r.pdb / FableWin.pdb ?Clone@CCreatureAction_PlayConversationAnimation@@UBEPAVCCreatureActionBase@@XZ. RTTI base:
//   CCreatureAction_PlayConversationAnimation : CCreatureActionBase (directly, not CCombatActionBase), so the base copy is
//   ??0CCreatureActionBase@@QAE@ABV0@@Z 0x0062EBC0 (bound so). It allocates 0xB0 bytes = the retail sizeof
//   (engine/CCreatureAction_PlayConversationAnimation.h: Ego_r 0xBC less the CCreatureActionBase shrink 0xB4 -> 0xA8).
//   The manifest label is right; the twin 0x006F8280 (CastWillDrainLifeSpell) has a CCombatActionBase base and two longs.
// Callees: operator new 0x00BFEA1A, CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0.
// Layout: Ego_r.pdb members UseAnimationMovement (bool, retail +0xA8: `mov al` byte copy) and Duration (float, retail
//   +0xAC: copied through ecx by the implicit copy constructor). engine/CCreatureAction_PlayConversationAnimation.h and
//   engine/CCreatureActionBase.h flatten the vptr into a data member, so the polymorphic base with its out-of-line copy
//   constructor is modelled locally over the same offsets (the landed CCreatureAction_TentacleStrikeLoop::Clone model).
// Shape (family K037): `return new X(*this);` with X's implicit copy constructor inlined; null-tested allocation.
class CCreatureActionBase {                                  // retail sizeof 0xA8 (Ego_r.pdb 0xB4)
public:
    CCreatureActionBase(const CCreatureActionBase& other);  // 0x0062EBC0
    virtual ~CCreatureActionBase();
    virtual CCreatureActionBase* Clone() const = 0;          // slot 11
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_PlayConversationAnimation : public CCreatureActionBase {   // vtable 0x012737CC
public:
    virtual CCreatureActionBase* Clone() const;
    bool  UseAnimationMovement;                              // +0xA8
    float Duration;                                          // +0xAC
};

CCreatureActionBase* CCreatureAction_PlayConversationAnimation::Clone() const
{
    return new CCreatureAction_PlayConversationAnimation(*this);
}