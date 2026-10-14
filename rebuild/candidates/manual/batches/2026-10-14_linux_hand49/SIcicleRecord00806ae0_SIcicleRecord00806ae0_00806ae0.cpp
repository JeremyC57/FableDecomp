// SIcicleRecord_00806ae0::SIcicleRecord_00806ae0(const SIcicleRecord_00806ae0& other) -- copy a 0x1c-byte record: the intelligent pointer at +0 (vtable 0x01238C6C) then five words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806AE0, length 71 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated over several unrelated records; named by address. Shape: the intelligent-pointer copy = base constructor, vtable, `SetPItem(other.GetPItem())` (0x00A01B10 / 0x00A01B50 / 0x00A01B90); `this` in esi, `other` in edi, `mov eax,esi`, `ret 4`.
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
struct CIntelligentPointerA_00806ae0 : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerA_00806ae0(const CIntelligentPointerA_00806ae0& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SIcicleRecord_00806ae0 {
    CIntelligentPointerA_00806ae0 m_pointer0;     // +0x00
    long m_8;                                      // +0x08
    long m_c;                                      // +0x0c
    long m_10;                                     // +0x10
    long m_14;                                     // +0x14
    long m_18;                                     // +0x18
    SIcicleRecord_00806ae0(const SIcicleRecord_00806ae0& other);
};

SIcicleRecord_00806ae0::SIcicleRecord_00806ae0(const SIcicleRecord_00806ae0& other)
    : m_pointer0(other.m_pointer0), m_8(other.m_8), m_c(other.m_c), m_10(other.m_10), m_14(other.m_14), m_18(other.m_18)
{
}
