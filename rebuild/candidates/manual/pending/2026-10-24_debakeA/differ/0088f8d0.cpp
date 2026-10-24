// Grade: DIFFER(34v40) -- IMPOSSIBLE: retail rounds seconds*WFPS with a bare `fistp dword ptr [esp]` after a float store/reload through the dead argument slot (the engine's inline-assembly ROUND helper, GOTCHAS 211/493 asm_bake family); plain `(long)` compiles to __ftol2 and /QIfist to a qword `fistp` with no reload (34v40 under /QIfist: `sub esp,8 ; fistp qword`), so no compiled C++ reproduces it.
// CGameScriptInterface::StartCountdownTimer(float seconds) const -- start the world's countdown timer, in world frames
// Retail 0x0088F8D0, 40 bytes (Ego_r.pdb ?StartCountdownTimer@CGameScriptInterface@@UBEXM@Z). Body:
// frames = ROUND(seconds * WFPS) with WFPS the global frame rate 0x01375550, then a tail jump to
// CWorld::StartCountdownTimer(long) 0x0049D8B0 (?StartCountdownTimer@CWorld@@QAEXJ@Z; the
// catalog's "CObservable::AddExclusiveObserver" label there is propagated) on the world pointer at
// +0x04, reusing the float argument slot for the frame count.
// Leaf: StartCountdownTimer
// Flags: /QIfist

extern long g_framesPerSecond;          // 0x01375550

class CWorld
{
public:
    void StartCountdownTimer(long frames);      // 0x0049D8B0
};

class CGameScriptInterface
{
public:
    virtual void StartCountdownTimer(float seconds) const;
    CWorld* m_pWorld;                   // +0x04
};

void CGameScriptInterface::StartCountdownTimer(float seconds) const
{
    float frames = g_framesPerSecond * seconds;
    m_pWorld->StartCountdownTimer((long)frames);
}
