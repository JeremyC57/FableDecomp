// CRetailVector_00412970::push_back(const SInputRecord_00412970& value) -- the retail vector push_back for 28-byte records: when there is room copy the record into place (`rep movsd` through a null-checked placement new) and advance `last`; else the reallocating insert (0x004125A0) with the scalar tag temporary homed in the dead argument slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412970, length 64 bytes.
// The chunk label "CInputManager::EventScanStart" is propagated; named by its effect. Shape: `rep movsd` of 7 dwords, `add dword ptr [eax+4],1Ch`, `ret 4`.
#include <new>
struct SScalarTag { SScalarTag() { } };
struct SInputRecord_00412970 { long m_words[7]; };   // 28 bytes
struct CRetailVector_00412970 {
    SInputRecord_00412970* first;
    SInputRecord_00412970* last;
    SInputRecord_00412970* end;
    void InsertGrow(SInputRecord_00412970* where, const SInputRecord_00412970& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x004125A0
    void push_back(const SInputRecord_00412970& value);
};

void CRetailVector_00412970::push_back(const SInputRecord_00412970& value)
{
    if (last != end) {
        new (last) SInputRecord_00412970(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}