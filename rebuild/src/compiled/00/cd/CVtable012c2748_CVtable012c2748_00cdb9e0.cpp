// CVtable_012c2748::CVtable_012c2748() -- CBaseClassNonCopyable (0x0099A2F0), this class's vtable (0x012C2748, no RTTI), the CCharString at +0x54 (0x0099E4B0), then the three words at +0x134..+0x13c cleared
// Leaf: ??0CVtable_012c2748@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDB9E0, length 48 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; named by its vtable.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
struct CVtable_012c2748 : CBaseClassNonCopyable {                  // vtable 0x012C2748
    unsigned char _pad04[0x50];
    CCharString   m_string54;                      // +0x54
    unsigned char _pad58[0xdc];
    long          m_134;                           // +0x134
    long          m_138;                           // +0x138
    long          m_13c;                           // +0x13c
    CVtable_012c2748();
};

CVtable_012c2748::CVtable_012c2748()
    : CBaseClassNonCopyable(), m_134(0), m_138(0), m_13c(0)
{
}