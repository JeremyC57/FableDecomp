// CSubtitleRenderer::CSetTextInfo::CSetTextInfo(const CWideString& text, const CWideString& button1Text, const CWideString& button2Text, const CWideString& button3Text, unsigned long displayFlags, float displayTime, bool updateWhilePaused, bool changePlayerMode) -- copy-construct the four strings, then store the flags, time and two switches
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DDF6, 78 bytes (`ret 20h`: eight arguments).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, four times (the members are
// constructed in declaration order from the pass-through `push dword ptr [ebp+N]` reference arguments).
//
// Shape: a frame-pointer, size-optimised TU -- `#pragma optimize("y", off)` + `#pragma optimize("s", on)`
// (GOTCHAS wave 24b: a frame-pointer size body with no spills); the float is copied as a dword
// (`mov eax,[ebp+1Ch] ; mov [esi+14h],eax`), the bools through al.
// Layout = Ego_r.pdb CSetTextInfo (28 bytes): CWideString Text +0, Button1Text +4, Button2Text +8,
// Button3Text +0xC, unsigned long DisplayFlags +0x10, float DisplayTime +0x14, bool UpdateWhilePaused +0x18,
// bool ChangePlayerMode +0x19 (no engine header exists for CSetTextInfo).
// Leaf: ??0CSetTextInfo@CSubtitleRenderer@@QAE@
// Identity (sweep2 1g): Ego_r.pdb's name set holds `??0CSetTextInfo@CSubtitleRenderer@@QAE@ABVCWideString@@000KM_N1@Z`
//   (the nested class; its copy ctor 0x004935A7 and dtor 0x0043E3D0 carry the same nesting).

#pragma optimize("y", off)
#pragma optimize("s", on)

struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);       // 0x0099B720
};

class CSubtitleRenderer {
public:
  class CSetTextInfo {                             // Ego_r.pdb, 28 bytes (a class: mangled V)
public:
    CWideString   Text;                          // +0x00
    CWideString   Button1Text;                   // +0x04
    CWideString   Button2Text;                   // +0x08
    CWideString   Button3Text;                   // +0x0C
    unsigned long DisplayFlags;                  // +0x10
    float         DisplayTime;                   // +0x14
    bool          UpdateWhilePaused;             // +0x18
    bool          ChangePlayerMode;              // +0x19
    CSetTextInfo(const CWideString& text, const CWideString& button1Text, const CWideString& button2Text,
                 const CWideString& button3Text, unsigned long displayFlags, float displayTime,
                 bool updateWhilePaused, bool changePlayerMode);
};
};

CSubtitleRenderer::CSetTextInfo::CSetTextInfo(const CWideString& text, const CWideString& button1Text,
                                              const CWideString& button2Text, const CWideString& button3Text,
                                              unsigned long displayFlags, float displayTime, bool updateWhilePaused,
                                              bool changePlayerMode)
    : Text(text)
    , Button1Text(button1Text)
    , Button2Text(button2Text)
    , Button3Text(button3Text)
    , DisplayFlags(displayFlags)
    , DisplayTime(displayTime)
    , UpdateWhilePaused(updateWhilePaused)
    , ChangePlayerMode(changePlayerMode)
{
}