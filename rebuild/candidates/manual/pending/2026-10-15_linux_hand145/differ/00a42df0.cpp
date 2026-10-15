// CLipSyncDataBank::NotifyEntryAdded(unsigned long, unsigned long index, const CArray<char>& data) -- grow the per-entry table (+0x164) to cover the new entry (zero-filled), record the entry's leading float (the sample length stored in the first four data bytes) and forward to CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42DF0, length 128 bytes.
// Grade: DIFFER(119v119) -- scheduling only: retail stores the float (`fstp [edx+edi*8+4]`) between `push edi` and `push eax` (the base call's id), ours after both pushes | tried: begin()+index, m_first[index], a float local punned through its address
// Leaf: ?NotifyEntryAdded@CLipSyncDataBank@@MAEXKKABV?$CArray@D@@@Z
// Callees: the entry vector's resize(size, value) @ 0x00A43F20 (as in the landed NotifyBankSize 0x00A42E70),
//          CBankFile::NotifyEntryAdded @ 0x009CBC20 (direct base call).
// Shape: size() is the signed `(last - first) >> 3` compared unsigned; the zero value is an 8-byte frame
// temporary stored after the pushes; the data pointer is the inline `size() ? first : (char*)this` and its
// first dword is reinterpreted as a float through a local the compiler homes in the dead `index` slot.
template <class T> class CArray {
public:
    T* m_first;
    T* m_last;
    T* m_end;
    const T* GetData() const { return (m_last - m_first) ? m_first : (const T*)this; }
};
struct SLipSyncEntry {                             // two floats, zeroed by the inline constructor
    float m_0;
    float m_length;                                // +0x04
    SLipSyncEntry() : m_0(0.0f), m_length(0.0f) { }
};
struct CLipSyncEntryVector {
    SLipSyncEntry* m_first;
    SLipSyncEntry* m_last;
    SLipSyncEntry* m_end;
    unsigned long size() const { return m_last - m_first; }
    SLipSyncEntry* begin() { return m_first; }
    void resize(unsigned long size, const SLipSyncEntry& value);   // 0x00A43F20
};
class CBankFile {
public:
    virtual void v00();
protected:
    virtual void NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data);   // 0x009CBC20
    unsigned char _pad04[0x160];
};
class CLipSyncDataBank : public CBankFile {
protected:
    CLipSyncEntryVector m_entries;                 // +0x164
    virtual void NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data);
};

void CLipSyncDataBank::NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data)
{
    if (index >= m_entries.size())
        m_entries.resize(index + 1, SLipSyncEntry());
    unsigned long bits = *(const unsigned long*)data.GetData();
    (m_entries.begin() + index)->m_length = *(float*)&bits;
    CBankFile::NotifyEntryAdded(id, index, data);
}
