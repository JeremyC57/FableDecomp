// CEntryTable_00befbc0::AddEntry(const SKey& key, const SValue& value) -- build a {key, value} record on the stack (the value's 12-byte vector copy-constructed @ 0x005BB346), push it into the table @ 0x00BEF9C0, free the copy's vector storage, mark the table dirty (+0xd)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEFBC0, length 96 bytes.
// Callees: the vector copy constructor @ 0x005BB346 (manifest "std::vector::operator=", called with the record's +0x10 and the value's +8), the push @ 0x00BEF9C0 (copies a 28-byte
// record: four words then the vector; manifest "CRandomAppearanceMorph::PushMorph"), free @ 0x00BFEA14 (the vector's first pointer when set). Shape: a 0x1c frame for the record, the two
// 8-byte halves copied through ecx/edx before the vector call, the inlined vector destructor, then the flag byte after the record's scope. The chunk label "CGuiControlTreePane::EnsureExpanded"
// is propagated (0x00BExxxx module); the class is named by address.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CRetailVector_00befbc0 {                    // 12 bytes: first, last, end of storage
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailVector_00befbc0(const CRetailVector_00befbc0& other);   // 0x005BB346
    ~CRetailVector_00befbc0()
    {
        if (m_pFirst)
            free(m_pFirst);
    }
};
struct SKey_00befbc0 {                             // 8 bytes, plain
    long m_a;
    long m_b;
};
struct SValue_00befbc0 {                           // 20 bytes: two words and the vector
    long m_c;
    long m_d;
    CRetailVector_00befbc0 m_items;
    SValue_00befbc0(const SValue_00befbc0& other) : m_c(other.m_c), m_d(other.m_d), m_items(other.m_items) {}
};
struct SEntry_00befbc0 {                           // 28 bytes: the pair pushed into the table
    SKey_00befbc0   first;
    SValue_00befbc0 second;
    SEntry_00befbc0(const SKey_00befbc0& key, const SValue_00befbc0& value) : first(key), second(value) {}
};

struct CEntryTable_00befbc0 {
    unsigned char _pad[0xd];
    bool          m_dirty;                         // +0x0d
    void Push(const SEntry_00befbc0& entry);       // 0x00BEF9C0
    void AddEntry(const SKey_00befbc0& key, const SValue_00befbc0& value);
};

void CEntryTable_00befbc0::AddEntry(const SKey_00befbc0& key, const SValue_00befbc0& value)
{
    {
        SEntry_00befbc0 entry(key, value);
        Push(entry);
    }
    m_dirty = true;
}
