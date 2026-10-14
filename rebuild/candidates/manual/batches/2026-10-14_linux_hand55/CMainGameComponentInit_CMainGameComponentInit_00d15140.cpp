// CMainGameComponentInit::CMainGameComponentInit() -- default-construct the four CCharString members (+0, +0xc, +0x10, +0x14; the two words at +4/+8 are left uninitialised)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15140, length 36 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x4). Shape: `this` in esi, `mov eax,esi`, `ret`.
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct CMainGameComponentInit {
    CCharString m_name;                            // +0x00
    long        m_4;                               // +0x04 (uninitialised)
    long        m_8;                               // +0x08 (uninitialised)
    CCharString m_c;                               // +0x0c
    CCharString m_10;                              // +0x10
    CCharString m_14;                              // +0x14
    CMainGameComponentInit();
};

CMainGameComponentInit::CMainGameComponentInit()
{
}
