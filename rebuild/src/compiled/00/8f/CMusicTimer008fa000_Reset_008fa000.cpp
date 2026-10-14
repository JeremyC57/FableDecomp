// CMusicTimer_008fa000::Reset() -- restart the music timer: unit rate, cleared accumulators, the current tick
// count as the start time, then two checks of the trigger, returning `this` for chaining.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA000, length 62 bytes (+ int3 padding).
// Callees: the imported tick counter through the IAT slot @ 0x0143FDFC (`call dword ptr [..]`, modelled as a
// global function pointer like the landed CTCHeroExperience body), then the timer's check-and-trigger member
// @ 0x008F9CA0 twice (manifest "MusicTimer_CheckAndTrigger"; it compares the float at +0x108 against a
// constant). The manifest's "CAIStateGroup_MinionHasInformation::OnActivate" is a propagated label (the body
// touches +0x100..+0x10c and returns `this`); the class/leaf are descriptive.
// Shape: stores at +0x108 (1.0f), +0x10c (0), the tick call, +0x100 (ticks), +0x104 (0) in statement order,
// then the two member calls and `mov eax,esi`.
extern long (*g_pfnGetTickCount)();         // IAT slot 0x0143FDFC

struct CMusicTimer_008fa000 {
    unsigned char _pad[0x100];
    long m_startTicks;                      // +0x100
    long m_elapsed;                         // +0x104
    float m_rate;                           // +0x108
    long m_triggered;                       // +0x10c
    void CheckAndTrigger();                 // 0x008F9CA0
    CMusicTimer_008fa000* Reset();
};

CMusicTimer_008fa000* CMusicTimer_008fa000::Reset()
{
    m_rate = 1.0f;
    m_triggered = 0;
    m_startTicks = g_pfnGetTickCount();
    m_elapsed = 0;
    CheckAndTrigger();
    CheckAndTrigger();
    return this;
}