// CWorld::StopAutoSaveProgress() -- if the auto-save progress display is up, keeps it on screen (100 ms sleeps, re-displaying with no text) until its minimum display time has passed, stops the progress, restores the previous progress display and releases the saved handle
// Leaf: StopAutoSaveProgress
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E8C0, 212 bytes, plain `ret` (`this` is never read).
//   PDB name ?StopAutoSaveProgress@CWorld@@IAEXXZ.
// Callees: GFGetTime 0x009E1BC0 (`__fastcall`, double), KERNEL32 Sleep (import 0x0143FE1C, loaded once into esi),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, DisplayProgress 0x009E9F40, StopProgress
//   0x009E9F20, SetProgressDisplay 0x009E9FD0 (all `__fastcall`), operator delete 0x00BFE9BC (the counted block).
// Data: the shown flag 0x013B89C1, the minimum end time 0x013B89C8 (double), GOldAutoSaveProgressDisplay 0x013B89E0
//   (CCountedPointer<CAProgressDisplay>: object +0, count block +4; its atexit destructor is the landed 0x01228C60).
// Shape: `while (GFGetTime() < end)` is rotated (test before the loop and at the bottom); the empty text is an
//   implicit conversion temporary; the handle is released by an inline Clear() that zeroes the block word first.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
class CAProgressDisplay;
struct CCountedPointer_CAProgressDisplay {
    CAProgressDisplay* Data;                       // +0
    SCountBlock*       Info;                       // +4
    void Clear()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // KERNEL32 (0x0143FE1C)
double __fastcall GFGetTime();                     // 0x009E1BC0
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40
void __fastcall StopProgress(bool stopNow, bool keepScreen);   // 0x009E9F20
void __fastcall SetProgressDisplay(CCountedPointer_CAProgressDisplay* display);   // 0x009E9FD0
extern bool g_bAutoSaveProgressShown;              // 0x013B89C1
extern double g_AutoSaveProgressEndTime;           // 0x013B89C8
extern CCountedPointer_CAProgressDisplay GOldAutoSaveProgressDisplay;   // 0x013B89E0
class CWorld { public:
    void StopAutoSaveProgress();
};
void CWorld::StopAutoSaveProgress()
{
    if (g_bAutoSaveProgressShown) {
        while (GFGetTime() < g_AutoSaveProgressEndTime) {
            Sleep(100);
            NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        }
        StopProgress(true, false);
        SetProgressDisplay(&GOldAutoSaveProgressDisplay);
        if (GOldAutoSaveProgressDisplay.Data)
            StopProgress(true, false);
        GOldAutoSaveProgressDisplay.Clear();
        g_bAutoSaveProgressShown = false;
    }
}