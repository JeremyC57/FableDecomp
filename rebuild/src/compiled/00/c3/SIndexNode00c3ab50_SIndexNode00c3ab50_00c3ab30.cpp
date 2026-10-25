// SIndexNode_00c3ab50::SIndexNode_00c3ab50(void* value, unsigned long index) -- constructs an 8-byte index-map node: the value at +0, the index at +4
// Leaf: ??0SIndexNode_00c3ab50@@QAE@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3AB30, 18 bytes, __thiscall `ret 8`, returns `this`:
//   mov edx,[esp+8] ; mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; mov [eax+4],edx ; ret 8
// Naming (sweep1, 2026-10-25): was landed as CDateAndTime::CDateAndTime (a propagated manifest label over a `T::Init`
//   stand-in); FINDINGS chunk 438 ("0xC3AB30/0xC3B0C0 CDateAndTime ctors = SIndexNode(value,index)"). Its only caller is
//   the landed CIndexMap_00c3ab50::Add 0x00C3AB50 (call 0x00C3ABAD: `Insert(new SIndexNode_00c3ab50(value, index))`), whose source
//   declares this class and constructor; the class name is that row's, address suffix included, so the two bind one
//   symbol. The twin 0x00C3B0C0 (SIndexNode_00c3b0e0) belongs to the other index-map copy (two COMDAT copies, so two distinct
//   classes or template instances; no PDB covers libpk).
struct SIndexNode_00c3ab50 {                           // 8 bytes
    void* m_pValue;                                         // +0x00
    unsigned long m_index;                                  // +0x04
    SIndexNode_00c3ab50(void* value, unsigned long index);
};

SIndexNode_00c3ab50::SIndexNode_00c3ab50(void* value, unsigned long index)
    : m_pValue(value), m_index(index)
{
}
