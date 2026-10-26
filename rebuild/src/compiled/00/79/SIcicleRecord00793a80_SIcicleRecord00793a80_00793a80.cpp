// SIcicleRecord_00793a80::SIcicleRecord_00793a80(const SIcicleRecord_00793a80& other) -- copy a 0x18-byte record: the intelligent pointer at +0 (vtable 0x0123E7D8), the word at +8, the intelligent pointer at +0xc (vtable 0x01238C6C), the word at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793A80, length 87 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated over several unrelated records; named by address. Shape: the intelligent-pointer copy = base constructor, vtable, `SetPItem(other.GetPItem())` (0x00A01B10 / 0x00A01B50 / 0x00A01B90); `this` in esi, `other` in edi, `mov eax,esi`, `ret 4`.
class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual CBaseObject v00();
    CBaseObject* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
    void SetPItem(CBaseObject*);   // 0x00A01B90
};
struct CIntelligentPointerA_00793a80 : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual CBaseObject v00();
    CIntelligentPointerA_00793a80(const CIntelligentPointerA_00793a80& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerB_00793a80 : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual CBaseObject v00();
    CIntelligentPointerB_00793a80(const CIntelligentPointerB_00793a80& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SIcicleRecord_00793a80 {
    CIntelligentPointerA_00793a80 m_pointer0;           // +0x00
    long                      m_8;                 // +0x08
    CIntelligentPointerB_00793a80 m_pointerc;           // +0x0c
    long                      m_14;                // +0x14
    SIcicleRecord_00793a80(const SIcicleRecord_00793a80& other);
};

SIcicleRecord_00793a80::SIcicleRecord_00793a80(const SIcicleRecord_00793a80& other)
    : m_pointer0(other.m_pointer0), m_8(other.m_8), m_pointerc(other.m_pointerc), m_14(other.m_14)
{
}