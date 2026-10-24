// NProgressDisplay::StartProgress(bool a, bool b, bool c, bool d, bool e, bool f) -- forward the six start flags to the installed progress display (virtual slot 1) when one is set
// Leaf: ?StartProgress@NProgressDisplay@@YIX_N00000@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9EF0, length 45 bytes. PDB: ?StartProgress@NProgressDisplay@@YIX_N00000@Z
//   (`__fastcall`: a/b in ecx/edx, the other four on the stack, `ret 10h`).
// Callees: CAProgressDisplay::StartProgress through vtable slot 1 (`call [esi+4]`; CProgressDisplay is the implementation).
// Globals: the CCountedPointer<CAProgressDisplay> progress display @ 0x013CAA38 (its Data word; set by the landed
//   SetProgressDisplay 0x009E9FD0; the landed StopProgress 0x009E9F20 calls slot 2 the same way).
// Shape: the pointer loaded once into eax and reused as `this`; the four stack flags are re-pushed through edi as whole
//   dwords (bool parameters are not re-widened), then edx and ecx.
class CCharString;

class CAProgressDisplay                            // abstract progress-display interface
{
public:
    virtual ~CAProgressDisplay();                                                          // slot 0
    virtual void StartProgress(bool a, bool b, bool c, bool d, bool e, bool f) = 0;       // slot 1
    virtual void StopProgress(bool a, bool b) = 0;                                         // slot 2
    virtual void DisplayProgress(const CCharString& text, float progress, bool a, bool b) = 0;   // slot 3
};

class CCountedPointerInfo;
template <class T>
class CCountedPointer
{
public:
    T*                   Data;                     // +0x00
    CCountedPointerInfo* Info;                     // +0x04
    T* operator->() const { return Data; }
};

extern CCountedPointer<CAProgressDisplay> g_ProgressDisplay;   // 0x013CAA38

namespace NProgressDisplay {
void __fastcall StartProgress(bool a, bool b, bool c, bool d, bool e, bool f)
{
    if (g_ProgressDisplay.Data)
        g_ProgressDisplay->StartProgress(a, b, c, d, e, f);
}
}