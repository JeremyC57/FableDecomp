// CWorld::StartCountdownTimer(long frames) -- store the script countdown at CWorld +0xA0 (CountdownTimer)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D8B0, 13 bytes: `mov eax,[esp+4] ; mov [ecx+0A0h],eax ; ret 4` (thiscall).
// Identity: the only caller is CGameScriptInterface::StartCountdownTimer 0x0088F8D0, which tail-jumps here with the world
//   in ecx; Ego_r.pdb + FableWin.pdb ?StartCountdownTimer@CWorld@@QAEXJ@Z directly after GetFrame / GetFrameAsString /
//   SetFrame (retail 0x0049D870 / 0x0049D880 / 0x0049D8A0, the same order); Ego_r.pdb CWorld::CountdownTimer is the long at
//   +0xA0. Not CObservable::AddExclusiveObserver (a propagated byte-twin label).
// Layout: engine/CWorld.h has the field but declares no member functions, so the class is modelled locally with the
//   header's +0xA0 offset.
// Leaf: ?StartCountdownTimer@CWorld@@QAEXJ@Z
class CWorld {
public:
    unsigned char _pad00[0xa0];
    long CountdownTimer;                        // +0xA0
    void StartCountdownTimer(long frames);
};

void CWorld::StartCountdownTimer(long frames)
{
    CountdownTimer = frames;
}
