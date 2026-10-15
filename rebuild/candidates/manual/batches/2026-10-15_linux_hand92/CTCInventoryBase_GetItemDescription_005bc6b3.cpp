// CTCInventoryBase::GetItemDescription(CWideString*, long, bool) -- base version: the item def's description text into `out` (the bool is unused here)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC6B3, length 35 bytes.
// Callees: the item-def description lookup @ 0x005D9630 (`__fastcall`, CWideString returned through the hidden
//          pointer in ecx, the def index in edx), CWideString::operator=(const CWideString&) @ 0x0099B7D0,
//          CWideString::~CWideString @ 0x0099B510.
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Shape: `this` is never read; the temporary description is homed in the dead `defIndex` slot ([ebp+0Ch]),
//        assigned into `out`, then destroyed. Same (out, long, bool) signature as the landed overrides
//        0x005BEAD5 / 0x005BF2ED.

struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};

CWideString __fastcall GetItemDefDescription(long defIndex);   // 0x005D9630

struct CTCInventoryBase_Retail {
    void GetItemDescription(CWideString* out, long defIndex, bool detailed);
};

void CTCInventoryBase_Retail::GetItemDescription(CWideString* out, long defIndex, bool detailed)
{
    *out = GetItemDefDescription(defIndex);
}
