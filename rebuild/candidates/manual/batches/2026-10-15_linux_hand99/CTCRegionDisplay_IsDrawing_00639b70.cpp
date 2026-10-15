// CTCRegionDisplay::IsDrawing() -- TRUE while the region name is visible: alpha above zero and a non-empty current area name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00639B70, length 33 bytes.
// Callees: CWideString::operator!=(const wchar_t*) const @ 0x0099B150 (on CurrentAreaName, `add ecx,0Ch`), with the pooled
// empty wide literal L"" @ 0x0122D70C.
// Shape: `cmp`-free `test eax,eax ; jle` on the signed alpha, the string compare's bool tested with `test al,al ; je`, and a
// `long` (BOOL) result on two separate exits (`mov eax,1 ; ret` / `xor eax,eax ; ret`): `if (a && b) return 1; return 0;`.
// Layout = Ego_r.pdb CTCRegionDisplay (0x28): CurrentAreaName (CWideString) +0x0c, CurrentAreaNameExtra +0x10,
// FrameNameChanged +0x14, CurrentAlpha +0x18, LastAlpha +0x1c, DisplayTime +0x20, TimeOut / InfoDisplay / UpdateWhilePaused
// +0x24..+0x26. No engine header; modelled locally (wchar_t is the era typedef from <stddef.h>).
#include <stddef.h>

struct CWideString {
    void* m_pData;
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
};

struct CTCRegionDisplay {
    virtual void v00();
    void*       ParentThing;                                // +0x04 (CTCBase)
    bool        ToBeRemoved;                                // +0x08
    bool        IsInGlobalUpdateTCs;                        // +0x09
    CWideString CurrentAreaName;                            // +0x0c
    CWideString CurrentAreaNameExtra;                       // +0x10
    long        FrameNameChanged;                           // +0x14
    long        CurrentAlpha;                               // +0x18
    long IsDrawing() const;
};

long CTCRegionDisplay::IsDrawing() const
{
    if (CurrentAlpha > 0 && CurrentAreaName != L"")
        return 1;
    return 0;
}
