// CVectorPairOwner_00ae3330::ReserveBoth(unsigned long) -- reserves the same element count in the two 16-byte-element vectors at +0xFC and +0x108
// Leaf: ReserveBoth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE3330, 37 bytes (+11 int3 padding). Callee: the 16-byte-element
// vector `reserve(n)` @ 0x00AE80F0 (manifest "Container_Reserve": compares (end_of_storage-first)>>4 with n and
// reallocates through malloc(n << 4)), called on two adjacent 12-byte vectors (+0xFC, +0x108).
// Label: the manifest's "CSpellContainerList::AddChild" is a propagated name over byte-identical two-member forwarders;
// this body's only caller is 0x00AC3E80 (render module), and the callee is a vector reserve, so the owner is modelled
// descriptively. Shape: the argument is loaded once into edi and pushed for both calls; `this` lives in esi.
struct SElement16 { unsigned long m_words[4]; };

struct CVector16 {
    SElement16* m_pFirst;
    SElement16* m_pLast;
    SElement16* m_pEnd;
    void Reserve(unsigned long count);          // 0x00AE80F0
};

struct CVectorPairOwner_00ae3330 {
    unsigned char _pad00[0xFC];
    CVector16     m_first;                       // +0xFC
    CVector16     m_second;                      // +0x108
    void ReserveBoth(unsigned long count);
};

void CVectorPairOwner_00ae3330::ReserveBoth(unsigned long count)
{
    m_first.Reserve(count);
    m_second.Reserve(count);
}
