// CAutoMapEntry_00c3baf0::CAutoMapEntry_00c3baf0() -- default constructor of the 0x30-byte libpk auto-map entry: the key at +0 and every other word/half-word zeroed, the MFC-style name string at +8 starting as the shared empty string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3BAF0, length 66 bytes (the oracle row carries 14 bytes of int3 padding).
// Callees: none; reads g_pchNil @ 0x0139C900. The keyed twin is @ 0x00C3BAA0 (same layout, key argument stored first). Shape: `mov eax,ecx`
// frees ecx as the zero register; six `mov word ptr` stores are the short members. The manifest label is propagated; the class is named by address.
extern char* g_pchNil_0139c900;                    // 0x0139C900

struct CAutoMapEntry_00c3baf0 {                    // 0x30 bytes
    void*          m_pKey;                         // +0x00
    long           m_4;                            // +0x04
    char*          m_pchName;                      // +0x08 MFC-style string data (nil = shared empty)
    short          m_c;                            // +0x0c
    short          m_e;                            // +0x0e
    long           m_10;                           // +0x10
    long           m_14;                           // +0x14
    short          m_18;                           // +0x18
    short          m_1a;                           // +0x1a
    short          m_1c;                           // +0x1c
    short          m_1e;                           // +0x1e
    short          m_20;                           // +0x20
    short          m_22;                           // +0x22
    long           m_24;                           // +0x24
    long           m_28;                           // +0x28
    long           m_2c;                           // +0x2c
    CAutoMapEntry_00c3baf0();
};

CAutoMapEntry_00c3baf0::CAutoMapEntry_00c3baf0()
    : m_pKey(0)
    , m_4(0)
    , m_pchName(g_pchNil_0139c900)
    , m_c(0), m_e(0)
    , m_10(0), m_14(0)
    , m_18(0), m_1a(0), m_1c(0), m_1e(0), m_20(0), m_22(0)
    , m_24(0), m_28(0), m_2c(0)
{
}