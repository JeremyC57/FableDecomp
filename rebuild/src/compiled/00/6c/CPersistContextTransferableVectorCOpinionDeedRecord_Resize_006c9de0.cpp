// CPersistContext_TransferableVector<COpinionDeedRecord>::Resize(unsigned long count) -- resize the transferred vector of 8-byte opinion-deed records to `count` (STLport resize(n) inlined: erase the tail when shrinking, fill-insert zeroed records when growing)
// Leaf: ?Resize@CPersistContext_TransferableVector@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9DE0, 88 bytes (+8 int3 padding), `ret 4`.
// Callees: vector<COpinionDeedRecord>::erase(first, last) @ 0x006C7FE0 (landed as CRetailVector<pod8>::erase),
// vector<COpinionDeedRecord>::_M_fill_insert(pos, n, x) @ 0x006C8610 (manifest `VectorLike_Insert_range_fill`); thiscall.
// Slot 1 of the transferable-vector vtable 0x0125E9DC (slot 0 count 0x006C5080, slot 2 TransferOut 0x006C78B0, slot 3
// TransferIn 0x006C9310), the wrapper the landed CPersistContext::Transfer<COpinionDeedRecord> 0x006C9EE0 builds. The
// manifest's `<CCrimeWaitingForBribe>` is propagated from the byte twin 0x007B5310.
// Element: 8 bytes, a long written by the long writer 0x009E2560 and a 4-byte member with its own traits (0x006C7960,
// default 0) -- see the TransferOut traits 0x006C78F0; the PDB has no COpinionDeedRecord, so the members are descriptive.
// Shape: STLport's one-argument `resize(n) { resize(n, T()); }` with BOTH resizes __forceinline (a plain inline leaves
// the two-argument resize out of line): TransVector stays in ecx, the default temporary's zero stores (through eax) are
// scheduled after the size computation; the size is `(finish - start) >> 3`; the shrink arm passes `start + n` and the
// cached finish, the grow arm reloads finish and passes (finish, n - size, &x); both arms return separately (`ret 4`).
struct COpinionDeedRecord {                        // 8 bytes
    long m_deedType;                               // +0x0
    long m_value;                                  // +0x4
    COpinionDeedRecord() : m_deedType(0), m_value(0) { }
};

struct COpinionDeedRecordVector {               // STLport vector<COpinionDeedRecord>
    COpinionDeedRecord* _M_start;
    COpinionDeedRecord* _M_finish;
    COpinionDeedRecord* _M_end_of_storage;
    COpinionDeedRecord* begin() { return _M_start; }
    COpinionDeedRecord* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    COpinionDeedRecord* erase(COpinionDeedRecord* first, COpinionDeedRecord* last);                         // 0x006C7FE0
    void _M_fill_insert(COpinionDeedRecord* position, unsigned long n, const COpinionDeedRecord& x);           // 0x006C8610
    void insert(COpinionDeedRecord* position, unsigned long n, const COpinionDeedRecord& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned long newSize, const COpinionDeedRecord& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    __forceinline void resize(unsigned long newSize) { resize(newSize, COpinionDeedRecord()); }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();                       // slot 0
    virtual void Resize(unsigned long count);      // slot 1
    COpinionDeedRecordVector* TransVector;      // +0x4
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}