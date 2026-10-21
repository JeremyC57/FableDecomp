// CMainGameComponent::PostChangeResolution(unsigned long, unsigned long, unsigned long) -- pass a resolution change on to the display engine, then the world
// Leaf: ?PostChangeResolution@CMainGameComponent@@QAEXKKK@Z
// Retail: Fable.exe @ 0x00416365, 45 bytes, `ret 0Ch`; Ego_r.pdb / FableWin.pdb ?PostChangeResolution@CMainGameComponent@@QAEXKKK@Z.
// Forwards its three arguments to CDisplayEngine::PostChangeResolution (0x00434A20; Ego_r.pdb ...@CDisplayEngine@@QAEXKKK@Z)
// on PDisplayEngine (+0x28), then to the world's handler (0x0049D820, an empty `ret 0Ch`; name descriptive) on PWorld (+0x24).
// Layout: retail CMainGameComponent PWorld +0x24, PDisplayEngine +0x28 (as 0x00416392 / 0x004160A6).
// Opt: size TU with frame pointers (`optimize("y", off)`: push ebp / mov ebp,esp; `push dword ptr [ebp+x]` re-reads).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CDisplayEngine {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00434A20
};
class CWorld {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x0049D820
};
class CMainGameComponent {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);
    unsigned char   _pad00[0x24];
    CWorld*         PWorld;                              // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
};
void CMainGameComponent::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    PDisplayEngine->PostChangeResolution(width, height, depth);
    PWorld->PostChangeResolution(width, height, depth);
}