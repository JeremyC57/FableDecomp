// CMeleeApprentice::Init() -- clear the script's state: the thirteen flag bytes +0x48..+0x54 (stored in the order 48..4f, 51, 53, 54, 50, 52), the dword at +0x58 and the byte at +0x68
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9FEC0, length 48 bytes.
// Leaf: Init
// Shape: no frame, one zero register, the stores in declaration order of the model, `ret`.
struct CMeleeApprentice {
    unsigned char _pad00[0x48];
    bool m_48, m_49, m_4a, m_4b, m_4c, m_4d, m_4e, m_4f;   // +0x48..+0x4f
    bool m_50;                                     // +0x50
    bool m_51;                                     // +0x51
    bool m_52;                                     // +0x52
    bool m_53;                                     // +0x53
    bool m_54;                                     // +0x54
    unsigned char _pad55[3];
    long m_58;                                     // +0x58
    unsigned char _pad5c[0xc];
    bool m_68;                                     // +0x68
    void Init();
};

void CMeleeApprentice::Init()
{
    m_48 = false; m_49 = false; m_4a = false; m_4b = false; m_4c = false; m_4d = false; m_4e = false; m_4f = false;
    m_51 = false;
    m_53 = false;
    m_54 = false;
    m_50 = false;
    m_52 = false;
    m_58 = 0;
    m_68 = false;
}