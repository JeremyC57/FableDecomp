// COpenCloseHelper::FrameUpdate(float frameTime) -- advance an opening/closing transition: remember last frame's amount, then step the amount up by frameTime/TotalTimeToOpen (snap to fully Opened at 1) or down by frameTime/TotalTimeToClose (snap to Closed at 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074FF60, length 115 bytes.
// Globals: 1.0f @ 0x0122DED8, the transition epsilon 0.0001f @ 0x0129BA3C.
// Layout: PDB COpenCloseHelper (0x14 bytes): Opened +0, Closed +1, Opening +2, Closing +3,
// OpenCloseTransitionAmount +4, LastOpenCloseTransitionAmount +8, TotalTimeToOpen +0xc, TotalTimeToClose +0x10.
// No engine header exists for it; the landed CTCChest ctor (0x007508D0) uses the same model.
// Shape: the flag tests compare against the shared zero register (`cmp al,dl`) that also supplies the false
// stores and the 0.0f amount; the 1.0f snap is an integer immediate store.
struct COpenCloseHelper {
    bool  Opened;                           // +0x00
    bool  Closed;                           // +0x01
    bool  Opening;                          // +0x02
    bool  Closing;                          // +0x03
    float OpenCloseTransitionAmount;        // +0x04
    float LastOpenCloseTransitionAmount;    // +0x08
    float TotalTimeToOpen;                  // +0x0c
    float TotalTimeToClose;                 // +0x10
    void FrameUpdate(float frameTime);
};

extern const float g_OpenCloseEpsilon;      // 0x0129BA3C (0.0001f)

void COpenCloseHelper::FrameUpdate(float frameTime)
{
    LastOpenCloseTransitionAmount = OpenCloseTransitionAmount;
    if (Opening)
    {
        OpenCloseTransitionAmount += frameTime / TotalTimeToOpen;
        if (OpenCloseTransitionAmount - 1.0f > -g_OpenCloseEpsilon)
        {
            Opened = true;
            Opening = false;
            OpenCloseTransitionAmount = 1.0f;
        }
    }
    else if (Closing)
    {
        OpenCloseTransitionAmount -= frameTime / TotalTimeToClose;
        if (OpenCloseTransitionAmount < g_OpenCloseEpsilon)
        {
            Closed = true;
            Closing = false;
            OpenCloseTransitionAmount = 0.0f;
        }
    }
}