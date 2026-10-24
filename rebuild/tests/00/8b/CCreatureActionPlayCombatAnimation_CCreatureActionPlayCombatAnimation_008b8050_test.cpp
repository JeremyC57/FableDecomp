#include <cstdio>
#include <cstdlib>
#include <new>
// Model of CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(owner, animation, usePhysics)
// @ 0x008B8050: base CCombatActionBase(&owner, 50, CSoundPair()), the three flags, then the animation pushed onto
// CombatAnimations.
struct CThingCreatureBase { int Id; };
struct CAnimationEntry { int Id; };
struct CSoundPair {
    int FirstSound;
    int LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
template <class _Tp>
class Vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    Vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, unsigned int __fill_len)
    {
        unsigned int old = (unsigned int)(__position - _M_start);
        unsigned int len = old + (old > __fill_len ? old : __fill_len);
        _Tp* fresh = (_Tp*)malloc(len * sizeof(_Tp));
        for (unsigned int i = 0; i < old; ++i) fresh[i] = _M_start[i];
        for (unsigned int j = 0; j < __fill_len; ++j) fresh[old + j] = __x;
        free(_M_start);
        _M_start = fresh;
        _M_finish = fresh + old + __fill_len;
        _M_end_of_storage = fresh + len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, 1U);
    }
};
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase* owner, int actionCode, const CSoundPair& sound)   // 0x00858030
        : Owner(owner), ActionCode(actionCode), FirstSound(sound.FirstSound), LastSound(sound.LastSound) { }
    virtual ~CCombatActionBase() { free(CombatAnimations._M_start); }
    CThingCreatureBase* Owner;
    int ActionCode;
    int FirstSound;
    int LastSound;
    Vector<const CAnimationEntry*> CombatAnimations;
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase {
public:
    bool FaceTarget;
    bool UsePhysics;
    bool AlwaysUseMovementInAnim;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase& owner, const CAnimationEntry& animation, bool usePhysics);
};
CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase& owner, const CAnimationEntry& animation, bool usePhysics)
    : CCombatActionBase(&owner, 50, CSoundPair())
    , FaceTarget(false)
    , UsePhysics(usePhysics)
    , AlwaysUseMovementInAnim(false)
{
    CombatAnimations.push_back(&animation);
}
int main()
{
    bool ok = true;
    CThingCreatureBase owner = { 1 };
    CAnimationEntry anim = { 2 };
    for (int physics = 0; physics < 2; ++physics) {
        CCreatureAction_PlayCombatAnimation action(owner, anim, physics != 0);
        if (action.Owner != &owner || action.ActionCode != 50 || action.FirstSound != 0 || action.LastSound != 0) ok = false;
        if (action.FaceTarget || action.UsePhysics != (physics != 0) || action.AlwaysUseMovementInAnim) ok = false;
        if (action.CombatAnimations._M_finish - action.CombatAnimations._M_start != 1) ok = false;
        if (action.CombatAnimations._M_start[0] != &anim) ok = false;
    }
    if (!ok) { std::printf("008b8050_TEST FAIL\n"); return 1; }
    std::printf("008b8050_TEST PASS\n");
    return 0;
}