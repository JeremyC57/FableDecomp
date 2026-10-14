// CVtable_012f2b0c::CVtable_012f2b0c(long a, long b) -- the CVtable_012c1648 base (0x00CB8110), the two keys at +0x40 / +0x44 (an intermediate novtable base), this class's vtable (0x012F2B0C, no RTTI), the three words at +0x7c..+0x84 cleared and the four CCharStrings at +0x88 (a counted loop of constructors)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED0E60, length 96 bytes.
// The chunk label "CInputProcessManager::CInputProcessManager" is propagated; named by its vtable. Shape: the loop counter in ebx, the string pointer in edi (+4 per step), `ret 8`.
struct CVtable_012c1648 {                          // 0x40 bytes (landed 0x00CB8110)
    virtual void v00();
    unsigned char _body[0x3c];
    CVtable_012c1648();                            // 0x00CB8110
};
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
struct __declspec(novtable) CKeyedBase_00ed0e60 : CVtable_012c1648 {   // the intermediate base: the two keys stored before the derived vtable
    long m_40;                                     // +0x40
    long m_44;                                     // +0x44
    CKeyedBase_00ed0e60(long a, long b) : CVtable_012c1648() { m_40 = b; m_44 = a; }
};
struct CVtable_012f2b0c : CKeyedBase_00ed0e60 {    // vtable 0x012F2B0C
    unsigned char _pad48[0x34];
    long          m_7c;                            // +0x7c
    long          m_80;                            // +0x80
    long          m_84;                            // +0x84
    CCharString   m_strings[4];                    // +0x88
    virtual void v00();
    CVtable_012f2b0c(long a, long b);
};

CVtable_012f2b0c::CVtable_012f2b0c(long a, long b)
    : CKeyedBase_00ed0e60(a, b), m_7c(0), m_80(0), m_84(0)
{
}