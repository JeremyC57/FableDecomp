// CCreatureAction_KrakenLowAttackInto::OnAnimationLoaded() -- base OnAnimationLoaded, then ActionTime = three quarters of TotalTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923CD0, length 32 bytes. OnAnimationLoaded is vtable slot 68 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::OnAnimationLoaded @ 0x00859EF0 (direct base call).
// Shape: `lea eax,[eax+eax*2]` then the signed divide-by-4 sequence (`cdq ; and edx,3 ; add ; sar 2`): a long `* 3 / 4`.
// TotalTime (+0x10) and ActionTime (+0x14) are the engine-header CCreatureActionBase names.
struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void OnAnimationLoaded();       // 0x00859EF0 (retail slot 68; the slot is immaterial to this direct call)
    unsigned char _pad04[0x0c];             // +0x04
    long TotalTime;                         // +0x10
    long ActionTime;                        // +0x14
};

struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {
    virtual void OnAnimationLoaded();
};

void CCreatureAction_KrakenLowAttackInto::OnAnimationLoaded()
{
    CCombatActionBase::OnAnimationLoaded();
    ActionTime = TotalTime * 3 / 4;
}