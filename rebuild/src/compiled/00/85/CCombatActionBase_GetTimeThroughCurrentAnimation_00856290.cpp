// CCombatActionBase::GetTimeThroughCurrentAnimation() const -- seconds the action has spent in its current animation: the RunningTimeSeconds less the animation start delay (frames over the frames-per-second), never negative
// Leaf: ?GetTimeThroughCurrentAnimation@CCombatActionBase@@IBEMXZ
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856290, 60 bytes, __fastcall, plain `ret`, float in st0. No callers in retail (unreferenced but linked).
// Identity: TU order matches Ego_r.pdb CCombatActionBase.cpp -- InitialiseActionMovement (TLC 0x00856240), then
//   ?GetLastTimeThroughCurrentAnimation@CCombatActionBase@@IBEMXZ (0x00856250, reads +0x44) and
//   ?GetTimeThroughCurrentAnimation@CCombatActionBase@@IBEMXZ (0x00856290, reads +0x40), then IsBlockCounter (0x008562D0).
//   The manifest label "CalcCooldownRemaining" is an autoname.
// Layout: CCreatureActionBase (Ego_r.pdb, rebuild/include/engine/CCreatureActionBase.h): RunningTimeSeconds +0x40,
//   LastRunningTimeSeconds +0x44; vtable slot 6 (+0x18) = GetAnimDelayTime() const (RTTI CCombatActionBase[6]).
// Globals: CMainGameComponent::GetConstantFPS() (long) @ 0x01375550, read into a local BEFORE the virtual call (retail spills it to [esp+8]
//   for the fidiv; reading the global inside the expression loads it after the call); 0.0f @ 0x0122DEDC.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CCombatActionBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual long GetAnimDelayTime() const;               // slot 6 (+0x18)
protected:
    float GetTimeThroughCurrentAnimation() const;
    unsigned char _pad_0x04[0x3c];
    float         RunningTimeSeconds;                    // +0x40
    float         LastRunningTimeSeconds;                // +0x44
};

float CCombatActionBase::GetTimeThroughCurrentAnimation() const
{
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    float t = RunningTimeSeconds - (float)GetAnimDelayTime() / framesPerSecond;
    if (t < 0.0f)
        t = 0.0f;
    return t;
}