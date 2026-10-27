// CEntryArray_012b7d50::Add(const SEntry_00c46360& entry) -- slot 2: append a 12-byte entry to the array (data +4, count +8): its two trailing words, then its key, then bump the count
// Leaf: ?Add@CEntryArray_012b7d50@@UAEXABUSEntry_00c46360@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46360, 55 bytes, thiscall `ret 4`.
// Identity: slot 2 of vtable 0x012B7D50 (slot 0 `_purecall`, slots 1/3 0x00C46520 / 0x00C46500), next to the ambient-cube
//   code (CAmbientCube::UpdateDistances 0x00C45810); no capacity check. Descriptive (libpk, no PDB/RTTI).
// Shape: the value pair is copied first through `m_data[m_count]`, then the key with data and count re-read (the pair
//   stores may alias them), then `++m_count` re-read.
struct SValuePair_00c46360 {
    long m_a;
    long m_b;
};

struct SEntry_00c46360 {
    long m_key;                             // +0
    SValuePair_00c46360 m_value;            // +4
};

class CEntryArray_012b7d50 {                // vtable 0x012B7D50
public:
    virtual void v00() = 0;
    virtual void v01();
    virtual void Add(const SEntry_00c46360& entry);
    SEntry_00c46360* m_data;                // +4
    long m_count;                           // +8
};

void CEntryArray_012b7d50::Add(const SEntry_00c46360& entry)
{
    m_data[m_count].m_value = entry.m_value;
    m_data[m_count].m_key = entry.m_key;
    ++m_count;
}