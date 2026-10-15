// CLipSyncFrameData::FindPhonemeID(unsigned long id) const -- return a copy of the phoneme record whose id matches (linear search of the 12-byte records), or an empty record (empty name, zero id and value) when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A48A40, length 128 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: the record count is `(last - first) / 12` (0x2AAAAAAB multiply, `sar edx,1`) compared unsigned; the
// record is returned through the hidden pointer (implicit memberwise copy ctor: the string's copy ctor, then two
// dword copies); the not-found result is the record's inline default ctor (CCharString(), then two zero stores).
class CCharString {
public:
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
private:
    void* m_pData;
};
struct CLipSyncPhoneme {                            // 12 bytes
    CCharString Name;                              // +0x00
    unsigned long ID;                               // +0x04
    long        Value;                             // +0x08
    CLipSyncPhoneme() : ID(0), Value(0) { }
};
struct CLipSyncFrameData {
    CLipSyncPhoneme* m_first;                       // +0x00 the phoneme record vector
    CLipSyncPhoneme* m_last;
    CLipSyncPhoneme* m_end;
    unsigned long size() const { return m_last - m_first; }
    CLipSyncPhoneme FindPhonemeID(unsigned long id) const;
};

CLipSyncPhoneme CLipSyncFrameData::FindPhonemeID(unsigned long id) const
{
    for (unsigned long i = 0; i < size(); ++i)
        if (m_first[i].ID == id)
            return m_first[i];
    return CLipSyncPhoneme();
}