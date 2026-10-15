// CSetTextInfo::CSetTextInfo(const CSetTextInfo& other) -- copy constructor: copy-construct the four strings, then copy the flags, time and two switches
// Retail: Fable.exe (TLC, VC7.1) @ 0x004935A7, 81 bytes (`ret 4`).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, four times with `lea eax,[edi+N]`
// (the source object kept in edi).
//
// Shape: memberwise copy in declaration order; the float is copied as a dword, the bools through al.
// The body sits at an unaligned address in a size-optimised module: `#pragma optimize("s", on)` is needed
// (the default /O2 build schedules the copies differently: same length, different order).
// Layout = Ego_r.pdb CSetTextInfo (28 bytes): CWideString Text +0, Button1Text +4, Button2Text +8,
// Button3Text +0xC, unsigned long DisplayFlags +0x10, float DisplayTime +0x14, bool UpdateWhilePaused +0x18,
// bool ChangePlayerMode +0x19 (no engine header exists for CSetTextInfo).
// Leaf: ??0CSetTextInfo

#pragma optimize("s", on)

struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);       // 0x0099B720
};

struct CSetTextInfo {
    CWideString   Text;                          // +0x00
    CWideString   Button1Text;                   // +0x04
    CWideString   Button2Text;                   // +0x08
    CWideString   Button3Text;                   // +0x0C
    unsigned long DisplayFlags;                  // +0x10
    float         DisplayTime;                   // +0x14
    bool          UpdateWhilePaused;             // +0x18
    bool          ChangePlayerMode;              // +0x19
    CSetTextInfo(const CSetTextInfo& other);
};

CSetTextInfo::CSetTextInfo(const CSetTextInfo& other)
    : Text(other.Text)
    , Button1Text(other.Button1Text)
    , Button2Text(other.Button2Text)
    , Button3Text(other.Button3Text)
    , DisplayFlags(other.DisplayFlags)
    , DisplayTime(other.DisplayTime)
    , UpdateWhilePaused(other.UpdateWhilePaused)
    , ChangePlayerMode(other.ChangePlayerMode)
{
}
