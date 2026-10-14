// CRecordOwner_00aaf800::AddRecord(const SRecord_00aaf800& value) -- push a 48-byte record onto the owner's retail vector at +0x40: when there is room copy it into place (null-checked placement new, `rep movsd` of 12 dwords) and advance `last`, else the reallocating insert with the scalar tag temporary homed in the dead argument slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAF800, length 63 bytes (+ int3 padding).
// Callees: CRetailVector_00aaf800::InsertGrow(last, value, const SScalarTag&, 1, true) @ 0x00AAFE50 (the manifest label "Vector_InsertAt").
// The chunk label "CInputManager::EventScanStart" is propagated; named by its effect. Shape: `lea eax,[ecx+40h]` is the vector
// (the push_back is inlined into a one-argument member of the owner), `add dword ptr [eax+4],30h`, `ret 4`; the same shape
// as the landed 28-/44-byte push_backs (0x00412970 / 0x006E5390).
#include <new>
struct SScalarTag { SScalarTag() { } };
struct SRecord_00aaf800 { long m_words[12]; };      // 48 bytes
struct CRetailVector_00aaf800 {
    SRecord_00aaf800* first;
    SRecord_00aaf800* last;
    SRecord_00aaf800* end;
    void InsertGrow(SRecord_00aaf800* where, const SRecord_00aaf800& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00AAFE50
    void push_back(const SRecord_00aaf800& value)
    {
        if (last != end) {
            new (last) SRecord_00aaf800(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CRecordOwner_00aaf800 {
    unsigned char          _pad[0x40];
    CRetailVector_00aaf800 m_records;               // +0x40
    void AddRecord(const SRecord_00aaf800& value);
};

void CRecordOwner_00aaf800::AddRecord(const SRecord_00aaf800& value)
{
    m_records.push_back(value);
}
