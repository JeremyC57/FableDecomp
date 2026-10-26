// CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase& owner, const CAnimationEntry& animation, bool usePhysics) -- base CCombatActionBase(owner, 50, zeroed CSoundPair), own vtable, the three flags (UsePhysics from the argument), then the animation entry appended to CombatAnimations
// Leaf: ??0CCreatureAction_PlayCombatAnimation@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8050, length 141 bytes (+ int3 padding), thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb `??0CCreatureAction_PlayCombatAnimation@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@_N@Z`;
//   vtable 0x0127A604 (RTTI `.?AVCCreatureAction_PlayCombatAnimation@@`), as the landed sibling constructors 0x008B7F80 /
//   0x008B7FF0 (the CCharString overloads). The catalog label "CCombatActionBase_Constructor_Action0x32" is descriptive.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030 (landed),
//   vector<const CAnimationEntry*>::_M_insert_overflow @ 0x005E1420 (the folded 4-byte-element growth insert).
// Layout (retail; Ego_r.pdb names): CCombatActionBase is 0x114 bytes with CombatAnimations (STLport vector<const
//   CAnimationEntry*>, 12 bytes) at +0xF0; FaceTarget +0x114, UsePhysics +0x115, AlwaysUseMovementInAnim +0x116.
// Shape: the sound pair zeroed through ebx and passed by reference; UsePhysics stored before the vtable, the two zero
//   flags after it (the sibling constructors' order); `CombatAnimations.push_back(&animation)` inline: the address is an
//   rvalue, so VC7.1 copies it into the dead usePhysics argument slot, and the overflow call homes the empty __true_type
//   tag on that same live slot -- only with a same-TU auto_inline(off) stand-in body for _M_insert_overflow (reads x
//   through its reference, never touches the tag).
inline void* operator new(unsigned int, void* where) { return where; }

class CThingCreatureBase;
class CAnimationEntry;

class CSoundPair { public:                                     // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

namespace std {
struct __true_type { __true_type() { } };

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in for the growth member 0x005E1420 (reads x through its reference only, never the tag).
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    if (*(void* const*)&__x != (void*)__position)
        return;
    _M_finish = __position + __fill_len + __atend;
}
#pragma auto_inline(on)
}

class CCombatActionBase {                               // retail sizeof 0x114
public:
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    unsigned char _body04[0xF0 - 4];
    std::vector<const CAnimationEntry*> CombatAnimations; // +0xF0
    unsigned char _bodyFC[0x114 - 0xFC];
};

class CCreatureAction_PlayCombatAnimation : public CCombatActionBase {   // vtable 0x0127A604
public:
    bool FaceTarget;                                    // +0x114
    bool UsePhysics;                                    // +0x115
    bool AlwaysUseMovementInAnim;                       // +0x116
    virtual void v00();
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