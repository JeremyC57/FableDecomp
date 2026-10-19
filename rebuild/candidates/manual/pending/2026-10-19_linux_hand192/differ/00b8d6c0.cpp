// Grade: DIFFER(172v181) -- register allocation: retail keeps `this` in eax, the key in edi, end() in ebx and spills the handle (ebp) into the pair temporary's frame word, ours keeps the handle in edi and spills the mapper pointer; the entry zeroes come from two zero registers in retail (one eax/edx pair in ours) (tried a member/free __forceinline lower_bound, cached end, local declaration order)
// CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID(unsigned long) -- take the next free occluder entry (cleared; -1 when all 0x2000 are in use) and record its handle against the retrieval ID in the sorted handle mapper (updating an existing pair, else inserting one); returns the handle
// Leaf: AllocateOccluderHandleWithRetrivalID
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8D6C0, 181 bytes (+ int3 padding), `ret 4`.
// Callees: vector<pair<unsigned long, unsigned long>>::insert(iterator, const value_type&) 0x00B8F650 (on HandleMapper).
// Layout (retail): Entries +0x4014, EntriesSize +0x401c (Ego_r.pdb / engine/CEngineOcclusionManager.h agree), HandleMapper
//   +0x2403c (the header's +0x24044 less 8: the two FlareTests CArrays before it are 12-byte vectors in retail), so `this`
//   is a local model with the PDB names. COccluderEntry {PixelCount, SlotID:30, Testing:1, Completed:1}, 8 bytes.
// Shape: the cleared entry is a default-constructed temporary assigned (two zero registers); the handle mapper search is
//   the inlined Dinkumware lower_bound (`half = count >> 1`, `count -= half + 1` on the less-than arm) with end() cached on the pair's first member; a hit overwrites second, a
//   miss inserts the {id, handle} pair temporary (built in the frame word where the handle was spilled).
struct COccluderEntry {                                // 8 bytes
    unsigned long PixelCount;                          // +0x00
    unsigned long Flags;                               // +0x04 (SlotID:30, Testing:1, Completed:1)
    COccluderEntry() : PixelCount(0), Flags(0) { }
};

struct SHandlePair {                                   // pair<unsigned long, unsigned long>
    unsigned long first;
    unsigned long second;
    SHandlePair(unsigned long a, unsigned long b) : first(a), second(b) { }
};

struct CHandleMapper {                                 // vector<pair<unsigned long, unsigned long>> (retail 12 bytes)
    SHandlePair* _M_start;
    SHandlePair* _M_finish;
    SHandlePair* _M_end_of_storage;
    SHandlePair* insert(SHandlePair* position, const SHandlePair& x);   // 0x00B8F650
    SHandlePair* begin() { return _M_start; }
    SHandlePair* end() { return _M_finish; }
};

__forceinline SHandlePair* LowerBound(SHandlePair* first, SHandlePair* last, const unsigned long& key)
{
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        if (first[half].first < key) {
            first += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}

struct CEngineOcclusionManager {
    unsigned char   _pad0000[0x4014];
    COccluderEntry* Entries;                           // +0x4014
    COccluderEntry* PrevFrameEntries;                  // +0x4018
    unsigned long   EntriesSize;                       // +0x401c
    unsigned char   _pad4020[0x2403c - 0x4020];
    CHandleMapper   HandleMapper;                      // +0x2403c (retail)
    unsigned long AllocateOccluderHandleWithRetrivalID(unsigned long retrievalID);
};

unsigned long CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID(unsigned long retrievalID)
{
    unsigned long handle;
    if (EntriesSize < 0x2000) {
        handle = EntriesSize;
        Entries[handle] = COccluderEntry();
        ++EntriesSize;
    } else
        handle = (unsigned long)-1;
    SHandlePair* last = HandleMapper.end();
    SHandlePair* it = LowerBound(HandleMapper.begin(), last, retrievalID);
    if (it != last && it->first == retrievalID) {
        it->second = handle;
        return handle;
    }
    HandleMapper.insert(it, SHandlePair(retrievalID, handle));
    return handle;
}
