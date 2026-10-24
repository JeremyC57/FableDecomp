// NProgressDisplay::DisplayProgress(const CCharString& text, float progress, bool a, bool b) -- forward a progress update to the installed progress display (virtual slot 3) when one is set
// Leaf: ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9F40, length 35 bytes. PDB: ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z
//   (`__fastcall`: text in ecx, the float on the stack, a in edx, b on the stack, `ret 8`) -- the declaration every landed
//   caller uses (CWorldMap::SaveGameState 0x004FDE80, CAFile::CopyFileW 0x00998E70, ...).
// Callees: CAProgressDisplay::DisplayProgress through vtable slot 3 (`call [esi+0Ch]`).
// Globals: the CCountedPointer<CAProgressDisplay> progress display @ 0x013CAA38 (Data word).
// Shape: same forwarder as StartProgress 0x009E9EF0: pointer in eax, the arguments re-pushed right to left (b, a = edx,
//   progress, text = ecx).
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
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    if (g_ProgressDisplay.Data)
        g_ProgressDisplay->DisplayProgress(text, progress, a, b);
}
}