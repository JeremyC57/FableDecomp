// CRetailVector_006e5390::push_back(const SInputRecord_006e5390& value) -- the retail vector push_back for 44-byte records: when there is room copy the record into place (`rep movsd` through a null-checked placement new) and advance `last`; else the reallocating insert (0x006E5100) with the scalar tag temporary homed in the dead argument slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5390, length 64 bytes.
// The chunk label "CInputManager::EventScanStart" is propagated; named by its effect. Shape: `rep movsd` of 11 dwords, `add dword ptr [eax+4],2Ch`, `ret 4`.
#include <new>
struct SScalarTag { SScalarTag() { } };
struct SInputRecord_006e5390 { long m_words[11]; };   // 44 bytes
struct CRetailVector_006e5390 {
    SInputRecord_006e5390* first;
    SInputRecord_006e5390* last;
    SInputRecord_006e5390* end;
    void InsertGrow(SInputRecord_006e5390* where, const SInputRecord_006e5390& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x006E5100
    void push_back(const SInputRecord_006e5390& value);
};

void CRetailVector_006e5390::push_back(const SInputRecord_006e5390& value)
{
    if (last != end) {
        new (last) SInputRecord_006e5390(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
