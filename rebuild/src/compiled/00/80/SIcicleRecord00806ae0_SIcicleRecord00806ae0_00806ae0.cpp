// SIcicleRecord_00806ae0::SIcicleRecord_00806ae0(const SIcicleRecord_00806ae0& other) -- copy a 0x1c-byte record: the intelligent pointer at +0 (vtable 0x01238C6C) then five words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806AE0, length 71 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated over several unrelated records; named by address. Shape: the intelligent-pointer copy = base constructor, vtable, `SetPItem(other.GetPItem())` (0x00A01B10 / 0x00A01B50 / 0x00A01B90); `this` in esi, `other` in edi, `mov eax,esi`, `ret 4`.
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
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct SIcicleRecord_00806ae0 {
    CIntelligentPointer<CThing> m_pointer0;     // +0x00
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