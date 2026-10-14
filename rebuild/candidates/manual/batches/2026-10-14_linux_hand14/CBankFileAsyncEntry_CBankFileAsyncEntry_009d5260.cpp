// CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync*, unsigned long) -- bind to one bank entry, cache its aligned size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5260, oracle length 52 bytes.
// Callees: CBase::CBase @ 0x0099A2F0 (the root with vtable 0x0129A7C4), CBankFile::GetEntryDataSize @ 0x009CC240,
//          CBankFile::Align @ 0x009CBB90.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase::CBase ; mov eax,[esp+0Ch] ; mov ecx,[esp+8] ; push eax ; mov dword ptr [esi],offset CBankFileAsyncEntry::`vftable' ;
//                mov [esi+4],ecx ; mov [esi+8],eax ; call GetEntryDataSize ; mov ecx,[esi+4] ; push eax ; call Align ; mov [esi+0Ch],eax ; mov eax,esi ; pop esi ; ret 8
// The out-of-line root constructor runs first, then this class's vtable (0x0129B880 per
// rtti-vtables.tsv), the bank pointer and the entry index; the body computes
// `m_pBank->Align(m_pBank->GetEntryDataSize(index))`: the inner call's `this` is the just-stored
// bank still in ecx, the outer re-reads the member after the call.
struct CBase {                       // the 4-slot root at 0x0099A2F0, stores vtable 0x0129A7C4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    CBase();                         // 0x0099A2F0
};

struct CBankFileAsync {
    unsigned long GetEntryDataSize(unsigned long index) const;   // 0x009CC240 (CBankFile)
    unsigned long Align(unsigned long size) const;               // 0x009CBB90 (CBankFile)
};

struct CBankFileAsyncEntry : CBase {
    CBankFileAsync* m_pBank;         // +0x04
    unsigned long   m_index;         // +0x08
    unsigned long   m_alignedSize;   // +0x0c
    CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index);   // 0x009D5260
};

CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index)
    : CBase(), m_pBank(bank), m_index(index)
{
    m_alignedSize = m_pBank->Align(m_pBank->GetEntryDataSize(index));
}
