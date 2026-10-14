// CTeleportState_00c3e520::CTeleportState_00c3e520() -- the TeleportData-style record base (@ 0x00C4D000), then twelve state words cleared and the own vtable stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3E520, length 89 bytes.
// Callees: the record base constructor @ 0x00C4D000 (landed as TeleportData_00c4d000: a novtable root with four 0x14-byte
// members; it stores its own vtable, overwritten here by 0x012B5BC0, which is not in rtti-vtables.tsv).
// Shape: every cleared member shares the `xor eax,eax` register; the store order (+0x1a4, +0x1a8, +0x180.., +0x58,
// +0x17c, +0x1d4, +0x174, +0x178) is not increasing, so these are constructor-body assignments in that order (an
// init list is emitted in declaration order); the vtable store is sunk below them by the scheduler. The
// manifest label "CDataInputStream::CDataInputStream" is propagated from a same-shape body; the class is named by
// address.
struct TeleportData_00c4d000 {                 // 0x54 bytes: vptr + four 0x14-byte members
    virtual void v00();
    unsigned char _members[0x54 - 4];
    TeleportData_00c4d000();                   // 0x00C4D000
};

struct CTeleportState_00c3e520 : TeleportData_00c4d000 {   // vtable 0x012B5BC0
    virtual void v00();
    unsigned char _pad54[4];                   // +0x54
    long m_pending;                            // +0x58
    unsigned char _pad5c[0x174 - 0x5c];        // +0x5c .. +0x174
    long m_a;                                  // +0x174
    long m_b;                                  // +0x178
    long m_c;                                  // +0x17c
    long m_d;                                  // +0x180
    long m_e;                                  // +0x184
    long m_f;                                  // +0x188
    unsigned char _pad18c[0x1a4 - 0x18c];      // +0x18c
    long m_g;                                  // +0x1a4
    long m_h;                                  // +0x1a8
    unsigned char _pad1ac[0x1b8 - 0x1ac];      // +0x1ac
    long m_i;                                  // +0x1b8
    long m_j;                                  // +0x1bc
    unsigned char _pad1c0[0x1d4 - 0x1c0];      // +0x1c0
    long m_k;                                  // +0x1d4
    CTeleportState_00c3e520();
};

CTeleportState_00c3e520::CTeleportState_00c3e520()
    : TeleportData_00c4d000()
{
    m_g = 0;
    m_h = 0;
    m_d = 0;
    m_e = 0;
    m_f = 0;
    m_i = 0;
    m_j = 0;
    m_pending = 0;
    m_c = 0;
    m_k = 0;
    m_a = 0;
    m_b = 0;
}