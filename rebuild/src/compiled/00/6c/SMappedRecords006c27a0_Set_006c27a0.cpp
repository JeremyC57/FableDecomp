// SMappedRecords_006c27a0::Set(const CRecordVector& records, const CRecordMap& map, long value) -- assign the vector at +0x10, the map at +0, then store the word at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C27A0, length 39 bytes (48 with padding).
// Leaf: Set
// The chunk label (CSpellContainerList::AddChild) is propagated garbage; named by address.
// Callees: 0x006C2D40 (vector<T>::operator= for 28-byte elements: the 0x92492493 / `sar 4` divide by 28;
// manifest label CCombatWheel_CopyAssign), 0x006B9E00 (a red-black tree operator= on `this`: head at +0,
// node count at +4; manifest label LTextTreeWalkThrough_CopyAssign).
// Shape: each argument read from its slot right before its use (no frame), the vector assigned on
// `lea ecx,[esi+10h]`, the map on `this`, the word stored last, `ret 0Ch`: three assignments in source order.
struct SRecord_006c27a0 { long m_words[7]; };      // 28 bytes

struct CRecordVector {                              // retail vector<SRecord>: first / last / end
    SRecord_006c27a0* first;
    SRecord_006c27a0* last;
    SRecord_006c27a0* end;
    CRecordVector& operator=(const CRecordVector& other);   // 0x006C2D40
};

struct CRecordMap {                                 // the tree: head, node count, ...
    void* m_pHead;                                  // +0x00
    long  m_count;                                  // +0x04
    long  m_8, m_c;                                 // +0x08..+0x0f
    CRecordMap& operator=(const CRecordMap& other);   // 0x006B9E00
};

struct SMappedRecords_006c27a0 {
    CRecordMap    m_map;                            // +0x00
    CRecordVector m_records;                        // +0x10
    long          m_value;                          // +0x1c
    void Set(const CRecordVector& records, const CRecordMap& map, long value);
};

void SMappedRecords_006c27a0::Set(const CRecordVector& records, const CRecordMap& map, long value)
{
    m_records = records;
    m_map = map;
    m_value = value;
}