// CLipSyncDataBank::NotifyBankSize(unsigned long size, const CArray<CBankFileDataTypeCount>& counts) -- resize the entry vector at +0x164 to the bank's size, new entries zero-filled (an 8-byte zero temporary built on the stack; `resize(size, value)` @ 0x00A43F20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42E70, length 48 bytes.
// Leaf: NotifyBankSize
// Callees: 0x00A43F20 (vector resize, thiscall on the vector at +0x164). Shape: an 8-byte frame for the zero value, stores scheduled after the pushes, `ret 8`.
struct SEntry_00a42e70 {                           // two floats, zeroed by the inline constructor (immediate stores, scheduled after the pushes)
    float m_0;
    float m_4;
    SEntry_00a42e70() : m_0(0.0f), m_4(0.0f) { }
};
struct CRetailVector_00a42e70 {
    SEntry_00a42e70* m_first;
    SEntry_00a42e70* m_last;
    SEntry_00a42e70* m_end;
    void resize(unsigned long size, const SEntry_00a42e70& value);   // 0x00A43F20
};
struct CArray_CBankFileDataTypeCount;

struct CLipSyncDataBank {
    unsigned char         _pad00[0x164];
    CRetailVector_00a42e70 m_entries;              // +0x164
    void NotifyBankSize(unsigned long size, const CArray_CBankFileDataTypeCount& counts);
};

void CLipSyncDataBank::NotifyBankSize(unsigned long size, const CArray_CBankFileDataTypeCount& counts)
{
    m_entries.resize(size, SEntry_00a42e70());
}