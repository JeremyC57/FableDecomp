// CSoundEvent::YieldPhysicalPlayer(CSoundEventManager*) -- libpk sound event in state 6: give the physical player up and go back to waiting for one (6 -> 1 -> 3), keeping its channel
// Leaf: ?YieldPhysicalPlayer@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Retail: Fable.exe @ 0x00C216D0, 35 bytes, `ret 4`. Descriptive names (the libpk audio library has no PDB or RTTI;
// class and transition names follow chunks 433/434: CSoundEvent, the 0xB4-byte event with its state word at +0xAC;
// chunk 435 calls the same class CPanelItem). Label correction: the manifest row "CWorldMap::ApplyScriptedMapBrush"
// is a propagation; the caller 0x00C10CBB hands it the entry found by 0x00C21EB0 together with the manager.
// Callees: CheckState(long) 0x00C20720 (6 = "Playing using physical player"), StopPhysical(CSoundEventManager*)
// 0x00C20B50 (6 -> 1, returns the channel), WaitForPhysicalPlayer(CSoundEventManager*, CSoundChannel*) 0x00C20950
// (1 -> 3).
// Shape: right-to-left arguments (the StopPhysical call is the second argument, evaluated before `manager` is pushed);
// StopPhysical is called with ecx still `this` from before CheckState: VC7.1 knows the same-TU CheckState body (all
// switch cases empty, it touches only eax) preserves ecx, so CheckState's body is defined above the caller under
// auto_inline(off) (only the function named on line 1 is graded).
struct CSoundEventManager;
struct CSoundChannel;

class CSoundEvent {
public:
    unsigned char  _pad00[0xac];
    unsigned short m_state;                     // +0xAC
    unsigned char  _padae[0x6];
    void CheckState(long expected);             // 0x00C20720
    CSoundChannel* StopPhysical(CSoundEventManager* manager);  // 0x00C20B50
    void WaitForPhysicalPlayer(CSoundEventManager* manager, CSoundChannel* channel);    // 0x00C20950
    void YieldPhysicalPlayer(CSoundEventManager* manager);
};

#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

void CSoundEvent::YieldPhysicalPlayer(CSoundEventManager* manager)
{
    CheckState(6);
    WaitForPhysicalPlayer(manager, StopPhysical(manager));
}