// CSimulationTable_00ac41d0::Resize(unsigned long count) -- resize the 8-byte-entry vector at +4: erase the tail when shrinking, else insert default entries at the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC41D0, length 80 bytes.
// Callees: the vector's erase @ 0x00ABEA40 (landed CRetailVector_pod8::erase: first+count, last) and insert @ 0x00AC42F0 (at last, count - size, a default entry
// in the two-word frame; the manifest labels are propagated). Shape: `add ecx,4` forms the vector, the size (`sub ; sar 3`) is recomputed for the insert arm,
// the default entry's constructor is empty (the frame words are never written). The chunk label "CEnginePrimitiveSimulationInfo::FindSimulation" is propagated;
// the class is named by address.
struct SSimulationEntry { void* m_pSimulation; long m_tag; SSimulationEntry() { } };   // 8 bytes; the default constructor writes nothing

struct CRetailVector_simulation {
    SSimulationEntry* m_pFirst;
    SSimulationEntry* m_pLast;
    SSimulationEntry* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    SSimulationEntry* erase(SSimulationEntry* first, SSimulationEntry* last);                       // 0x00ABEA40
    void insert(SSimulationEntry* where, unsigned long count, const SSimulationEntry& value);     // 0x00AC42F0
    void resize(unsigned long count)
    {
        if (count < size())
            erase(m_pFirst + count, m_pLast);
        else
            insert(m_pLast, count - size(), SSimulationEntry());
    }
};

struct CSimulationTable_00ac41d0 {
    long                     m_a;
    CRetailVector_simulation m_entries;            // +0x04
    void Resize(unsigned long count);
};

void CSimulationTable_00ac41d0::Resize(unsigned long count)
{
    m_entries.resize(count);
}