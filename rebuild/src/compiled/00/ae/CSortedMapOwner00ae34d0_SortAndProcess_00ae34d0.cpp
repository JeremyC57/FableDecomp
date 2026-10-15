// CSortedMapOwner_00ae34d0::SortAndProcess() -- sorts the object's pair vector-map at +0x88 if it is dirty, then runs its +0x08 member through the processing method @ 0x00AE0710
// Leaf: SortAndProcess
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE34D0, length 59 bytes.
// Label: the catalog name "CTCVillage::OnInitialActivate" is propagated (no CTCVillage member matches: the body
// is the inline CVectorMap::Sort() of the 0x009CFB70 twin on a vector-map at +0x88). The owning class (code next to
// CParticleProgram 0x00AE3550) is not identified: descriptive names. No callers or vtable references were found.
// Callees: the /Gr STL sort of 8-byte pairs @ 0x009B8EF0 (`__fastcall`: first in ecx, last in edx, the 1-byte
// comparator by value on the stack; manifest "std::introsort_partial<pair<CCharString,CCharString>>" is propagated),
// a member @ 0x00AE0710 taking the address of the +0x08 member (manifest "CalcErrorMetric_Vertices" is a scratch label).
// Shape: CVectorMap::Sort() inline; `call ; pop esi ; ret` (the callee pops an argument this function does not have).
struct SSortedPair {
    unsigned long Key;
    unsigned long Value;
};

struct CPairCompareLess {
    unsigned char _empty;
};

void __fastcall SortPairs(SSortedPair* first, SSortedPair* last, CPairCompareLess compare);   // 0x009B8EF0

struct CPairVectorMap {                     // CVectorMap<K, V, Compare> (retail vector + comparator + dirty flag)
    SSortedPair*     _M_start;              // +0x00
    SSortedPair*     _M_finish;             // +0x04
    SSortedPair*     _M_end_of_storage;     // +0x08
    CPairCompareLess Compare;               // +0x0C
    bool             NeedsSort;             // +0x0D
    void Sort()
    {
        if (NeedsSort) {
            SortPairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
};

struct SProcessedBlock {
    unsigned char _body[0x80];
};

struct CSortedMapOwner_00ae34d0 {
    unsigned char   _pad_0x00[0x8];
    SProcessedBlock m_block;                // +0x08
    CPairVectorMap  m_map;                  // +0x88
    void Process(SProcessedBlock* block);   // 0x00AE0710
    void SortAndProcess();
};

void CSortedMapOwner_00ae34d0::SortAndProcess()
{
    m_map.Sort();
    Process(&m_block);
}