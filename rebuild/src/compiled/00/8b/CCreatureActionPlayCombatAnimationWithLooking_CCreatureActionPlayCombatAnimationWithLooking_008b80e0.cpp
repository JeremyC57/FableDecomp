// CCreatureAction_PlayCombatAnimationWithLooking::CCreatureAction_PlayCombatAnimationWithLooking(CThingCreatureBase& creature, const CCharString& animation, bool flag) -- forward the three arguments to the CCreatureAction_PlayCombatAnimation constructor, then store this class's vtable
// Leaf: ??0CCreatureAction_PlayCombatAnimationWithLooking@@QAE@AAVCThingCreatureBase@@ABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008b80e0, 37 bytes, thiscall, `ret 0Ch`.
// Identity (proven): it stores vtable 0x0127A74C (RTTI .?AVCCreatureAction_PlayCombatAnimationWithLooking@@, base CCreatureAction_PlayCombatAnimation) after the base constructor 0x008B7FF0 (bound ??0CCreatureAction_PlayCombatAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@_N@Z); Ego_r.pdb ??0CCreatureAction_PlayCombatAnimationWithLooking@@QAE@AAVCThingCreatureBase@@ABVCCharString@@_N@Z; caller CActionPlayCombatAnimation::Update 0x00712A80.
// Callees: the base constructor 0x008B7FF0; vtable 0x0127A74C.
class CThingCreatureBase;
class CCharString;
class CCreatureAction_PlayCombatAnimation {
public:
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase& creature, const CCharString& animation, bool flag);   // 0x008B7FF0
    virtual ~CCreatureAction_PlayCombatAnimation();
};
class CCreatureAction_PlayCombatAnimationWithLooking : public CCreatureAction_PlayCombatAnimation {
public:
    CCreatureAction_PlayCombatAnimationWithLooking(CThingCreatureBase& creature, const CCharString& animation, bool flag);
    virtual ~CCreatureAction_PlayCombatAnimationWithLooking();
};
CCreatureAction_PlayCombatAnimationWithLooking::CCreatureAction_PlayCombatAnimationWithLooking(CThingCreatureBase& creature, const CCharString& animation, bool flag)
    : CCreatureAction_PlayCombatAnimation(creature, animation, flag)
{
}