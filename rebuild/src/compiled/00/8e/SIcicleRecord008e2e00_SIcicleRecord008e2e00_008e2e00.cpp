// SIcicleRecord_008e2e00::SIcicleRecord_008e2e00() -- build a 0x18-byte record: two default intelligent pointers at +0 and +8 (vtable 0x01238C6C), the word at +0x10 cleared and the word at +0x14 set to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2E00, length 64 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; named by address. Shape: the second pointer's address in edi before the first vtable store, `mov eax,esi`.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct SIcicleRecord_008e2e00 {
    CIntelligentPointer<CThing> m_pointer0;       // +0x00
    CIntelligentPointer<CThing> m_pointer8;       // +0x08
    long                         m_10;             // +0x10
    long                         m_14;             // +0x14
    SIcicleRecord_008e2e00();
};

SIcicleRecord_008e2e00::SIcicleRecord_008e2e00()
    : m_pointer0(), m_pointer8(), m_10(0), m_14(-1)
{
}