// SIcicleRecord_008e2e00::SIcicleRecord_008e2e00() -- build a 0x18-byte record: two default intelligent pointers at +0 and +8 (vtable 0x01238C6C), the word at +0x10 cleared and the word at +0x14 set to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2E00, length 64 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; named by address. Shape: the second pointer's address in edi before the first vtable store, `mov eax,esi`.
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
struct CIntelligentPointer_008e2e00 : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_008e2e00() : CBaseIntelligentPointer() { }
};
struct SIcicleRecord_008e2e00 {
    CIntelligentPointer_008e2e00 m_pointer0;       // +0x00
    CIntelligentPointer_008e2e00 m_pointer8;       // +0x08
    long                         m_10;             // +0x10
    long                         m_14;             // +0x14
    SIcicleRecord_008e2e00();
};

SIcicleRecord_008e2e00::SIcicleRecord_008e2e00()
    : m_pointer0(), m_pointer8(), m_10(0), m_14(-1)
{
}