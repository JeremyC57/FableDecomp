// CAnimationSet::GetAnimsWithNameStartingWith(const CCharString& prefix, vector<const CAnimationEntry*>* out, const CAnimFilterBase* filter) -- collect every animation whose name starts with `prefix` and that the filter accepts; consecutive entries with the same key are skipped once their name failed to match
// Grade: DIFFER(253v255) -- frame packing: retail homes the GetString result temporary in the dead `out` argument slot (and re-reads its handle from there for the name text, prefix text in eax / name in ecx), ours gives it a fourth frame word and uses the returned pointer (register swap in the strncmp arguments); tried a cached `out` local and an unnamed overflow tag (258)
// Leaf: ?GetAnimsWithNameStartingWith@CAnimationSet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DCB00, length 255 bytes (`ret 0Ch`).
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828 (CCharString by value),
//          strncmp @ 0x00BFEAF8, ~CCharString @ 0x0099EAE0, the filter's virtual operator() (slot 0),
//          vector<const CAnimationEntry*>::_M_insert_overflow @ 0x005E1420 (manifest "VectorInsertSequence").
// Layout: Ego_r.pdb CAnimationSet: Anims (CVectorMap<unsigned long, CAnimationEntry>, 0x14-byte elements) at +0;
//          CAnimationEntry.AnimName (CDefString) at +4.
// Shape: the name test is an inline `StartsWith` on the GetString temporary (the temporary is built before the strncmp
//          arguments; the free strncmp form evaluates the prefix first); STLport push_back inline.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

struct CCharStringData { char* PData; unsigned long StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    operator const char*() const { return PStringData == 0 ? "" : PStringData->PData; }
    bool StartsWith(const CCharString& prefix) const { return strncmp(*this, prefix, prefix.GetLength()) == 0; }
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefString { long TablePos; };
struct CAnimationEntry {                                // 16 bytes
    long       BankIndex;                               // +0x00
    CDefString AnimName;                                // +0x04
    CDefString GroupName;                               // +0x08
    void*      Components;                              // +0x0c
};
struct CAnimFilterBase {
    virtual bool operator()(const CAnimationEntry* entry) const;   // slot 0
};

struct __true_type {};
struct CAnimEntryPtrVector {                            // STLport vector<const CAnimationEntry*>
    const CAnimationEntry** _M_start;
    const CAnimationEntry** _M_finish;
    const CAnimationEntry** _M_end_of_storage;
    void _M_insert_overflow(const CAnimationEntry** pos, const CAnimationEntry* const& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x005E1420
    void push_back(const CAnimationEntry* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};

struct SAnimMapEntry {                                  // CVectorMap<unsigned long, CAnimationEntry> element (0x14)
    unsigned long   Key;
    CAnimationEntry Entry;
};

class CAnimationSet {
public:
    SAnimMapEntry* _M_start;                            // +0x00 Anims
    SAnimMapEntry* _M_finish;                           // +0x04
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimEntryPtrVector* out, const CAnimFilterBase* filter);
};

void CAnimationSet::GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimEntryPtrVector* out, const CAnimFilterBase* filter)
{
    bool skipKey = false;
    unsigned long lastKey = 0;
    for (SAnimMapEntry* it = _M_start; it != _M_finish; ++it) {
        unsigned long key = it->Key;
        if (key != lastKey || !skipKey) {
            if (g_DefStringTable.GetString(it->Entry.AnimName.TablePos).StartsWith(prefix)) {
                if ((*filter)(&it->Entry))
                    out->push_back(&it->Entry);
                skipKey = false;
            } else
                skipKey = true;
            lastKey = key;
        }
    }
}
