// CGameScriptInterface::StartCountdownTimer(float seconds) const -- start the world's countdown timer, in world frames
// Leaf: ?StartCountdownTimer@CGameScriptInterface@@UBEXM@Z
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /O2 /Oy /W3
// Retail-asm: 0x0088F8DF `fld dword ptr [esp+8]` + 0x0088F8E3 `fistp dword ptr [esp]` (the engine's inline-assembly ROUND helper: the product is stored back into the float argument slot and reloaded for a bare dword fistp into a `push ecx` slot); VC7.1 C++ `(long)` calls _ftol2 and /QIfist emits `fistp qword` with no reload, so the dword fistp is the original's `__asm`.
// Retail 0x0088F8D0, 40 bytes (Ego_r.pdb ?StartCountdownTimer@CGameScriptInterface@@UBEXM@Z). Body:
// frames = ROUND(seconds * WFPS) with WFPS the global frame rate 0x01375550, then a tail jump to
// CWorld::StartCountdownTimer(long) 0x0049D8B0 (?StartCountdownTimer@CWorld@@QAEXJ@Z; the
// catalog's "CObservable::AddExclusiveObserver" label there is propagated) on the world pointer at
// +0x04, reusing the float argument slot for the frame count.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

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
    seconds = CMainGameComponent::GetConstantFPS() * seconds;
    long frames;
    __asm fld seconds
    __asm fistp frames
    m_pWorld->StartCountdownTimer(frames);
}
