// CVtable_012e5ff0::CVtable_012e5ff0() -- base CBase_0042d9e0() (0x0042D9E0), this class's vtable, then the twelve dwords +0x2c..+0x58 zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F336F0, length 56 bytes (64 with padding).
// Leaf: ??0CVtable_012e5ff0
// The chunk label (CSpecialAbilitiesBattleChargeDef) is propagated from a byte-similar def constructor; the
// vtable stored here, 0x012E5FF0, has no complete-object locator (a no-RTTI module), so the class is named
// after it, and the base constructor 0x0042D9E0 (manifest label CPSCBase, a hint only) by address, as in the
// landed 0x00F33DC0 family. Shape: `xor eax,eax` before the vtable store, the vtable store before twelve
// `mov [esi+N],eax` (+0x2c..+0x58): the zeroed block is one embedded member object with an inline
// constructor. Member types are not recoverable; `long` models them.
struct CBase_0042d9e0 {
    virtual void v00();
    unsigned char _base[0x28];                  // +0x04..+0x2b: the base's own members
    CBase_0042d9e0();                           // 0x0042D9E0
};

struct SZeroed_012e5ff0 {                       // +0x2c..+0x5b: twelve dwords, all zero after construction
    long m_2c, m_30, m_34, m_38, m_3c, m_40, m_44, m_48, m_4c, m_50, m_54, m_58;
    SZeroed_012e5ff0()
        : m_2c(0), m_30(0), m_34(0), m_38(0), m_3c(0), m_40(0), m_44(0), m_48(0),
          m_4c(0), m_50(0), m_54(0), m_58(0) { }
};

struct CVtable_012e5ff0 : CBase_0042d9e0 {
    SZeroed_012e5ff0 m_data;                    // +0x2c
    virtual void v00();
    CVtable_012e5ff0();
};

CVtable_012e5ff0::CVtable_012e5ff0()
    : CBase_0042d9e0()
    , m_data()
{
}