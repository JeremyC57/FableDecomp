// CGameScriptInterface::Rest(float seconds) -- block the calling script for the given time: compute the end frame (now + frames-per-second * seconds) and, while the game frame is still before it and the script runner has not been told to stop, yield one script frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E71F0, length 128 bytes. `ret 4`.
// Callees: the global frame-counter getter @ 0x0049D870 (`mov eax,[0x013B89BC] ; ret`; catalog "GFGetPreMainMemoryUsed" is wrong), the
// script runner's virtual slot 2 (+8, yield one frame; as the landed NewScriptFrame 0x006E7410, which tests the same stop byte +5).
// Globals: the script runner @ 0x013D2838, the frames-per-second count @ 0x01375550.
// Shape: the end frame is a float homed in the dead `seconds` slot; the frame rate is read before the clock call; the loop is a
// `while` whose test is duplicated at the bottom (`fild ; fcomp ; test ah,5 ; jp/jnp`), the runner global re-read every pass.
struct CScriptRunner {
    virtual void v00();
    virtual void v01();
    virtual void YieldFrame();              // slot 2 (+8)
    unsigned char m_pad4;                   // +0x04
    bool          m_bStopRequested;         // +0x05
};

extern CScriptRunner* g_pScriptRunner;      // 0x013D2838
extern long g_FramesPerSecond;              // 0x01375550
long GetGameFrame();                        // 0x0049D870

struct CGameScriptInterface {
    void Rest(float seconds);
};

void CGameScriptInterface::Rest(float seconds)
{
    if (g_pScriptRunner) {
        long fps = g_FramesPerSecond;
        float endFrame = (float)GetGameFrame() + (float)fps * seconds;
        while ((float)GetGameFrame() < endFrame) {
            if (g_pScriptRunner->m_bStopRequested)
                break;
            g_pScriptRunner->YieldFrame();
        }
    }
}