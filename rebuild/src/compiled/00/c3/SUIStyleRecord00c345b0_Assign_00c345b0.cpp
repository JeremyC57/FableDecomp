// SUIStyleRecord_00c345b0::Assign(const SUIStyleRecord_00c345b0&) -- copies the 16-byte record unless it already equals the source; returns 1 when it changed, 0 when it was equal
// Leaf: Assign
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C345B0, 58 bytes (+6 int3 padding). Callee: the record equality test
// @ 0x00C20370 (landed as "CTCScriptedControl::IsActive": a __fastcall (record, other) that compares the byte at +0xC
// and then the rest through 0x00C20330; the caller tests only `al`, so it returns bool). Caller 0x00C20E62: a UI control assigns its record at +0x38 and ORs 6 into its
// dirty flags (+0xA8) when the result is non-zero.
// Label: the manifest "CStringParser::PeekNextItem" is propagated; the body is a change-detecting record assignment
// (named by address here).
// Shape: the equality helper is __fastcall (`this` in ecx, the source's address in edx); the long result is
// `xor eax,eax` / `mov eax,1`; the copy is member by member through eax/ecx/edx, i.e. a memberwise operator= (a POD
// assignment copies with `movsd`).
struct SUIStyleRecord_00c345b0 {
    unsigned long m_word0;                       // +0x0
    unsigned long m_word4;                       // +0x4
    unsigned long m_word8;                       // +0x8
    unsigned long m_wordC;                       // +0xC (its low byte is compared first)
    SUIStyleRecord_00c345b0& operator=(const SUIStyleRecord_00c345b0& other)
    {
        m_word0 = other.m_word0;
        m_word4 = other.m_word4;
        m_word8 = other.m_word8;
        m_wordC = other.m_wordC;
        return *this;
    }
    long Assign(const SUIStyleRecord_00c345b0& other);
};

bool __fastcall UIStyleRecordEquals(const SUIStyleRecord_00c345b0* record, const SUIStyleRecord_00c345b0* other);   // 0x00C20370

long SUIStyleRecord_00c345b0::Assign(const SUIStyleRecord_00c345b0& other)
{
    if (UIStyleRecordEquals(this, &other))
        return 0;
    *this = other;
    return 1;
}