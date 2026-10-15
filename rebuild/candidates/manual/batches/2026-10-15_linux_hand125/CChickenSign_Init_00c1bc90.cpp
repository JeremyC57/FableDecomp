// CChickenSign::Init() -- resets a packed 20-byte parameter record to its defaults through its own setters
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BC90, 121 bytes of code. The record and its accessors live at 0x00C1BA20..0x00C1BC88
// in the same translation unit; the class name is the manifest's (no RTTI), the member names are descriptive.
// Callees (all __thiscall, `ret 4`): SetLevel @ 0x00C1BB20 (byte +0 = x*127), SetScale @ 0x00C1BB40 (short +8 = x*100),
//          SetOffset @ 0x00C1BB60 (byte +1 = x*100), SetRangeMin @ 0x00C1BB80 / SetRangeMax @ 0x00C1BBA0 (shorts +0x10/+0x12
//          through the 16-bit fixed-point encoder @ 0x00C1BA20: x*65535*0.0001), SetWordA @ 0x00C1BBC0 (+0xa),
//          SetWordB @ 0x00C1BBD0 (+4), SetWordC @ 0x00C1BBE0 (+6), SetByteD @ 0x00C1BBF0 (+3), SetWordE @ 0x00C1BC00 (+0xc),
//          SetFlag0 @ 0x00C1BC10 (bit 0 of the flag byte +2), SetTime @ 0x00C1BC70 (short +0xe = x*65535).
// Globals: the default range floats @ 0x0139C96C (1.0) and @ 0x0139C970 (30.0), pushed by value.
// Shape: VC7.1 knows the register usage of setters compiled EARLIER in the same TU: the plain word/byte setters
// touch only eax, so after SetWordA the calls run without reloading ecx from esi (the float setters call __ftol2 and
// clobber ecx, so each of those is preceded by `mov ecx,esi`). The setters are therefore defined above Init with
// automatic inlining off. The flag byte is cleared first (scheduled after the first push).
#pragma auto_inline(off)

extern float g_defaultRangeMin;                 // 0x0139C96C
extern float g_defaultRangeMax;                 // 0x0139C970

static long __stdcall EncodeFixed16(float value)   // 0x00C1BA20
{
    return (long)(value * 65535.0f * 0.0001f);
}

struct CChickenSign {
    unsigned char  m_level;                     // +0x00 0..1 as 0..127
    unsigned char  m_offset;                    // +0x01 0..1 as 0..100
    unsigned char  m_flags;                     // +0x02 bit 0..2: three switches
    unsigned char  m_byteD;                     // +0x03
    short          m_wordB;                     // +0x04
    short          m_wordC;                     // +0x06
    short          m_scale;                     // +0x08 0..1 as 0..100
    unsigned short m_wordA;                     // +0x0a
    unsigned short m_wordE;                     // +0x0c
    unsigned short m_time;                      // +0x0e 0..1 as 0..65535
    unsigned short m_rangeMin;                  // +0x10 16-bit fixed point (0..10000)
    unsigned short m_rangeMax;                  // +0x12

    void SetLevel(float value);
    void SetScale(float value);
    void SetOffset(float value);
    void SetRangeMin(float value);
    void SetRangeMax(float value);
    void SetWordA(unsigned short value);
    void SetWordB(short value);
    void SetWordC(short value);
    void SetByteD(unsigned char value);
    void SetWordE(unsigned short value);
    void SetFlag0(bool on);
    void SetTime(float value);
    void Init();
};

void CChickenSign::SetLevel(float value)
{
    m_level = (unsigned char)(long)(value * 127.0f);
}

void CChickenSign::SetScale(float value)
{
    m_scale = (short)(long)(value * 100.0f);
}

void CChickenSign::SetOffset(float value)
{
    m_offset = (unsigned char)(long)(value * 100.0f);
}

void CChickenSign::SetRangeMin(float value)
{
    m_rangeMin = (unsigned short)EncodeFixed16(value);
}

void CChickenSign::SetRangeMax(float value)
{
    m_rangeMax = (unsigned short)EncodeFixed16(value);
}

void CChickenSign::SetWordA(unsigned short value)
{
    m_wordA = value;
}

void CChickenSign::SetWordB(short value)
{
    m_wordB = value;
}

void CChickenSign::SetWordC(short value)
{
    m_wordC = value;
}

void CChickenSign::SetByteD(unsigned char value)
{
    m_byteD = value;
}

void CChickenSign::SetWordE(unsigned short value)
{
    m_wordE = value;
}

void CChickenSign::SetFlag0(bool on)
{
    if (on) m_flags |= 1; else m_flags &= ~1;
}

void CChickenSign::SetTime(float value)
{
    m_time = (unsigned short)(long)(value * 65535.0f);
}

void CChickenSign::Init()
{
    m_flags = 0;
    SetLevel(1.0f);
    SetScale(1.0f);
    SetOffset(0.0f);
    SetRangeMin(g_defaultRangeMin);
    SetRangeMax(g_defaultRangeMax);
    SetWordA(1);
    SetWordB(0);
    SetWordC(0);
    SetByteD(0);
    SetWordE(0);
    SetFlag0(true);
    SetTime(1.0f);
}
