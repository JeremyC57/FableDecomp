// CThingFilter_IsValid::func_00ac20f0() -- the address of the static "MPRT" descriptor at 0x0129EF40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC20F0, length 6 bytes (16 with padding); no direct reference. Part of
// the catalogue row 00ac20d0 (skipped ledger, chunk 904). 0x0129EF40 holds the four-character code "MPRT" followed by
// a table of function pointers (0x01359F2C, 0x00AC3960, 0x00AA75D0 ...): a static descriptor record.
// Shape: `mov eax,offset g_descriptor ; ret`.
struct SMprtDescriptor_Retail {
    char  m_tag[8];                            // "MPRT"
    void* m_entries[1];                        // function table (open-ended)
};

extern SMprtDescriptor_Retail g_MprtDescriptor;   // 0x0129EF40

struct CDescriptorOwner_Retail {
    SMprtDescriptor_Retail* func_00ac20f0();
};

SMprtDescriptor_Retail* CDescriptorOwner_Retail::func_00ac20f0()
{
    return &g_MprtDescriptor;
}
